#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include "Bus.h"

typedef struct {
	std::string name;
	int bytes;
	int cycles;
	uint8_t opcode;
	uint16_t addr;
	uint8_t param8;
	uint16_t param16;
} Instruction;

class CPU
{
public:
	CPU(std::shared_ptr<Bus> bus);
	uint16_t tick();
	std::vector<std::string> getDisassembly(int lines);
	std::vector<std::string> getRegisters();
	bool instructionComplete();
	void interrupt(uint16_t addr);
	void reset();
	union {
		struct {
			bool vblank : 1;
			bool lcdc : 1;
			bool timer : 1;
			bool serial : 1;
			bool joypad : 1;
			uint8_t unused : 3;
		};
		uint8_t reg = 0x00;
	} IF;
	union {
		struct {
			bool vblank : 1;
			bool lcdc : 1;
			bool timer : 1;
			bool serial : 1;
			bool joypad : 1;
			uint8_t unused : 3;
		};
		uint8_t reg = 0x00;
	} IE;
	bool stop = false;
private:
	union {
		struct {
			bool U0 : 1;
			bool U1 : 1;
			bool U2 : 1;
			bool U3 : 1;
			bool C : 1;
			bool H : 1;
			bool N : 1;
			bool Z : 1;
		};
		uint8_t reg = 0xB0;
	} flags;

	uint8_t a = 0x01;
	uint8_t b = 0x00;
	uint8_t c = 0x13;
	uint8_t d = 0x00;
	uint8_t e = 0xD8;
	uint8_t h = 0x01;
	uint8_t l = 0x4D;
	uint16_t sp = 0xFFFE;
	uint16_t pc = 0x0000;

	bool ime = false;
	bool shouldResetIme = false;
	uint16_t currentInterrupt = 0x0000;
	int cyclesRemaining = 0;

	std::vector<Instruction> glossary;
	std::vector<Instruction> cb_glossary;


	Instruction fetch(bool setCycles = true);
	void execute(Instruction ins);
	uint8_t read(uint16_t addr);
	void write(uint16_t addr, uint8_t data);

	void inc(uint8_t* reg);
	void incMem(uint16_t addr);
	void dec(uint8_t* reg);
	void decMem(uint16_t addr);
	void add(uint8_t* reg, int val);
	void sub(uint8_t* reg, int val);
	void cp(int val);
	void cb(uint8_t opcode);
	uint8_t daa();

	std::string registerString(uint8_t reg, std::string name);

	std::shared_ptr<Bus> bus;
};

