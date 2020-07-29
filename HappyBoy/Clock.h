#pragma once

#include <memory>

#include "CPU.h"
#include "PPU.h"
#include "APU.h"

class Clock
{
public:
	Clock(std::shared_ptr<CPU> cpu, std::shared_ptr<PPU> ppu, std::shared_ptr<APU> apu);
	void tick();
	void step();
	void frame();
	bool paused = true;
private:
	std::shared_ptr<CPU> cpu;
	std::shared_ptr<PPU> ppu;
	std::shared_ptr<APU> apu;
	bool doStep = false;
};

