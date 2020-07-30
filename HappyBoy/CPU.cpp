#include <iomanip>
#include <iostream>
#include <sstream>

#include "CPU.h"

uint8_t CPU::read(uint16_t addr)
{
	return bus->read(addr);
}

void CPU::write(uint16_t addr, uint8_t data)
{
	bus->write(addr, data);
}

CPU::CPU(std::shared_ptr<Bus> bus) : bus(bus)
{
	glossary = {
		{ "NOP", 1, 4 }, { "LD BC,d16", 3, 12 }, { "LD (BC),A", 1, 8 }, { "INC BC", 1, 8 }, { "INC B", 1, 4 }, { "DEC B", 1, 4 }, { "LD B,d8", 2, 8 }, { "RLCA", 1, 4 }, { "LD (a16),SP", 3, 20 }, { "ADD HL,BC", 1, 8 }, { "LD A,(BC)", 1, 8 }, { "DEC BC", 1, 8 }, { "INC C", 1, 4 }, { "DEC C", 1, 4 }, { "LD C,d8", 2, 8 }, { "RRCA", 1, 4 },
		{ "STOP 0", 2, 4 }, { "LD DE,d16", 3, 12 }, { "LD (DE),A", 1, 8 }, { "INC DE", 1, 8 }, { "INC D", 1, 4 }, { "DEC D", 1, 4 }, { "LD D,d8", 2, 8 }, { "RLA", 1, 4 }, { "JR r8", 2, 12 }, { "ADD HL,DE", 1, 8 }, { "LD A,(DE)", 1, 8 }, { "DEC DE", 1, 8 }, { "INC E", 1, 4 }, { "DEC E", 1, 4 }, { "LD E,d8", 2, 8 }, { "RRA", 1, 4 },
		{ "JR NZ,r8", 2, 8 }, { "LD HL,d16", 3, 12 }, { "LD (HL+),A", 1, 8 }, { "INC HL", 1, 8 }, { "INC H", 1, 4 }, { "DEC H", 1, 4 }, { "LD H,d8", 2, 8 }, { "DAA", 1, 4 }, { "JR Z,r8", 2, 8 }, { "ADD HL,HL", 1, 8 }, { "LD A,(HL+)", 1, 8 }, { "DEC HL", 1, 8 }, { "INC L", 1, 4 }, { "DEC L", 1, 4 }, { "LD L,d8", 2, 8 }, { "CPL", 1, 4 },
		{ "JR NC,r8", 2, 8 }, { "LD SP,d16", 3, 12 }, { "LD (HL-),A", 1, 8 }, { "INC SP", 1, 8 }, { "INC (HL)", 1, 12 }, { "DEC (HL)", 1, 12 }, { "LD (HL),d8", 2, 12 }, { "SCF", 1, 4 }, { "JR C,r8", 2, 8 }, { "ADD HL,SP", 1, 8 }, { "LD A,(HL-)", 1, 8 }, { "DEC SP", 1, 8 }, { "INC A", 1, 4 }, { "DEC A", 1, 4 }, { "LD A,d8", 2, 8 }, { "CCF", 1, 4 },
		{ "LD B,B", 1, 4 }, { "LD B,C", 1, 4 }, { "LD B,D", 1, 4 }, { "LD B,E", 1, 4 }, { "LD B,H", 1, 4 }, { "LD B,L", 1, 4 }, { "LD B,(HL)", 1, 8 }, { "LD B,A", 1, 4 }, { "LD C,B", 1, 4 }, { "LD C,C", 1, 4 }, { "LD C,D", 1, 4 }, { "LD C,E", 1, 4 }, { "LD C,H", 1, 4 }, { "LD C,L", 1, 4 }, { "LD C,(HL)", 1, 8 }, { "LD C,A", 1, 4 },
		{ "LD D,B", 1, 4 }, { "LD D,C", 1, 4 }, { "LD D,D", 1, 4 }, { "LD D,E", 1, 4 }, { "LD D,H", 1, 4 }, { "LD D,L", 1, 4 }, { "LD D,(HL)", 1, 8 }, { "LD D,A", 1, 4 }, { "LD E,B", 1, 4 }, { "LD E,C", 1, 4 }, { "LD E,D", 1, 4 }, { "LD E,E", 1, 4 }, { "LD E,H", 1, 4 }, { "LD E,L", 1, 4 }, { "LD E,(HL)", 1, 8 }, { "LD E,A", 1, 4 },
		{ "LD H,B", 1, 4 }, { "LD H,C", 1, 4 }, { "LD H,D", 1, 4 }, { "LD H,E", 1, 4 }, { "LD H,H", 1, 4 }, { "LD H,L", 1, 4 }, { "LD H,(HL)", 1, 8 }, { "LD H,A", 1, 4 }, { "LD L,B", 1, 4 }, { "LD L,C", 1, 4 }, { "LD L,D", 1, 4 }, { "LD L,E", 1, 4 }, { "LD L,H", 1, 4 }, { "LD L,L", 1, 4 }, { "LD L,(HL)", 1, 8 }, { "LD L,A", 1, 4 },
		{ "LD (HL),B", 1, 8 }, { "LD (HL),C", 1, 8 }, { "LD (HL),D", 1, 8 }, { "LD (HL),E", 1, 8 }, { "LD (HL),H", 1, 8 }, { "LD (HL),L", 1, 8 }, { "HALT", 1, 4 }, { "LD (HL),A", 1, 8 }, { "LD A,B", 1, 4 }, { "LD A,C", 1, 4 }, { "LD A,D", 1, 4 }, { "LD A,E", 1, 4 }, { "LD A,H", 1, 4 }, { "LD A,L", 1, 4 }, { "LD A,(HL)", 1, 8 }, { "LD A,A", 1, 4 },
		{ "ADD A,B", 1, 4 }, { "ADD A,C", 1, 4 }, { "ADD A,D", 1, 4 }, { "ADD A,E", 1, 4 }, { "ADD A,H", 1, 4 }, { "ADD A,L", 1, 4 }, { "ADD A,(HL)", 1, 8 }, { "ADD A,A", 1, 4 }, { "ADC A,B", 1, 4 }, { "ADC A,C", 1, 4 }, { "ADC A,D", 1, 4 }, { "ADC A,E", 1, 4 }, { "ADC A,H", 1, 4 }, { "ADC A,L", 1, 4 }, { "ADC A,(HL)", 1, 8 }, { "ADC A,A", 1, 4 },
		{ "SUB B", 1, 4 }, { "SUB C", 1, 4 }, { "SUB D", 1, 4 }, { "SUB E", 1, 4 }, { "SUB H", 1, 4 }, { "SUB L", 1, 4 }, { "SUB (HL)", 1, 8 }, { "SUB A", 1, 4 }, { "SBC A,B", 1, 4 }, { "SBC A,C", 1, 4 }, { "SBC A,D", 1, 4 }, { "SBC A,E", 1, 4 }, { "SBC A,H", 1, 4 }, { "SBC A,L", 1, 4 }, { "SBC A,(HL)", 1, 8 }, { "SBC A,A", 1, 4 },
		{ "AND B", 1, 4 }, { "AND C", 1, 4 }, { "AND D", 1, 4 }, { "AND E", 1, 4 }, { "AND H", 1, 4 }, { "AND L", 1, 4 }, { "AND (HL)", 1, 8 }, { "AND A", 1, 4 }, { "XOR B", 1, 4 }, { "XOR C", 1, 4 }, { "XOR D", 1, 4 }, { "XOR E", 1, 4 }, { "XOR H", 1, 4 }, { "XOR L", 1, 4 }, { "XOR (HL)", 1, 8 }, { "XOR A", 1, 4 },
		{ "OR B", 1, 4 }, { "OR C", 1, 4 }, { "OR D", 1, 4 }, { "OR E", 1, 4 }, { "OR H", 1, 4 }, { "OR L", 1, 4 }, { "OR (HL)", 1, 8 }, { "OR A", 1, 4 }, { "CP B", 1, 4 }, { "CP C", 1, 4 }, { "CP D", 1, 4 }, { "CP E", 1, 4 }, { "CP H", 1, 4 }, { "CP L", 1, 4 }, { "CP (HL)", 1, 8 }, { "CP A", 1, 4 },
		{ "RET NZ", 1, 8 }, { "POP BC", 1, 12 }, { "JP NZ,a16", 3, 12 }, { "JP a16", 3, 16 }, { "CALL NZ,a16", 3, 12 }, { "PUSH BC", 1, 16 }, { "ADD A,d8", 2, 8 }, { "RST 00H", 1, 16 }, { "RET Z", 1, 8 }, { "RET", 1, 16 }, { "JP Z,a16", 3, 12 }, { "PREFIX CB", 1, 4 }, { "CALL Z,a16", 3, 12 }, { "CALL a16", 3, 24 }, { "ADC A,d8", 2, 8 }, { "RST 08H", 1, 16 },
		{ "RET NC", 1, 8 }, { "POP DE", 1, 12 }, { "JP NC,a16", 3, 12 }, { "???", 1, 1 }, { "CALL NC,a16", 3, 12 }, { "PUSH DE", 1, 16 }, { "SUB d8", 2, 8 }, { "RST 10H", 1, 16 }, { "RET C", 1, 8 }, { "RETI", 1, 16 }, { "JP C,a16", 3, 12 }, { "???", 1, 1 }, { "CALL C,a16", 3, 12 }, { "???", 1, 1 }, { "SBC A,d8", 2, 8 }, { "RST 18H", 1, 16 },
		{ "LDH (a8),A", 2, 12 }, { "POP HL", 1, 12 }, { "LD (C),A", 1, 8 }, { "???", 1, 1 }, { "???", 1, 1 }, { "PUSH HL", 1, 16 }, { "AND d8", 2, 8 }, { "RST 20H", 1, 16 }, { "ADD SP,r8", 2, 16 }, { "JP (HL)", 1, 4 }, { "LD (a16),A", 3, 16 }, { "???", 1, 1 }, { "???", 1, 1 }, { "???", 1, 1 }, { "XOR d8", 2, 8 }, { "RST 28H", 1, 16 },
		{ "LDH A,(a8)", 2, 12 }, { "POP AF", 1, 12 }, { "LD A,(C)", 1, 8 }, { "DI", 1, 4 }, { "???", 1, 1 }, { "PUSH AF", 1, 16 }, { "OR d8", 2, 8 }, { "RST 30H", 1, 16 }, { "LD HL,SP+r8", 2, 12 }, { "LD SP,HL", 1, 8 }, { "LD A,(a16)", 3, 16 }, { "EI", 1, 4 }, { "???", 1, 1 }, { "???", 1, 1 }, { "CP d8", 2, 8 }, { "RST 38H", 1, 16 },
	};

	cb_glossary = {
		{ "RLC B", 2, 8 }, { "RLC C", 2, 8 }, { "RLC D", 2, 8 }, { "RLC E", 2, 8 }, { "RLC H", 2, 8 }, { "RLC L", 2, 8 }, { "RLC (HL)", 2, 16 }, { "RLC A", 2, 8 }, { "RRC B", 2, 8 }, { "RRC C", 2, 8 }, { "RRC D", 2, 8 }, { "RRC E", 2, 8 }, { "RRC H", 2, 8 }, { "RRC L", 2, 8 }, { "RRC (HL)", 2, 16 }, { "RRC A", 2, 8 },
		{ "RL B", 2, 8 }, { "RL C", 2, 8 }, { "RL D", 2, 8 }, { "RL E", 2, 8 }, { "RL H", 2, 8 }, { "RL L", 2, 8 }, { "RL (HL)", 2, 16 }, { "RL A", 2, 8 }, { "RR B", 2, 8 }, { "RR C", 2, 8 }, { "RR D", 2, 8 }, { "RR E", 2, 8 }, { "RR H", 2, 8 }, { "RR L", 2, 8 }, { "RR (HL)", 2, 16 }, { "RR A", 2, 8 },
		{ "SLA B", 2, 8 }, { "SLA C", 2, 8 }, { "SLA D", 2, 8 }, { "SLA E", 2, 8 }, { "SLA H", 2, 8 }, { "SLA L", 2, 8 }, { "SLA (HL)", 2, 16 }, { "SLA A", 2, 8 }, { "SRA B", 2, 8 }, { "SRA C", 2, 8 }, { "SRA D", 2, 8 }, { "SRA E", 2, 8 }, { "SRA H", 2, 8 }, { "SRA L", 2, 8 }, { "SRA (HL)", 2, 16 }, { "SRA A", 2, 8 },
		{ "SWAP B", 2, 8 }, { "SWAP C", 2, 8 }, { "SWAP D", 2, 8 }, { "SWAP E", 2, 8 }, { "SWAP H", 2, 8 }, { "SWAP L", 2, 8 }, { "SWAP (HL)", 2, 16 }, { "SWAP A", 2, 8 }, { "SRL B", 2, 8 }, { "SRL C", 2, 8 }, { "SRL D", 2, 8 }, { "SRL E", 2, 8 }, { "SRL H", 2, 8 }, { "SRL L", 2, 8 }, { "SRL (HL)", 2, 16 }, { "SRL A", 2, 8 },
		{ "BIT 0,B", 2, 8 }, { "BIT 0,C", 2, 8 }, { "BIT 0,D", 2, 8 }, { "BIT 0,E", 2, 8 }, { "BIT 0,H", 2, 8 }, { "BIT 0,L", 2, 8 }, { "BIT 0,(HL)", 2, 16 }, { "BIT 0,A", 2, 8 }, { "BIT 1,B", 2, 8 }, { "BIT 1,C", 2, 8 }, { "BIT 1,D", 2, 8 }, { "BIT 1,E", 2, 8 }, { "BIT 1,H", 2, 8 }, { "BIT 1,L", 2, 8 }, { "BIT 1,(HL)", 2, 16 }, { "BIT 1,A", 2, 8 },
		{ "BIT 2,B", 2, 8 }, { "BIT 2,C", 2, 8 }, { "BIT 2,D", 2, 8 }, { "BIT 2,E", 2, 8 }, { "BIT 2,H", 2, 8 }, { "BIT 2,L", 2, 8 }, { "BIT 2,(HL)", 2, 16 }, { "BIT 2,A", 2, 8 }, { "BIT 3,B", 2, 8 }, { "BIT 3,C", 2, 8 }, { "BIT 3,D", 2, 8 }, { "BIT 3,E", 2, 8 }, { "BIT 3,H", 2, 8 }, { "BIT 3,L", 2, 8 }, { "BIT 3,(HL)", 2, 16 }, { "BIT 3,A", 2, 8 },
		{ "BIT 4,B", 2, 8 }, { "BIT 4,C", 2, 8 }, { "BIT 4,D", 2, 8 }, { "BIT 4,E", 2, 8 }, { "BIT 4,H", 2, 8 }, { "BIT 4,L", 2, 8 }, { "BIT 4,(HL)", 2, 16 }, { "BIT 4,A", 2, 8 }, { "BIT 5,B", 2, 8 }, { "BIT 5,C", 2, 8 }, { "BIT 5,D", 2, 8 }, { "BIT 5,E", 2, 8 }, { "BIT 5,H", 2, 8 }, { "BIT 5,L", 2, 8 }, { "BIT 5,(HL)", 2, 16 }, { "BIT 5,A", 2, 8 },
		{ "BIT 6,B", 2, 8 }, { "BIT 6,C", 2, 8 }, { "BIT 6,D", 2, 8 }, { "BIT 6,E", 2, 8 }, { "BIT 6,H", 2, 8 }, { "BIT 6,L", 2, 8 }, { "BIT 6,(HL)", 2, 16 }, { "BIT 6,A", 2, 8 }, { "BIT 7,B", 2, 8 }, { "BIT 7,C", 2, 8 }, { "BIT 7,D", 2, 8 }, { "BIT 7,E", 2, 8 }, { "BIT 7,H", 2, 8 }, { "BIT 7,L", 2, 8 }, { "BIT 7,(HL)", 2, 16 }, { "BIT 7,A", 2, 8 },
		{ "RES 0,B", 2, 8 }, { "RES 0,C", 2, 8 }, { "RES 0,D", 2, 8 }, { "RES 0,E", 2, 8 }, { "RES 0,H", 2, 8 }, { "RES 0,L", 2, 8 }, { "RES 0,(HL)", 2, 16 }, { "RES 0,A", 2, 8 }, { "RES 1,B", 2, 8 }, { "RES 1,C", 2, 8 }, { "RES 1,D", 2, 8 }, { "RES 1,E", 2, 8 }, { "RES 1,H", 2, 8 }, { "RES 1,L", 2, 8 }, { "RES 1,(HL)", 2, 16 }, { "RES 1,A", 2, 8 },
		{ "RES 2,B", 2, 8 }, { "RES 2,C", 2, 8 }, { "RES 2,D", 2, 8 }, { "RES 2,E", 2, 8 }, { "RES 2,H", 2, 8 }, { "RES 2,L", 2, 8 }, { "RES 2,(HL)", 2, 16 }, { "RES 2,A", 2, 8 }, { "RES 3,B", 2, 8 }, { "RES 3,C", 2, 8 }, { "RES 3,D", 2, 8 }, { "RES 3,E", 2, 8 }, { "RES 3,H", 2, 8 }, { "RES 3,L", 2, 8 }, { "RES 3,(HL)", 2, 16 }, { "RES 3,A", 2, 8 },
		{ "RES 4,B", 2, 8 }, { "RES 4,C", 2, 8 }, { "RES 4,D", 2, 8 }, { "RES 4,E", 2, 8 }, { "RES 4,H", 2, 8 }, { "RES 4,L", 2, 8 }, { "RES 4,(HL)", 2, 16 }, { "RES 4,A", 2, 8 }, { "RES 5,B", 2, 8 }, { "RES 5,C", 2, 8 }, { "RES 5,D", 2, 8 }, { "RES 5,E", 2, 8 }, { "RES 5,H", 2, 8 }, { "RES 5,L", 2, 8 }, { "RES 5,(HL)", 2, 16 }, { "RES 5,A", 2, 8 },
		{ "RES 6,B", 2, 8 }, { "RES 6,C", 2, 8 }, { "RES 6,D", 2, 8 }, { "RES 6,E", 2, 8 }, { "RES 6,H", 2, 8 }, { "RES 6,L", 2, 8 }, { "RES 6,(HL)", 2, 16 }, { "RES 6,A", 2, 8 }, { "RES 7,B", 2, 8 }, { "RES 7,C", 2, 8 }, { "RES 7,D", 2, 8 }, { "RES 7,E", 2, 8 }, { "RES 7,H", 2, 8 }, { "RES 7,L", 2, 8 }, { "RES 7,(HL)", 2, 16 }, { "RES 7,A", 2, 8 },
		{ "SET 0,B", 2, 8 }, { "SET 0,C", 2, 8 }, { "SET 0,D", 2, 8 }, { "SET 0,E", 2, 8 }, { "SET 0,H", 2, 8 }, { "SET 0,L", 2, 8 }, { "SET 0,(HL)", 2, 16 }, { "SET 0,A", 2, 8 }, { "SET 1,B", 2, 8 }, { "SET 1,C", 2, 8 }, { "SET 1,D", 2, 8 }, { "SET 1,E", 2, 8 }, { "SET 1,H", 2, 8 }, { "SET 1,L", 2, 8 }, { "SET 1,(HL)", 2, 16 }, { "SET 1,A", 2, 8 },
		{ "SET 2,B", 2, 8 }, { "SET 2,C", 2, 8 }, { "SET 2,D", 2, 8 }, { "SET 2,E", 2, 8 }, { "SET 2,H", 2, 8 }, { "SET 2,L", 2, 8 }, { "SET 2,(HL)", 2, 16 }, { "SET 2,A", 2, 8 }, { "SET 3,B", 2, 8 }, { "SET 3,C", 2, 8 }, { "SET 3,D", 2, 8 }, { "SET 3,E", 2, 8 }, { "SET 3,H", 2, 8 }, { "SET 3,L", 2, 8 }, { "SET 3,(HL)", 2, 16 }, { "SET 3,A", 2, 8 },
		{ "SET 4,B", 2, 8 }, { "SET 4,C", 2, 8 }, { "SET 4,D", 2, 8 }, { "SET 4,E", 2, 8 }, { "SET 4,H", 2, 8 }, { "SET 4,L", 2, 8 }, { "SET 4,(HL)", 2, 16 }, { "SET 4,A", 2, 8 }, { "SET 5,B", 2, 8 }, { "SET 5,C", 2, 8 }, { "SET 5,D", 2, 8 }, { "SET 5,E", 2, 8 }, { "SET 5,H", 2, 8 }, { "SET 5,L", 2, 8 }, { "SET 5,(HL)", 2, 16 }, { "SET 5,A", 2, 8 },
		{ "SET 6,B", 2, 8 }, { "SET 6,C", 2, 8 }, { "SET 6,D", 2, 8 }, { "SET 6,E", 2, 8 }, { "SET 6,H", 2, 8 }, { "SET 6,L", 2, 8 }, { "SET 6,(HL)", 2, 16 }, { "SET 6,A", 2, 8 }, { "SET 7,B", 2, 8 }, { "SET 7,C", 2, 8 }, { "SET 7,D", 2, 8 }, { "SET 7,E", 2, 8 }, { "SET 7,H", 2, 8 }, { "SET 7,L", 2, 8 }, { "SET 7,(HL)", 2, 16 }, { "SET 7,A", 2, 8 },
	};
}

