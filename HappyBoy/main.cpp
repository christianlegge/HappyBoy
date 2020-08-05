#include <memory>
#include <iostream>
#include <fstream>
#include <chrono>
#include "Screen.h"
#include "Speaker.h"


int main(int argc, char* argv[]) {
	if (argc != 3) {
		std::cout << "Usage: " << argv[0] << " <width> <height>" << std::endl;
		return 0;
	}
	std::ifstream bootfile("dmg_boot.bin", std::ios_base::binary);
	unsigned char* bootrom = new unsigned char[256];
	bootfile.read((char*)bootrom, 256);

	//std::ifstream romfile("mooneye/acceptance/instr/daa.gb", std::ios_base::binary);
	std::ifstream romfile("bgbtest.gb", std::ios_base::binary);
	//std::ifstream romfile("blargg_cpu_instrs/04-op r,imm.gb", std::ios_base::binary);
	//std::ifstream romfile("mealybug/m3_scx_high_5_bits.gb", std::ios_base::binary);
	romfile.seekg(0, std::ios::end);
	size_t romlength = romfile.tellg();
	romfile.seekg(0, std::ios::beg);
	uint8_t* cart = new uint8_t[romlength];
	romfile.read((char*)cart, romlength);

	std::shared_ptr<PPU> ppu = std::make_shared<PPU>();
	std::shared_ptr<APU> apu = std::make_shared<APU>();
	std::shared_ptr<Bus> bus = std::make_shared<Bus>(ppu, apu);
	bus->bootrom = bootrom;
	bus->cart = cart;
	ppu->bus = bus;
	std::shared_ptr<CPU> cpu = std::make_shared<CPU>(bus);
	ppu->cpu = cpu;
	bus->cpu = cpu;
	std::shared_ptr<Clock> clock = std::make_shared<Clock>(cpu, ppu, apu);
	Screen screen(atoi(argv[1]), atoi(argv[2]), clock, cpu, bus, ppu);
	Speaker speaker(apu);

	while (true) {
		clock->tick();
	}
	return 0;
}