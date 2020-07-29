#include "Clock.h"

Clock::Clock(std::shared_ptr<CPU> cpu, std::shared_ptr<PPU> ppu, std::shared_ptr<APU> apu) : cpu(cpu), ppu(ppu), apu(apu)
{
}

void Clock::tick()
{
	if (!paused) {
		if (cpu->tick() == 0xffff) {
			paused = true;
		}
		ppu->tick();
	}
}

void Clock::step() {
	if (paused) {
		do {
			cpu->tick();
			ppu->tick();
		} while (!cpu->instructionComplete());
	}
}

void Clock::frame() {
	if (paused) {
		do {
			cpu->tick();
			ppu->tick();
		} while (!ppu->frameComplete());
	}
}