uint16_t CPU::tick() {

	if (ime) {
		uint16_t addr = 0;
		if (IF.vblank && IE.vblank) {
			addr = 0x0040;
		}
		else if (IF.lcdc && IE.lcdc) {
			addr = 0x0048;
		}
		else if (IF.timer && IE.timer) {
			addr = 0x0050;
		}
		else if (IF.serial && IE.serial) {
			addr = 0x0058;
		}
		else if (IF.joypad && IE.joypad) {
			addr = 0x0060;
		}

		if (addr) {
			ime = false;
			IF.reg = 0;
			write(--sp, pc >> 8);
			write(--sp, pc & 0b11111111);
			pc = addr;
		}
	}

	if (cyclesRemaining <= 0) {
		Instruction ins = fetch();
		execute(ins);
	}

	cyclesRemaining--;
	counter++;

	if (TAC.running) {
		uint8_t bit;
		switch (TAC.clockselect) {
		case 0:
			bit = 9;
			break;
		case 1:
			bit = 3;
			break;
		case 2:
			bit = 5;
			break;
		case 3:
			bit = 7;
			break;
		}
		bool tmp = counter & (1 << bit);
		counter++;
		if (tmp != (counter & (1 << bit))) {
			TIMA++;
		}
		if (TIMA == 0) {
			IF.timer = true;
			TIMA = TMA;
		}
	}
	else {
		counter++;
	}

	return pc;
}

