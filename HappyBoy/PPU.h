#pragma once

#include <cstdint>
#include <memory>

class Bus;
class CPU;

typedef struct {
	uint8_t ypos;
	uint8_t xpos;
	uint8_t tile;
	union {
		struct {
			uint8_t cgb_palette : 3;
			bool bank : 1;
			bool dmg_palette : 1;
			bool xflip : 1;
			bool yflip : 1;
			bool priority : 1;
		};
		uint8_t reg;
	} flags;
} Sprite;

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} Color;

typedef struct {
	uint8_t color : 2;
	uint8_t source : 4; // BG = 0
} Pixel;

typedef union {
	struct {
		uint8_t c0 : 2;
		uint8_t c1 : 2;
		uint8_t c2 : 2;
		uint8_t c3 : 2;
	};
	uint8_t reg;
} Palette;

class PPU
{
public:
	void tick();
	Color* getScreen();
	bool frameComplete();
	bool getScreenFrameReady();
	union {
		struct {
			bool bg : 1;
			bool sprites : 1;
			bool spritesize : 1;
			bool bgtilemap : 1;
			bool bgtileset : 1;
			bool window : 1;
			bool windowtilemap : 1;
			bool power : 1;
		};
		uint8_t reg;
	} LCDC;
	uint8_t LY;
	uint8_t LYC;
	Palette BGP;
	Palette OBP0;
	Palette OBP1;
	union {
		struct {
			uint8_t screenmode : 2;
			bool comparison : 1;
			bool hcheck : 1;
			bool vcheck : 1;
			bool oamcheck : 1;
			bool lyccheck : 1;
			bool unused : 1;
		};
		uint8_t reg;
	} STAT;
	bool stat_irq = false;
	uint8_t SCY;
	uint8_t SCX;
	uint8_t WY;
	uint8_t WX;
	std::shared_ptr<Bus> bus;
	std::shared_ptr<CPU> cpu;
	void getTileset(Color* tileset);
	void getTilemap(Color* tilemap);
private:
	int cycles = 0;
	int screenx = -1;
	uint8_t fetch_x = 0;
	bool drawing_window = false;
	Sprite sprites_for_scanline[10];
	uint8_t num_sprites_in_line;
	Color screen[160 * 144];
#if 0
	Color index[4] = { { 255, 255, 255 }, { 170, 170, 170 } ,{ 85, 85, 85 } ,{ 0, 0, 0 } };
#else
	Color index[4] = { { 155, 188, 18 }, { 139, 172, 15 } ,{ 48, 98, 48 } ,{ 15, 56, 15 } };
#endif
	uint8_t read(uint16_t addr);
	bool screenFrameReady = false;
	struct {
		uint8_t leftbound = 15;
		uint8_t rightbound = 0;
		Pixel pixels[16];

		void push(Pixel p) {
			if (rightbound == leftbound) {
				return;
			}
			pixels[rightbound] = p;
			rightbound = (rightbound + 1) % 16;
		}

		Pixel pop() {
			if ((leftbound + 1) % 16 == rightbound) {
				return { 0, 0 };
			}
			leftbound = (leftbound + 1) % 16;
			return pixels[leftbound];
		}

		int8_t size() {
			int8_t dist = rightbound - leftbound;
			if (dist <= 0) {
				dist += 16;
			}
			return dist - 1;
		}

		void clear() {
			while (size() > 0) {
				leftbound = (leftbound + 1) % 16;
			}
		}

		void mix_sprite(Pixel sprite[], bool priority) {
			for (int i = 0; i < 8; i++) {
				if (sprite[i].color != 0 && (!priority || pixels[(leftbound + 1 + i) % 16].color == 0)) {
					pixels[(leftbound + 1 + i) % 16] = sprite[i];
				}
			}
		}
	} pixel_fifo;
};

