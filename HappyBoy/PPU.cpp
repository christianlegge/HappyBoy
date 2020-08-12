#include "PPU.h"
#include "CPU.h"
#include "Bus.h"

#include <thread>
#include <iostream>

void PPU::tick()
{
	cycles++;

	if (STAT.screenmode == OAMSCAN_MODE) {
		if (!scanned_oam) {
			Sprite* oam = (Sprite*)bus->getOamPointer();
			num_sprites_in_line = 0;
			for (int i = 0; i < 40; i++) {
				Sprite s = oam[i];
				uint8_t height = LCDC.spritesize ? 16 : 8;
				if ((s.ypos - 16) <= LY && LY < (s.ypos - 16) + height) {
					sprites_for_scanline[num_sprites_in_line++] = s;
					if (num_sprites_in_line >= 10) {
						break;
					}
				}
			}
			scanned_oam = true;
		}

		if (cycles == OAM_CYCLES) {
			STAT.screenmode = DRAWING_MODE;
		}
	}

	else if (STAT.screenmode == DRAWING_MODE) {
		uint8_t SCX_consumed = 0;
		while (pixel_fifo.size() < 8) {
			uint16_t mapaddr = drawing_window ? (LCDC.windowtilemap ? 0x9C00 : 0x9800) : (LCDC.bgtilemap ? 0x9C00 : 0x9800);
			uint16_t setaddr = LCDC.bgtileset ? 0x8000 : 0x8800;
			uint8_t scanline = drawing_window ? LY - WY : LY + SCY;
			int tilex = fetch_x / 8;
			int tiley = scanline / 8;
			uint8_t tilenum = read(mapaddr + tiley * 32 + tilex);

			if (setaddr == 0x8800) {
				tilenum += 128;
			}

			int line = scanline % 8;
			uint8_t linedata1 = read(setaddr + tilenum * 16 + line * 2);
			uint8_t linedata2 = read(setaddr + 1 + tilenum * 16 + line * 2);
			for (int i = 0; i < 8; i++) {
				uint8_t color = ((linedata2 & (1 << (7 - i))) ? 2 : 0) | ((linedata1 & (1 << (7 - i))) ? 1 : 0);
				pixel_fifo.push({ color, 0 });
			}

			fetch_x += 8;

			if (screenx == 0 && !drawing_window) {
				for (int i = 0; i < 8 && SCX_consumed != SCX; i++, SCX_consumed++) {
					pixel_fifo.pop();
				}
			}
		}

		Sprite* s = nullptr;
		for (int i = 0; i < num_sprites_in_line; i++) {
			if ((sprites_for_scanline[i].xpos - 8) == screenx) {
				s = &sprites_for_scanline[i];
			}
		}
		if (s) {
			Pixel p[8];
			uint16_t setaddr = 0x8000;

			int line = (uint8_t)(LY - (s->ypos - 16));
			uint8_t linedata1 = read(setaddr + s->tile * 16 + line * 2);
			uint8_t linedata2 = read(setaddr + 1 + s->tile * 16 + line * 2);
			if (s->flags.xflip) {
				for (int i = 0; i < 8; i++) {
					uint8_t color = ((linedata1 & (1 << (i))) ? 2 : 0) | ((linedata2 & (1 << (i))) ? 1 : 0);
					p[i] = { color, 1 };
				}
			}
			else {
				for (int i = 0; i < 8; i++) {
					uint8_t color = ((linedata1 & (1 << (7 - i))) ? 2 : 0) | ((linedata2 & (1 << (7 - i))) ? 1 : 0);
					p[i] = { color, 1 };
				}
			}
			pixel_fifo.mix_sprite(p, s->flags.priority);

		}

		Palette pal;
		Pixel p = pixel_fifo.pop();
		if (p.source == 0) {
			pal = BGP;
		}
		else {
			pal = sprites_for_scanline[p.source - 1].flags.dmg_palette ? OBP1 : OBP0;
		}

		switch (p.color) {
		case 0:
			screen[LY * 160 + screenx] = index[pal.c0];
			break;
		case 1:
			screen[LY * 160 + screenx] = index[pal.c1];
			break;
		case 2:
			screen[LY * 160 + screenx] = index[pal.c2];
			break;
		case 3:
			screen[LY * 160 + screenx] = index[pal.c3];
			break;
		}

		screenx++;
		if (screenx == 160) {
			STAT.screenmode = HBLANK_MODE;
		}
	}

	else if (STAT.screenmode == HBLANK_MODE) {
		if (cycles > SCANLINE_CYCLES) {
			cycles = 0;
			pixel_fifo.clear();
			fetch_x = 0;
			screenx = 0;
			scanned_oam = false;
			drawing_window = false;
			LY++;
			if (LY >= 144) {
				STAT.screenmode = VBLANK_MODE;
				cpu->interrupt(0x0040);
			}
			else {
				STAT.screenmode = OAMSCAN_MODE;
			}
		}
	}

	else if (STAT.screenmode == VBLANK_MODE) {
		if (cycles == SCANLINE_CYCLES) {
			cycles = 0;
			LY++;
			if (LY >= 154) {
				screenFrameReady = true;
				LY = 0;
				STAT.screenmode = OAMSCAN_MODE;
			}
		}
	}

	bool tmp = stat_irq;
	stat_irq = ((LY == LYC) && STAT.lyccheck) ||
		((STAT.screenmode == HBLANK_MODE) && STAT.hcheck) ||
		((STAT.screenmode == OAMSCAN_MODE) && STAT.oamcheck) ||
		((STAT.screenmode == VBLANK_MODE) && (STAT.vcheck || STAT.oamcheck));
	if (stat_irq && !tmp) {
		cpu->interrupt(0x0048);
	}
}

