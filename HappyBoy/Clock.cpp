#include "Clock.h"

Clock::Clock(std::shared_ptr<CPU> cpu, std::shared_ptr<PPU> ppu, std::shared_ptr<APU> apu) : cpu(cpu), ppu(ppu), apu(apu)
{
	frame_time = 1 / 59.7;
	last_frame = std::chrono::steady_clock::now();
}

void Clock::tick()
{
	if (!paused && !cpu->stopped) {
		if (cpu->tick() == breakpoint) {
			paused = true;
		}
		ppu->tick();
	}
	if (ppu->frameComplete()) {
		std::chrono::duration<float> diff = std::chrono::steady_clock::now() - last_frame;
		while (diff.count() < frame_time) {
			diff = std::chrono::steady_clock::now() - last_frame;
		}
		last_frame = std::chrono::steady_clock::now();
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