std::vector<std::string> CPU::getDisassembly(int lines)
{
	std::vector<std::string> disasm;
	uint16_t pc_tmp = pc;
	int cycles_tmp = cyclesRemaining;
	for (int i = 0; i < lines; i++) {
		Instruction ins = fetch(false);
		size_t d8pos = ins.name.find("d8");
		size_t r8pos = ins.name.find("r8");
		size_t a8pos = ins.name.find("a8");
		size_t a16pos = ins.name.find("a16");
		size_t d16pos = ins.name.find("d16");
		if (d8pos != std::string::npos) {
			std::stringstream s;
			s << "$" << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)ins.param8;
			ins.name.replace(d8pos, 2, s.str());
		}
		if (r8pos != std::string::npos) {
			std::stringstream s;
			s << "$" << std::setfill('0') << std::setw(4) << std::hex << (int8_t)ins.param8 + pc;
			ins.name.replace(r8pos, 2, s.str());
		}
		if (a8pos != std::string::npos) {
			std::stringstream s;
			s << "$" << std::setfill('0') << std::setw(4) << std::hex << (0xFF00 + ins.param8);
			ins.name.replace(a8pos, 2, s.str());
		}
		if (a16pos != std::string::npos) {
			std::stringstream s;
			s << "$" << std::setfill('0') << std::setw(4) << std::hex << ins.param16;
			ins.name.replace(a16pos, 3, s.str());
		}
		if (d16pos != std::string::npos) {
			std::stringstream s;
			s << "$" << std::setfill('0') << std::setw(4) << std::hex << ins.param16;
			ins.name.replace(d16pos, 3, s.str());
		}

		std::stringstream s;
		s << "$" << std::setfill('0') << std::setw(4) << std::hex << ins.addr << ": " << ins.name;
		disasm.push_back(s.str());
	}
	pc = pc_tmp;
	cyclesRemaining = cycles_tmp;
	return disasm;
}

