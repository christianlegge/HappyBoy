#pragma once

#include <cstdint>

class Mapper {
public:
	virtual uint8_t read(uint16_t addr) = 0;
	virtual void write(uint16_t addr, uint8_t data) = 0;
	uint8_t* rom;
};