Color* PPU::getScreen()
{
	return screen;
}

bool PPU::frameComplete()
{
	return LY == 0 && cycles == 1;
}

bool PPU::getScreenFrameReady()
{
	if (screenFrameReady) {
		screenFrameReady = false;
		return true;
	}
	return false;
}

void PPU::getTileset(Color* tileset)
{
	for (int tilex = 0; tilex < 16; tilex++) {
		for (int tiley = 0; tiley < 24; tiley++) {
			for (int line = 0; line < 8; line++) {
				int numtile = tiley * 16 + tilex;
				uint8_t linedata1 = read(0x8000 + numtile * 16 + line*2);
				uint8_t linedata2 = read(0x8001 + numtile * 16 + line*2);
				for (int col = 0; col < 8; col++) {
					int screenx = tilex * 8 + col;
					int screeny = tiley * 8 + line;
					int color = ((linedata2 & (1 << (7 - col))) ? 2 : 0) + ((linedata1 & (1 << (7 - col))) ? 1 : 0);
					tileset[screeny * 16 * 8 + screenx] = index[color];
				}
			}
		}
	}
}

void PPU::getTilemap(Color* tilemap)
{
	uint16_t mapaddr = LCDC.bgtilemap ? 0x9C00 : 0x9800;
	uint16_t setaddr = LCDC.bgtileset ? 0x8000 : 0x8800;
	for (int tilex = 0; tilex < 32; tilex++) {
		for (int tiley = 0; tiley < 32; tiley++) {
			uint8_t tilenum = read(mapaddr + tiley * 32 + tilex);
			if (setaddr == 0x8800) {
				tilenum += 128;
			}
			for (int line = 0; line < 8; line++) {
				uint8_t linedata1 = read(setaddr + tilenum * 16 + line * 2);
				uint8_t linedata2 = read(setaddr + 1 + tilenum * 16 + line * 2);
				for (int col = 0; col < 8; col++) {
					int screenx = tilex * 8 + col;
					int screeny = tiley * 8 + line;
					int color = ((linedata2 & (1 << (7 - col))) ? 2 : 0) + ((linedata1 & (1 << (7 - col))) ? 1 : 0);
					tilemap[screeny * 32 * 8 + screenx] = index[color];
				}
			}
		}
	}
}

uint8_t PPU::read(uint16_t addr)
{
	return bus->read(addr);
}
