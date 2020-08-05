#include "Mapper.h"

class Passthrough : public Mapper {
public:
	virtual uint8_t read(uint16_t addr) override
	{
		return rom[addr];
	}
	virtual void write(uint16_t addr, uint8_t data) override
	{
		return;
	}
};