std::string CPU::registerString(uint8_t reg, std::string name) {
	std::stringstream s;
	s << name << ": ";
	for (int i = 7; i >= 0; i--) {
		s << ((reg & (1 << i)) ? 1 : 0);
	}
	s << " ($" << std::setfill('0') << std::setw(2) << std::hex << (int)reg << ") ";
	return s.str();
}

std::vector<std::string> CPU::getRegisters()
{
	std::vector<std::string> registers;
	std::stringstream s;
	registers.push_back(registerString(b, "b") + registerString(c, "c"));
	registers.push_back(registerString(d, "d") + registerString(e, "e"));
	registers.push_back(registerString(h, "h") + registerString(l, "l"));
	registers.push_back(registerString(a, "a") + registerString(flags.reg, "f"));
	s << "sp: $" << std::setfill('0') << std::setw(4) << std::hex << sp << "        pc: $" << std::setfill('0') << std::setw(4) << std::hex << pc;
	registers.push_back(s.str());
	return registers;
}

bool CPU::instructionComplete()
{
	return cyclesRemaining == 0;
}

void CPU::interrupt(uint16_t addr)
{
	if (addr == 0x0040) {
		IF.vblank = true;
	}
	else if (addr == 0x0048) {
		IF.lcdc = true;
	}
	else if (addr == 0x0050) {
		IF.timer = true;
	}
	else if (addr == 0x0058) {
		IF.serial = true;
	}
	else if (addr == 0x0060) {
		IF.joypad = true;
	}
}

void CPU::reset()
{
	stop = false;
	pc = 0x0000;
}

Instruction CPU::fetch(bool setCycles) {
	uint16_t addr = pc;
	uint8_t opcode = read(pc++);
	Instruction i = glossary[opcode];
	if (opcode == 0xCB) {
		uint8_t cb_opcode = read(pc++);
		i = cb_glossary[cb_opcode];
		i.param8 = cb_opcode;
	}
	else {
		if (i.bytes == 2) {
			i.param8 = read(pc++);
		}
		else if (i.bytes == 3) {
			i.param16 = read(pc++) + (read(pc++) << 8);
		}
	}
	i.opcode = opcode;
	i.addr = addr;
	if (setCycles) {
		cyclesRemaining = i.cycles - 1;
	}
	return i;
}

