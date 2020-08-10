#include "Mapper.h"

class Passthrough : public Mapper {
public:
	Passthrough(uint8_t* cart) {
		this->rom = cart;
	}

	virtual uint8_t read(uint16_t addr) override
	{
		if (0x0000 <= addr && addr < 0x8000) {
			return rom[addr];
		}
		else {
			return 0xFF;
		}
	}

	virtual void write(uint16_t addr, uint8_t data) override
	{
		return;
	}
};