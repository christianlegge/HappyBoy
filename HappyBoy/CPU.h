#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include "Bus.h"

typedef struct {
	uint8_t bytes : 2;
	uint8_t cycles : 5;
	uint8_t opcode;
	uint16_t addr;
	uint8_t param8;
	uint16_t param16;
} Instruction;

enum class AddressingMode {
	RegisterA, RegisterB, RegisterC, RegisterD, RegisterE, RegisterF, RegisterH, RegisterL,
	RegisterAF, RegisterBC, RegisterDE, RegisterHL,
	AddressAF, AddressBC, AddressDE, AddressHL,
	Immediate8, Immediate16,
	Absolute8, Absolute16, AbsoluteHL
};

enum class WritebackMode {
	RegisterA, RegisterB, RegisterC, RegisterD, RegisterE, RegisterF, RegisterH, RegisterL,
	RegisterAF, RegisterBC, RegisterDE, RegisterHL,
	AddressAF, AddressBC, AddressDE, AddressHL,
	Immediate8, Immediate16,
	Absolute8, Absolute16, AbsoluteHL
};

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
	bool halted = false;
	bool stopped = false;
	uint16_t counter = 0x0000;
	uint8_t TIMA;
	uint8_t TMA;
	union {
		struct {
			uint8_t clockselect : 2;
			bool running : 1;
			uint8_t unused : 5;
		};
		uint8_t reg;
	} TAC;
private:
	union {
		struct {
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
				uint8_t reg;
			} F;
			uint8_t A;
		};
		uint16_t AF = 0x01B0;
	} AF;
	union {
		struct {
			uint8_t C;
			uint8_t B;
		};
		uint16_t BC = 0x0013;
	} BC;
	union {
		struct {
			uint8_t E;
			uint8_t D;
		};
		uint16_t DE = 0x00D8;
	} DE;
	union {
		struct {
			uint8_t L;
			uint8_t H;
		};
		uint16_t HL = 0x014D;
	} HL;

	uint16_t SP = 0xFFFE;
	uint16_t PC = 0x0000;

	bool ime = false;
	int cyclesRemaining = 0;

	std::vector<Instruction> glossary;
	std::vector<Instruction> cb_glossary;
	std::vector<void(CPU::*)()> opcode_funcs;
	std::vector<std::string> names;
	Instruction current_instruction;
	Instruction debug_instruction;


	void fetch(bool debug = false);
	void execute(Instruction& ins);
	uint8_t readBus(uint16_t addr);
	void writeBus(uint16_t addr, uint8_t data);

	template <class T, AddressingMode>
	T& getOperand();
	template <class T, WritebackMode>
	T& getWriteTarget();

	template <class T, WritebackMode>
	void writeValue(T value);

	void NOP();
	template <WritebackMode, AddressingMode>
	void LD();
	template <AddressingMode>
	void INC();
	template <AddressingMode>
	void DEC();
	void RLCA();
	template <WritebackMode, AddressingMode>
	void ADD();
	void RRCA();
	void STOP();
	void RLA();
	template <WritebackMode, AddressingMode>
	void JR();
	void RRA();
	void DAA();
	void CPL();
	void SCF();
	void CCF();
	void HALT();
	template <AddressingMode>
	void ADC();
	template <AddressingMode>
	void SUB();
	template <AddressingMode>
	void SBC();
	template <AddressingMode>
	void AND();
	template <AddressingMode>
	void XOR();
	template <AddressingMode>
	void OR();
	template <AddressingMode>
	void CP();
	template <WritebackMode, AddressingMode>
	void RET();
	template <WritebackMode, AddressingMode>
	void POP();
	template <WritebackMode, AddressingMode>
	void JP();
	template <WritebackMode, AddressingMode>
	void CALL();
	template <WritebackMode, AddressingMode>
	void PUSH();
	template <WritebackMode, AddressingMode>
	void RST();
	void RETI();
	void DI();
	void EI();

	void incMem(uint16_t addr);
	void dec(uint8_t* reg);
	void decMem(uint16_t addr);
	void sub(uint8_t* reg, int val);
	void cp(int val);
	void cb(uint8_t opcode);
	uint8_t daa();

	std::string registerString(uint8_t reg, std::string name);

	std::shared_ptr<Bus> bus;
};