void CPU::execute(Instruction ins)
{
	int tmp;
	switch (ins.opcode)
	{
	case 0:
		break;
	case 1:
		c = ins.param16 & 0xFF;
		b = ins.param16 >> 8;
		break;
	case 2:
		write((b << 8) + c, a);
		break;
	case 3:
		c++;
		if (c == 0)
		{
			b++;
		}
		break;
	case 4:
		inc(&b);
		break;
	case 5:
		dec(&b);
		break;
	case 6:
		b = ins.param8;
		break;
	case 7:
		flags.Z = 0;
		flags.N = 0;
		flags.H = 0;
		flags.C = a & 0b10000000;
		a = a << 1;
		a = a | flags.C;
		break;
	case 8:
		write(ins.param16, (sp >> 8));
		write(ins.param16 + 1, sp);
		break;
	case 9:
	{
		tmp = h & 0b00001000;
		flags.N = 0;
		uint16_t hl = (h << 8) | l;
		uint16_t bc = (b << 8) | c;
		uint16_t res = hl + bc;
		h = (res & 0xFF00) >> 8;
		l = res & 0xFF;
		if (hl + bc > 0xFFFF)
		{
			flags.C = 1;
		}
		else
		{
			flags.C = 0;
		}
		if ((hl & 0x0FFF) + bc > 0x0FFF)
		{
			flags.H = 1;
		}
		else
		{
			flags.H = 0;
		}
	}
	break;
	case 10:
		a = read((b << 8) + c);
		break;
	case 11:
		c--;
		if (c == 255)
		{
			b--;
		}
		break;
	case 12:
		inc(&c);
		break;
	case 13:
		dec(&c);
		break;
	case 14:
		c = ins.param8;
		break;
	case 15:
		flags.Z = 0;
		flags.N = 0;
		flags.H = 0;
		flags.C = a & 0b00000001;
		a = a >> 1;
		a = a | (flags.C << 7);
		break;
	case 16:
		stop = true;
		break;
	case 17:
		e = ins.param16 & 0xFF;
		d = (ins.param16 >> 8) & 0xFF;
		break;
	case 18:
		write((d << 8) + e, a);
		break;
	case 19:
		e++;
		if (e == 0)
		{
			d++;
		}
		break;
	case 20:
		inc(&d);
		break;
	case 21:
		dec(&d);
		break;
	case 22:
		d = ins.param8;
		break;
	case 23:
		tmp = flags.C;
		flags.Z = 0;
		flags.N = 0;
		flags.H = 0;
		flags.C = a & 0b10000000;
		a = a << 1;
		a = a | tmp;
		break;
	case 24:
		pc += (int8_t)ins.param8;
		break;
	case 25:
	{
		tmp = h & 0b00001000;
		flags.N = 0;
		uint16_t hl = (h << 8) | l;
		uint16_t de = (b << 8) | c;
		uint16_t res = hl + de;
		h = (res & 0xFF00) >> 8;
		l = res & 0xFF;
		if (hl + de > 0xFFFF)
		{
			flags.C = 1;
		}
		else
		{
			flags.C = 0;
		}
		if ((hl & 0x0FFF) + de > 0x0FFF)
		{
			flags.H = 1;
		}
		else
		{
			flags.H = 0;
		}
	}
	break;
	case 26:
		a = read((d << 8) + e);
		break;
	case 27:
		e--;
		if (e == -1)
		{
			e = 255;
			d--;
			if (d == -1)
			{
				d = 255;
			}
		}
		break;
	case 28:
		inc(&e);
		break;
	case 29:
		dec(&e);
		break;
	case 30:
		e = ins.param8;
		break;
	case 31:
		tmp = flags.C;
		flags.Z = 0;
		flags.N = 0;
		flags.H = 0;
		flags.C = a & 0b00000001;
		a = a >> 1;
		a = a | (tmp << 7);
		break;
	case 32:
		if (flags.Z == 0)
		{
			pc += (int8_t)ins.param8;
			ins.cycles += 4;
		}
		break;
	case 33:
		l = ins.param16 & 0xFF;
		h = (ins.param16 >> 8) & 0xFF;
		break;
	case 34:
		write((h << 8) + l, a);
		l++;
		if (l == 0)
		{
			h++;
		}
		break;
	case 35:
		l++;
		if (l == 0)
		{
			h++;
		}
		break;
	case 36:
		inc(&h);
		break;
	case 37:
		dec(&h);
		break;
	case 38:
		h = ins.param8;
		break;
	case 39:
		a += daa();
		flags.Z = (a == 0);
		break;
	case 40:
		if (flags.Z == 1)
		{
			pc += (int8_t)ins.param8;
			ins.cycles += 4;
		}
		break;
	case 41:
	{
		tmp = h & 0b00001000;
		flags.N = 0;
		uint16_t hl = (h << 8) | l;
		uint16_t res = hl + hl;
		h = (res & 0xFF00) >> 8;
		l = res & 0xFF;
		if (hl + hl > 0xFFFF)
		{
			flags.C = 1;
		}
		else
		{
			flags.C = 0;
		}
		if ((hl & 0x0FFF) + hl > 0x0FFF)
		{
			flags.H = 1;
		}
		else
		{
			flags.H = 0;
		}
	}
		break;
	case 42:
		a = read((h << 8) + l);
		l++;
		if (l == 0)
		{
			h++;
		}
		break;
	case 43:
		l--;
		if (l == 255)
		{
			h--;
		}
		break;
	case 44:
		inc(&l);
		break;
	case 45:
		dec(&l);
		break;
	case 46:
		l = ins.param8;
		break;
	case 47:
		a = a ^ a;
		flags.N = 1;
		flags.H = 1;
		break;
	case 48:
		if (flags.C == 0)
		{
			pc += (int8_t)ins.param8;
			ins.cycles += 4;
		}
		break;
	case 49:
		sp = ins.param16;
		break;
	case 50:
		write((h << 8) + l, a);
		l--;
		if (l == 255)
		{
			h--;
		}
		break;
	case 51:
		sp++;
		break;
	case 52:
		incMem((h << 8) + l);
		break;
	case 53:
		decMem((h << 8) + l);
		break;
	case 54:
		write((h << 8) + l, ins.param8);
		break;
	case 55:
		flags.N = 0;
		flags.H = 0;
		flags.C = 1;
		break;
	case 56:
		if (flags.C == 1)
		{
			pc += (int8_t)ins.param8;
			ins.cycles += 4;
		}
		break;
	case 57:
	{
		tmp = h & 0b00001000;
		flags.N = 0;
		uint16_t hl = (h << 8) | l;
		uint16_t res = hl + sp;
		h = (res & 0xFF00) >> 8;
		l = res & 0xFF;
		if (hl + sp > 0xFFFF)
		{
			flags.C = 1;
		}
		else
		{
			flags.C = 0;
		}
		if ((hl & 0x0FFF) + sp > 0x0FFF)
		{
			flags.H = 1;
		}
		else
		{
			flags.H = 0;
		}
	}
		break;
	case 58:
		a = read((h << 8) + l);
		l--;
		if (l == 255)
		{
			h--;
		}
		break;
	case 59:
		sp--;
		break;
	case 60:
		inc(&a);
		break;
	case 61:
		dec(&a);
		break;
	case 62:
		a = ins.param8;
		break;
	case 63:
		flags.N = 0;
		flags.H = 0;
		flags.C = 1 - flags.C;
		break;
	case 64:
		b = b;
		break;
	case 65:
		b = c;
		break;
	case 66:
		b = d;
		break;
	case 67:
		b = e;
		break;
	case 68:
		b = h;
		break;
	case 69:
		b = l;
		break;
	case 70:
		b = read((h << 8) + l);
		break;
	case 71:
		b = a;
		break;
	case 72:
		c = b;
		break;
	case 73:
		c = c;
		break;
	case 74:
		c = d;
		break;
	case 75:
		c = e;
		break;
	case 76:
		c = h;
		break;
	case 77:
		c = l;
		break;
	case 78:
		c = read((h << 8) + l);
		break;
	case 79:
		c = a;
		break;
	case 80:
		d = b;
		break;
	case 81:
		d = c;
		break;
	case 82:
		d = d;
		break;
	case 83:
		d = e;
		break;
	case 84:
		d = h;
		break;
	case 85:
		d = l;
		break;
	case 86:
		d = read((h << 8) + l);
		break;
	case 87:
		d = a;
		break;
	case 88:
		e = b;
		break;
	case 89:
		e = c;
		break;
	case 90:
		e = d;
		break;
	case 91:
		e = e;
		break;
	case 92:
		e = h;
		break;
	case 93:
		e = l;
		break;
	case 94:
		e = read((h << 8) + l);
		break;
	case 95:
		e = a;
		break;
	case 96:
		h = b;
		break;
	case 97:
		h = c;
		break;
	case 98:
		h = d;
		break;
	case 99:
		h = e;
		break;
	case 100:
		h = h;
		break;
	case 101:
		h = l;
		break;
	case 102:
		h = read((h << 8) + l);
		break;
	case 103:
		h = a;
		break;
	case 104:
		l = b;
		break;
	case 105:
		l = c;
		break;
	case 106:
		l = d;
		break;
	case 107:
		l = e;
		break;
	case 108:
		l = h;
		break;
	case 109:
		l = l;
		break;
	case 110:
		l = read((h << 8) + l);
		break;
	case 111:
		l = a;
		break;
	case 112:
		write((h << 8) + l, b);
		break;
	case 113:
		write((h << 8) + l, c);
		break;
	case 114:
		write((h << 8) + l, d);
		break;
	case 115:
		write((h << 8) + l, e);
		break;
	case 116:
		write((h << 8) + l, h);
		break;
	case 117:
		write((h << 8) + l, l);
		break;
	case 118:
		throw "Not implemented";
		break;
	case 119:
		write((h << 8) + l, a);
		break;
	case 120:
		a = b;
		break;
	case 121:
		a = c;
		break;
	case 122:
		a = d;
		break;
	case 123:
		a = e;
		break;
	case 124:
		a = h;
		break;
	case 125:
		a = l;
		break;
	case 126:
		a = read((h << 8) + l);
		break;
	case 127:
		a = a;
		break;
	case 128:
		add(&a, b);
		break;
	case 129:
		add(&a, c);
		break;
	case 130:
		add(&a, d);
		break;
	case 131:
		add(&a, e);
		break;
	case 132:
		add(&a, h);
		break;
	case 133:
		add(&a, l);
		break;
	case 134:
		add(&a, read((h << 8) + l));
		break;
	case 135:
		add(&a, a);
		break;
	case 136:
		add(&a, b + flags.C);
		break;
	case 137:
		add(&a, c + flags.C);
		break;
	case 138:
		add(&a, d + flags.C);
		break;
	case 139:
		add(&a, e + flags.C);
		break;
	case 140:
		add(&a, h + flags.C);
		break;
	case 141:
		add(&a, l + flags.C);
		break;
	case 142:
		add(&a, read((h << 8) + l) + flags.C);
		break;
	case 143:
		add(&a, a + flags.C);
		break;
	case 144:
		sub(&a, b);
		break;
	case 145:
		sub(&a, c);
		break;
	case 146:
		sub(&a, d);
		break;
	case 147:
		sub(&a, e);
		break;
	case 148:
		sub(&a, h);
		break;
	case 149:
		sub(&a, l);
		break;
	case 150:
		sub(&a, read((h << 8) + l));
		break;
	case 151:
		sub(&a, a);
		break;
	case 152:
		sub(&a, b + flags.C);
		break;
	case 153:
		sub(&a, c + flags.C);
		break;
	case 154:
		sub(&a, d + flags.C);
		break;
	case 155:
		sub(&a, e + flags.C);
		break;
	case 156:
		sub(&a, h + flags.C);
		break;
	case 157:
		sub(&a, l + flags.C);
		break;
	case 158:
		sub(&a, read((h << 8) + l) + flags.C);
		break;
	case 159:
		sub(&a, a + flags.C);
		break;
	case 160:
		a = a & b;
		flags.Z = a == 0 ? 1 : 0;
		flags.N = 0;
		flags.H = 1;
		flags.C = 0;
		break;
	case 161:
		a = a & c;
		flags.Z = a == 0 ? 1 : 0;
		flags.N = 0;
		flags.H = 1;
		flags.C = 0;
		break;
	case 162:
		a = a & d;
		flags.Z = a == 0 ? 1 : 0;
		flags.N = 0;
		flags.H = 1;
		flags.C = 0;
		break;
	case 163:
		a = a & e;
		flags.Z = a == 0 ? 1 : 0;
		flags.N = 0;
		flags.H = 1;
		flags.C = 0;
		break;
	case 164:
		a = a & h;
		flags.Z = a == 0 ? 1 : 0;
		flags.N = 0;
		flags.H = 1;
		flags.C = 0;
		break;
	case 165:
		a = a & l;
		flags.Z = a == 0 ? 1 : 0;
		flags.N = 0;
		flags.H = 1;
		flags.C = 0;
		break;
	case 166:
		a = a & read((h << 8) + l);
		flags.Z = a == 0 ? 1 : 0;
		flags.N = 0;
		flags.H = 1;
		flags.C = 0;
		break;
	case 167:
		a = a & a;
		flags.Z = a == 0 ? 1 : 0;
		flags.N = 0;
		flags.H = 1;
		flags.C = 0;
		break;
	case 168:
		a = a ^ b;
		flags.Z = a == 0 ? 1 : 0;
		flags.N = 0;
		flags.H = 0;
		flags.C = 0;
		break;
	case 169:
		a = a ^ c;
		flags.Z = a == 0 ? 1 : 0;
		flags.N = 0;
		flags.H = 0;
		flags.C = 0;
		break;
	case 170:
		a = a ^ d;
		flags.Z = a == 0 ? 1 : 0;
		flags.N = 0;
		flags.H = 0;
		flags.C = 0;
		break;
	case 171:
		a = a ^ e;
		flags.Z = a == 0 ? 1 : 0;
		flags.N = 0;
		flags.H = 0;
		flags.C = 0;
		break;
	case 172:
		a = a ^ h;
		flags.Z = a == 0 ? 1 : 0;
		flags.N = 0;
		flags.H = 0;
		flags.C = 0;
		break;
	case 173:
		a = a ^ l;
		flags.Z = a == 0 ? 1 : 0;
		flags.N = 0;
		flags.H = 0;
		flags.C = 0;
		break;
	case 174:
		a = a ^ read((h << 8) + l);
		flags.Z = a == 0 ? 1 : 0;
		flags.N = 0;
		flags.H = 0;
		flags.C = 0;
		break;
	case 175:
		a = a ^ a;
		flags.Z = a == 0 ? 1 : 0;
		flags.N = 0;
		flags.H = 0;
		flags.C = 0;
		break;
	case 176:
		a = a | b;
		flags.Z = a == 0 ? 1 : 0;
		flags.N = 0;
		flags.H = 0;
		flags.C = 0;
		break;
	case 177:
		a = a | c;
		flags.Z = a == 0 ? 1 : 0;
		flags.N = 0;
		flags.H = 0;
		flags.C = 0;
		break;
	case 178:
		a = a | d;
		flags.Z = a == 0 ? 1 : 0;
		flags.N = 0;
		flags.H = 0;
		flags.C = 0;
		break;
	case 179:
		a = a | e;
		flags.Z = a == 0 ? 1 : 0;
		flags.N = 0;
		flags.H = 0;
		flags.C = 0;
		break;
	case 180:
		a = a | h;
		flags.Z = a == 0 ? 1 : 0;
		flags.N = 0;
		flags.H = 0;
		flags.C = 0;
		break;
	case 181:
		a = a | l;
		flags.Z = a == 0 ? 1 : 0;
		flags.N = 0;
		flags.H = 0;
		flags.C = 0;
		break;
	case 182:
		a = a | read((h << 8) + l);
		flags.Z = a == 0 ? 1 : 0;
		flags.N = 0;
		flags.H = 0;
		flags.C = 0;
		break;
	case 183:
		a = a | a;
		flags.Z = a == 0 ? 1 : 0;
		flags.N = 0;
		flags.H = 0;
		flags.C = 0;
		break;
	case 184:
		cp(b);
		break;
	case 185:
		cp(c);
		break;
	case 186:
		cp(d);
		break;
	case 187:
		cp(e);
		break;
	case 188:
		cp(h);
		break;
	case 189:
		cp(l);
		break;
	case 190:
		cp(read((h << 8) + l));
		break;
	case 191:
		cp(a);
		break;
	case 192:
		if (flags.Z == 0)
		{
			pc = read(sp++);
			pc |= read(sp++) << 8;
			ins.cycles += 12;
		}
		break;
	case 193:
		c = read(sp++);
		b = read(sp++);
		break;
	case 194:
		if (flags.Z == 0)
		{
			pc = ins.param16;
			ins.cycles += 4;
		}
		break;
	case 195:
		pc = ins.param16;
		break;
	case 196:
		if (flags.Z == 0)
		{
			write(--sp, pc >> 8);
			write(--sp, pc & 0b11111111);
			pc = ins.param16;
			ins.cycles += 12;
		}
		break;
	case 197:
		write(--sp, b);
		write(--sp, c);
		break;
	case 198:
		add(&a, ins.param8);
		break;
	case 199:
		write(--sp, pc >> 8);
		write(--sp, pc & 0b11111111);
		pc = 0x0000;
		break;
	case 200:
		if (flags.Z == 1)
		{
			pc = read(sp++);
			pc |= read(sp++) << 8;
			ins.cycles += 12;
		}
		break;
	case 201:
		pc = read(sp++);
		pc |= read(sp++) << 8;
		break;
	case 202:
		if (flags.Z == 1)
		{
			pc = ins.param16;
			ins.cycles += 4;
		}
		break;
	case 203: // CB PREFIX
		cb(ins.param8);
		break;
	case 204:
		if (flags.Z == 1)
		{
			write(--sp, pc >> 8);
			write(--sp, pc & 0b11111111);
			pc = ins.param16;
			ins.cycles += 12;
		}
		break;
	case 205:
		write(--sp, pc >> 8);
		write(--sp, pc & 0b11111111);
		pc = ins.param16;
		break;
	case 206:
		add(&a, ins.param8 + flags.C);
		break;
	case 207:
		write(--sp, pc >> 8);
		write(--sp, pc & 0b11111111);
		pc = 0x0008;
		break;
	case 208:
		if (flags.C == 0)
		{
			pc = read(sp++);
			pc |= read(sp++) << 8;
			ins.cycles += 12;
		}
		break;
	case 209:
		e = read(sp++);
		d = read(sp++);
		break;
	case 210:
		if (flags.C == 0)
		{
			pc = ins.param16;
			ins.cycles += 4;
		}
		break;
	case 211:
		throw "Invalid opcode";
		break;
	case 212:
		if (flags.C == 0)
		{
			write(--sp, pc >> 8);
			write(--sp, pc & 0b11111111);
			pc = ins.param16;
			ins.cycles += 12;
		}
		break;
	case 213:
		write(--sp, d);
		write(--sp, e);
		break;
	case 214:
		sub(&a, ins.param8);
		break;
	case 215:
		write(--sp, pc >> 8);
		write(--sp, pc & 0b11111111);
		pc = 0x0010;
		break;
	case 216:
		if (flags.C == 1)
		{
			pc = read(sp++);
			pc |= read(sp++) << 8;
			ins.cycles += 12;
		}
		break;
	case 217:
		ime = true;
		pc = read(sp++);
		pc |= read(sp++) << 8;
		break;
	case 218:
		if (flags.C == 1)
		{
			pc = ins.param16;
			ins.cycles += 4;
		}
		break;
	case 219:
		throw "Invalid opcode";
		break;
	case 220:
		if (flags.C == 1)
		{
			write(sp--, pc >> 8);
			write(sp--, pc & 0b11111111);
			pc = ins.param16;
			ins.cycles += 12;
		}
		break;
	case 221:
		throw "Invalid opcode";
		break;
	case 222:
		sub(&a, ins.param8 + flags.C);
		break;
	case 223:
		write(--sp, pc >> 8);
		write(--sp, pc & 0b11111111);
		pc = 0x0018;
		break;
	case 224:
		write(0xFF00 + ins.param8, a);
		break;
	case 225:
		l = read(sp++);
		h = read(sp++);
		break;
	case 226:
		write(0xFF00 + c, a);
		break;
	case 227:
		throw "Invalid opcode";
		break;
	case 228:
		throw "Invalid opcode";
		break;
	case 229:
		write(--sp, h);
		write(--sp, l);
		break;
	case 230:
		a = a & ins.param8;
		flags.Z = a == 0 ? 1 : 0;
		flags.N = 0;
		flags.H = 1;
		flags.C = 0;
		break;
	case 231:
		write(--sp, pc >> 8);
		write(--sp, pc & 0b11111111);
		pc = 0x0020;
		break;
	case 232:
		flags.Z = 0;
		flags.N = 0;
		if ((sp & 0x0FFF) + ins.param8 > 0x0FFF)
		{
			flags.H = 1;
		}
		else
		{
			flags.H = 0;
		}
		sp += ins.param8; 
		if (sp < ins.param8)
		{
			flags.C = 1;
		}
		else
		{
			flags.C = 0;
		}
		break;
	case 233:
		pc = l;
		pc |= h << 8;
		break;
	case 234:
		write(ins.param16, a);
		break;
	case 235:
		throw "Invalid opcode";
		break;
	case 236:
		throw "Invalid opcode";
		break;
	case 237:
		throw "Invalid opcode";
		break;
	case 238:
		a = a ^ ins.param8;
		flags.Z = a == 0 ? 1 : 0;
		flags.N = 0;
		flags.H = 0;
		flags.C = 0;
		break;
	case 239:
		write(--sp, pc >> 8);
		write(--sp, pc & 0b11111111);
		pc = 0x0028;
		break;
	case 240:
		a = read(0xFF00 + ins.param8);
		break;
	case 241:
		flags.reg = read(sp++);
		flags.reg &= 0xF0;
		a = read(sp++);
		break;
	case 242:
		a = read(0xFF00 + c);
		break;
	case 243:
		ime = false;
		break;
	case 244:
		throw "Invalid opcode";
		break;
	case 245:
		write(--sp, a);
		write(--sp, flags.reg);
		break;
	case 246:
		a = a | ins.param8;
		flags.Z = a == 0 ? 1 : 0;
		flags.N = 0;
		flags.H = 0;
		flags.C = 0;
		break;
	case 247:
		write(--sp, pc >> 8);
		write(--sp, pc & 0b11111111);
		pc = 0x0030;
		break;
	case 248:
	{
		uint16_t init = sp;
		uint16_t res = sp + (int8_t)ins.param8;
		h = res >> 8;
		l = res & 0xFF;
		flags.Z = 0;
		flags.H = (init & 0x0FFF) + (int8_t)ins.param8 > 0x0FFF;
		flags.N = 0;
		flags.C = (init & 0x8000) && !(res & 0x8000);
	}
		break;
	case 249:
		sp = l;
		sp |= h << 8;
		break;
	case 250:
		a = read(ins.param16);
		break;
	case 251:
		ime = true;
		break;
	case 252:
		throw "Invalid opcode";
		break;
	case 253:
		throw "Invalid opcode";
		break;
	case 254:
		cp(ins.param8);
		break;
	case 255:
		write(--sp, pc >> 8);
		write(--sp, pc & 0b11111111);
		pc = 0x0038;
		break;
	}
}


