#include "Mapper.h"

class MBC1 : public Mapper {
public:
	virtual uint8_t read(uint16_t addr) override
	{
		if (0x0000 <= addr && addr < 0x4000) {
			return rom[addr];
		}
		else if (0x4000 <= addr && addr < 0x8000) {
			return rom[(rombank << 14) | (addr & 0x3FFF)];
		}
	}
	virtual void write(uint16_t addr, uint8_t data) override
	{
		if (0x0000 <= addr && addr < 0x2000) {
			ramenable = (data & 0x0F) == 0x0A;
		}
		else if (0x2000 <= addr && addr < 0x4000) {
			rombank = data & 0x1F;
			if (rombank == 0x00) {
				rombank = 0x01;
			}
		}
		return;
	}

private:
	uint8_t rombank = 0x01;
	bool ramenable = false;
};