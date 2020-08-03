#include "Clock.h"

Clock::Clock(std::shared_ptr<CPU> cpu, std::shared_ptr<PPU> ppu, std::shared_ptr<APU> apu) : cpu(cpu), ppu(ppu), apu(apu)
{
}

void Clock::tick()
{
	if (!paused && !cpu->stopped) {
		if (cpu->tick() == breakpoint) {
			paused = true;
		}
		ppu->tick();
	}
}

void Clock::step() {
	if (paused && !cpu->stopped) {
		do {
			cpu->tick();
			ppu->tick();
		} while (!cpu->instructionComplete());
	}
}

void Clock::frame() {
	if (paused && !cpu->stopped) {
		do {
			cpu->tick();
			ppu->tick();
		} while (!ppu->frameComplete());
	}
}