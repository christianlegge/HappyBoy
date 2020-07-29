#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <memory>

#include "PPU.h"
#include "APU.h"

class CPU;

class Bus
{
public:
	Bus(std::shared_ptr<PPU> ppu, std::shared_ptr<APU> apu);
	uint8_t read(uint16_t addr);
	void write(uint16_t addr, uint8_t data);
	std::vector<std::string> getMemory(uint16_t addr);
	uint8_t ram[0x10000];
	uint8_t* cart;
	uint8_t* bootrom;
	std::shared_ptr<CPU> cpu;
private:
	std::shared_ptr<PPU> ppu;
	std::shared_ptr<APU> apu;
	bool bootRomControl = false;
};

