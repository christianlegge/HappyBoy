#pragma once

#include <cstdint>
#include <memory>

class Bus;
class CPU;

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} color;

class PPU
{
public:
	void tick();
	color* getScreen();
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
	union {
		struct {
			uint8_t c0 : 2;
			uint8_t c1 : 2;
			uint8_t c2 : 2;
			uint8_t c3 : 2;
		};
		uint8_t reg = 0xE4;
	} BGP;
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
	void getTileset(color* tileset);
	void getTilemap(color* tilemap);
private:
	int cycles = 0;
	int screenx = -1;
	color screen[160 * 144];
#if 0
	color index[4] = { { 255, 255, 255 }, { 170, 170, 170 } ,{ 85, 85, 85 } ,{ 0, 0, 0 } };
#else
	color index[4] = { { 155, 188, 18 }, { 139, 172, 15 } ,{ 48, 98, 48 } ,{ 15, 56, 15 } };
#endif
	uint8_t read(uint16_t addr);
	bool screenFrameReady = false;
};