void CPU::inc(uint8_t* reg)
{
	(*reg)++;
	flags.N = 0;
	if (*reg == 0)
	{
		flags.Z = 1;
	}
	else
	{
		flags.Z = 0;
	}
	if ((*reg & 0b00001111) == 0)
	{
		flags.H = 1;
	}
	else
	{
		flags.H = 0;
	}
}

void CPU::incMem(uint16_t addr)
{
	write(addr, read(addr) + 1);
	flags.N = 0;
	if (read(addr) == 256)
	{
		write(addr, 0);
		flags.Z = 1;
	}
	else
	{
		flags.Z = 0;
	}
	if ((read(addr) & 0b00001111) == 0)
	{
		flags.H = 1;
	}
	else
	{
		flags.H = 0;
	}
}

void CPU::dec(uint8_t* reg)
{
	(*reg)--;
	flags.N = 0;
	if (*reg == -1)
	{
		*reg = 255;
	}
	if (*reg == 0)
	{
		flags.Z = 1;
	}
	else
	{
		flags.Z = 0;
	}
	if ((*reg & 0b00001111) == 0b00001111)
	{
		flags.H = 1;
	}
	else
	{
		flags.H = 0;
	}
}

void CPU::decMem(uint16_t addr)
{
	write(addr, read(addr) - 1);
	flags.N = 0;
	if (read(addr) == -1)
	{
		write(addr, 255);
		flags.Z = 1;
	}
	else
	{
		flags.Z = 0;
	}
	if ((read(addr) & 0b00001111) == 0)
	{
		flags.H = 1;
	}
	else
	{
		flags.H = 0;
	}
}

