#include "PPU.h"
#include "CPU.h"
#include "Bus.h"

#include <thread>
#include <iostream>

void PPU::tick()
{
	if (cycles > 80 && screenx < 160) {
		screenx++;
		if (screenx == 160) {
			STAT.screenmode = 0;
		}
	}

	if (LY < 144 && 0 <= screenx && screenx < 160) {
		STAT.screenmode = 3;
		uint16_t mapaddr = LCDC.bgtilemap ? 0x9C00 : 0x9800;
		uint16_t setaddr = LCDC.bgtileset ? 0x8000 : 0x8800;
		int tilex = (uint8_t)(screenx + SCX) / 8;
		int tiley = (uint8_t)(LY + SCY) / 8;
		int tilenum = read(mapaddr + tiley * 32 + tilex);
		int line = (uint8_t)(LY + SCY) % 8;

		uint8_t linedata1 = read(setaddr + tilenum * 16 + line * 2);
		uint8_t linedata2 = read(setaddr + 1 + tilenum * 16 + line * 2);
		int col = (uint8_t)(screenx + SCX) % 8;

		int color = ((linedata1 & (1 << (7 - col))) ? 2 : 0) + ((linedata2 & (1 << (7 - col))) ? 1 : 0);
		if (color != 0) {
			int asdasd = 0;
		}
		switch (color) {
		case 0:
			color = BGP.c0;
			break;
		case 1:
			color = BGP.c1;
			break;
		case 2:
			color = BGP.c2;
			break;
		case 3:
			color = BGP.c3;
			break;
		}
		screen[LY * 160 + screenx] = index[color];
	}

	cycles++;

	if (cycles > (20 + 43 + 51) * 4) {
		cycles = 0;
		screenx = -1;
		LY++;
		if (LY == 144) {
			cpu->interrupt(0x0040);
			STAT.screenmode = 3;
		}
	}
	if (LY > 154) {
		LY = 0;
		STAT.screenmode = 2;
	}

	bool tmp = stat_irq;
	stat_irq = ((LY == LYC) && STAT.lyccheck) ||
		((STAT.screenmode == 0) && STAT.hcheck) ||
		((STAT.screenmode == 2) && STAT.oamcheck) ||
		((STAT.screenmode == 1) && (STAT.vcheck || STAT.oamcheck));
	if (stat_irq && !tmp) {
		cpu->interrupt(0x0048);
	}
}

color* PPU::getScreen()
{
	return screen;
}

bool PPU::frameComplete()
{
	return LY == 0 && cycles == 0;
}

void PPU::getTileset(color* tileset)
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
					int color = ((linedata1 & (1 << (7 - col))) ? 2 : 0) + ((linedata2 & (1 << (7 - col))) ? 1 : 0);
					tileset[screeny * 16 * 8 + screenx] = index[color];
				}
			}
		}
	}
}

void PPU::getTilemap(color* tilemap)
{
	uint16_t mapaddr = LCDC.bgtilemap ? 0x9C00 : 0x9800;
	uint16_t setaddr = LCDC.bgtileset ? 0x8000 : 0x8800;
	for (int tilex = 0; tilex < 32; tilex++) {
		for (int tiley = 0; tiley < 32; tiley++) {
			int tilenum = read(mapaddr + tiley * 32 + tilex);
			for (int line = 0; line < 8; line++) {
				uint8_t linedata1 = read(setaddr + tilenum * 16 + line * 2);
				uint8_t linedata2 = read(setaddr + 1 + tilenum * 16 + line * 2);
				for (int col = 0; col < 8; col++) {
					int screenx = tilex * 8 + col;
					int screeny = tiley * 8 + line;
					int color = ((linedata1 & (1 << (7 - col))) ? 2 : 0) + ((linedata2 & (1 << (7 - col))) ? 1 : 0);
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


