#include <memory>
#include <iostream>
#include <fstream>
#include <chrono>
#include "Screen.h"
#include "Speaker.h"
#include "Mappers.h"

uint8_t getRomBanks(uint8_t code) {
	switch (code) {
	case 0x00:
		return 0;
		break;
	case 0x01:
		return 4;
		break;
	case 0x02:
		return 8;
		break;
	case 0x03:
		return 16;
		break;
	case 0x04:
		return 32;
		break;
	case 0x05:
		return 64;
		break;
	case 0x06:
		return 128;
		break;
	case 0x07:
		return 256;
		break;
	case 0x08:
		return 512;
		break;
	case 0x52:
		return 72;
		break;
	case 0x53:
		return 80;
		break;
	case 0x54:
		return 96;
		break;
	}
}

size_t getRamSize(uint8_t code) {
	switch (code) {
	case 0x00:
		return 0;
		break;
	case 0x01:
		return 0x800;
		break;
	case 0x02:
		return 0x2000;
		break;
	case 0x03:
		return 0x8000;
		break;
	case 0x04:
		return 0x20000;
		break;
	case 0x05:
		return 0x10000;
		break;
	}
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		std::cout << "Usage: " << argv[0] << " <width> <height>" << std::endl;
		return 0;
	}
	std::ifstream bootfile("dmg_boot.bin", std::ios_base::binary);
	unsigned char* bootrom = new unsigned char[256];
	bootfile.read((char*)bootrom, 256);

	//std::ifstream romfile("mooneye/acceptance/interrupts/ie_push.gb", std::ios_base::binary);
	std::ifstream romfile("tetris.gb", std::ios_base::binary);
	//std::ifstream romfile("blargg_dmg_sound/02-len ctr.gb", std::ios_base::binary);
	//std::ifstream romfile("mealybug/m3_scy_change.gb", std::ios_base::binary);
	romfile.seekg(0, std::ios::end);
	size_t romlength = romfile.tellg();
	romfile.seekg(0, std::ios::beg);
	uint8_t* cart = new uint8_t[romlength];
	romfile.read((char*)cart, romlength);

	uint8_t rom_banks = getRomBanks(cart[0x0148]);
	size_t cart_ram_size = getRamSize(cart[0x0149]);

	Mapper* mapper;
	switch (cart[0x147]) {
	case 0:
		mapper = new Passthrough(cart);
		break;
	case 1:
	case 2:
	case 3:
		mapper = new MBC1(cart, rom_banks, cart_ram_size);
		break;
	default:
		throw std::logic_error{ "Unsupported mapper" };
		break;
	}

	std::shared_ptr<PPU> ppu = std::make_shared<PPU>();
	std::shared_ptr<APU> apu = std::make_shared<APU>();
	std::shared_ptr<Bus> bus = std::make_shared<Bus>(ppu, apu);
	bus->bootrom = bootrom;
	bus->cart = mapper;
	ppu->bus = bus;
	std::shared_ptr<CPU> cpu = std::make_shared<CPU>(bus, apu);
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