void CPU::add(uint8_t* reg, int val)
{
	flags.N = 0;
	*reg += val;
	if (((*reg & 0xF + (val & 0xF)) & 0x10) == 0x10)
	{
		flags.H = 1;
	}
	else
	{
		flags.H = 0;
	}
	if (*reg > 255)
	{
		*reg -= 256;
		flags.C = 1;
	}
	else if (*reg < 0)
	{
		*reg += 256;
		flags.C = 1;
	}
	else
	{
		flags.C = 0;
	}
	if (*reg == 0)
	{
		flags.Z = 1;
	}
	else
	{
		flags.Z = 0;
	}
}

void CPU::sub(uint8_t* reg, int val)
{
	flags.N = 1;
	uint8_t init = *reg;
	*reg -= val;
	if (((*reg & 0xF + (((val & 0xF) ^ (0xF)) + 1) & 0x10) == 0x10))
	{
		flags.H = 1;
	}
	else
	{
		flags.H = 0;
	}
	if (*reg > init)
	{
		flags.C = 1;
	}
	else
	{
		flags.C = 0;
	}
	if (*reg == 0)
	{
		flags.Z = 1;
	}
	else
	{
		flags.Z = 0;
	}
}

void CPU::cp(int val)
{
	flags.N = 1;
	int diff = a - val;
	if (diff < 0)
	{
		flags.C = 1;
		flags.H = 0;
		flags.Z = 0;
	}
	else if (diff > 0)
	{
		flags.C = 0;
		flags.H = 1;
		flags.Z = 0;
	}
	else
	{
		flags.C = 0;
		flags.H = 0;
		flags.Z = 1;
	}
}

