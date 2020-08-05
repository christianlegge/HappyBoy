#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <memory>

#include "PPU.h"
#include "APU.h"

class CPU;

enum class GBButton {
	Up, Down, Left, Right, B, A, Select, Start
};

class Bus
{
public:
	Bus(std::shared_ptr<PPU> ppu, std::shared_ptr<APU> apu);
	uint8_t read(uint16_t addr);
	void write(uint16_t addr, uint8_t data);
	std::vector<std::string> getMemory(uint16_t addr);
	void button(GBButton button, bool pressed);
	uint8_t ram[0x10000];
	uint8_t* cart;
	uint8_t* bootrom;
	std::shared_ptr<CPU> cpu;

private:
	std::shared_ptr<PPU> ppu;
	std::shared_ptr<APU> apu;
	bool bootRomControl = false;
	union {
		struct {
			bool right : 1;
			bool left : 1;
			bool up : 1;
			bool down : 1;
		};
		uint8_t reg = 0xFF;
	} dpad;
	union {
		struct {
			bool a : 1;
			bool b : 1;
			bool select : 1;
			bool start : 1;
		};
		uint8_t reg = 0xFF;
	} buttons;
	uint8_t joypad = 0x7F;
};

