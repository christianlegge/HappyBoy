#include "Mapper.h"

class MBC1 : public Mapper {
public:
	MBC1(uint8_t* cart, uint8_t rom_banks, size_t ram_size) {
		this->rom = cart;
		this->bankmask = (rom_banks - 1) & 0x1F;
		this->large_rom = rom_banks > 32;
		this->ram = new uint8_t[ram_size];
		this->ram_size = ram_size;
	}

	~MBC1() {
		delete this->ram;
	}

	virtual uint8_t read(uint16_t addr) override
	{
		if (0x0000 <= addr && addr < 0x4000) {
			if (large_rom && bankmode) {
				return rom[((secondary_bank << 5) << 14) | addr];
			}
			else {
				return rom[addr];
			}
		}
		else if (0x4000 <= addr && addr < 0x8000) {
			if (large_rom) {
				return rom[(((secondary_bank << 5) | rombank) << 14) | (addr & 0x3FFF)];
			}
			else {
				return rom[(rombank << 14) | (addr & 0x3FFF)];
			}
		}
		else if (0xA000 <= addr && addr < 0xC000) {
			if (ram_size == 0) {
				return 0xFF;
			}
			else if (ram_size <= 0x2000) {
				return ram[(addr & 0x1FFF) % ram_size];
			}
			else if (bankmode) {
				return ram[(secondary_bank << 13) | (addr & 0x1FFF)];
			}
			else {
				return ram[addr & 0x1FFF];
			}
		}
		else {
			return 0xFF;
		}
	}
	virtual void write(uint16_t addr, uint8_t data) override
	{
		if (0x0000 <= addr && addr < 0x2000) {
			ramenable = (data & 0x0F) == 0x0A;
		}
		else if (0x2000 <= addr && addr < 0x4000) {
			rombank = data & bankmask;
			if (rombank == 0x00) {
				rombank = 0x01;
			}
		}
		else if (0x4000 <= addr && addr < 0x6000) {
			secondary_bank = data & 0x03;
		}
		else if (0x6000 <= addr && addr < 0x8000) {
			if (data <= 1) {
				bankmode = data;
			}
		}
		else if (0xA000 <= addr && addr < 0xC000) {
			if (ram_size == 0) {
				return;
			}
			else if (ram_size <= 0x2000) {
				ram[(addr & 0x1FFF) % ram_size] = data;
			}
			else if (bankmode) {
				ram[(secondary_bank << 13) | (addr & 0x1FFF)] = data;
			}
			else {
				ram[addr & 0x1FFF] = data;
			}
		}
	}

private:
	uint8_t rombank = 0x01;
	uint8_t secondary_bank = 0x00;
	uint8_t bankmask;
	bool large_rom;
	bool ramenable = false;
	bool bankmode = false;
	uint8_t *ram;
	size_t ram_size;
};