void CPU::cb(uint8_t opcode)
{
	std::string op;
	if ((opcode & 0b11111000) == 0b00000000) {
		op = "RLC";
	}
	else if ((opcode & 0b11111000) == 0b00001000) {
		op = "RRC";
	}
	else if ((opcode & 0b11111000) == 0b00010000) {
		op = "RL";
	}
	else if ((opcode & 0b11111000) == 0b00011000) {
		op = "RR";
	}
	else if ((opcode & 0b11111000) == 0b00100000) {
		op = "SLA";
	}
	else if ((opcode & 0b11111000) == 0b00101000) {
		op = "SRA";
	}
	else if ((opcode & 0b11111000) == 0b00110000) {
		op = "SWAP";
	}
	else if ((opcode & 0b11111000) == 0b00111000) {
		op = "SRL";
	}
	else if ((opcode & 0b11000000) == 0b01000000) {
		op = "BIT";
	}
	else if ((opcode & 0b11000000) == 0b10000000) {
		op = "RES";
	}
	else if ((opcode & 0b11000000) == 0b11000000) {
		op = "SET";
	}

	uint8_t bit = (opcode & 0b00111000) >> 3;

	uint8_t data;
	switch (opcode & 0b111) {
	case 0:
		data = b;
		break;
	case 1:
		data = c;
		break;
	case 2:
		data = d;
		break;
	case 3:
		data = e;
		break;
	case 4:
		data = h;
		break;
	case 5:
		data = l;
		break;
	case 6:
		data = read((h << 8) + l);
		break;
	case 7:
		data = a;
		break;
	}
	int tmp;
	if (op == "RLC") {
		flags.N = 0;
		flags.H = 0;
		flags.C = data & 0b10000000;
		data = data << 1;
		data = data | flags.C;
		flags.Z = data == 0 ? 0 : 1;
	}
	else if (op == "RRC") {
		flags.N = 0;
		flags.H = 0;
		flags.C = data & 0b00000001;
		data = data >> 1;
		data = data | (flags.C << 7);
		flags.Z = data == 0 ? 0 : 1;
	}
	else if (op == "RL") {
		tmp = flags.C;
		flags.N = 0;
		flags.H = 0;
		flags.C = data & 0b10000000;
		data = data << 1;
		data = data | tmp;
		flags.Z = data == 0 ? 0 : 1;
	}
	else if (op == "RR") {
		tmp = flags.C;
		flags.N = 0;
		flags.H = 0;
		flags.C = data & 0b00000001;
		data = data >> 1;
		data = data | (tmp << 7);
		flags.Z = data == 0 ? 0 : 1;
	}
	else if (op == "SLA") {
		flags.N = 0;
		flags.H = 0;
		flags.C = data & 0b10000000;
		data = data << 1;
		flags.Z = data == 0 ? 0 : 1;
	}
	else if (op == "SRA") {
		tmp = data & 0b10000000;
		flags.N = 0;
		flags.H = 0;
		flags.C = data & 0b00000001;
		data = data >> 1;
		data = data | tmp;
		flags.Z = data == 0 ? 0 : 1;
	}
	else if (op == "SWAP") {
		flags.N = 0;
		flags.H = 0;
		flags.C = 0;
		tmp = data & 0b00001111;
		data = data >> 4;
		data = data & (tmp << 4);
		flags.Z = data == 0 ? 0 : 1;
	}
	else if (op == "SRL") {

		flags.N = 0;
		flags.H = 0;
		flags.C = data & 0b00000001;
		data = data >> 1;
		flags.Z = data == 0 ? 0 : 1;
	}
	else if (op == "BIT") {
		flags.N = 0;
		flags.H = 1;
		flags.Z = data & (1 << bit);
		flags.Z = 1 - flags.Z;
	}
	else if (op == "RES") {
		data = data & (~(1 << bit));
	}
	else if (op == "SET") {
		data = data | (1 << bit);
	}

	switch (opcode & 0b111) {
	case 0:
		b = data;
		break;
	case 1:
		c = data;
		break;
	case 2:
		d = data;
		break;
	case 3:
		e = data;
		break;
	case 4:
		h = data;
		break;
	case 5:
		l = data;
		break;
	case 6:
		write((h << 8) + l, data);
		break;
	case 7:
		a = data;
		break;
	}
}

uint8_t CPU::daa()
{
	if (flags.N) {
		if (flags.C) {
			if (flags.H) {
				return 0x9A;
			}
			else {
				return 0xA0;
			}
		}
		else {
			if (flags.H) {
				return 0xFA;
			}
			else {
				return 0x00;
			}
		}
	}
	else {
		if (flags.C) {
			if ((a >> 4) < 0x03) {
				if ((a & 0x0F) < 0x0A) {
					return 0x60;
				}
				else {
					return 0x66;
				}
			}
			else {
				return 0x66;
			}
		}
		else {
			if (flags.H) {
				if ((a >> 4) < 0x0A) {
					return 0x06;
				}
				else {
					flags.C = true;
					return 0x66;
				}
			}
			else {
				if ((a & 0x0F) < 0x0A) {
					if ((a >> 4) < 0x0A) {
						return 0x00;
					}
					else {
						flags.C = true;
						return 0x60;
					}
				}
				else {
					if ((a >> 4) < 0x09) {
						return 0x06;
					}
					else {
						flags.C = true;
						return 0x66;
					}
				}
			}
		}
	}
}
