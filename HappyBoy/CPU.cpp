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
		{ 1, 4 }, { 3, 12 }, { 1, 8 }, { 1, 8 }, { 1, 4 }, { 1, 4 }, { 2, 8 }, { 1, 4 }, { 3, 20 }, { 1, 8 }, { 1, 8 }, { 1, 8 }, { 1, 4 }, { 1, 4 }, { 2, 8 }, { 1, 4 },
		{ 2, 4 }, { 3, 12 }, { 1, 8 }, { 1, 8 }, { 1, 4 }, { 1, 4 }, { 2, 8 }, { 1, 4 }, { 2, 12 }, { 1, 8 }, { 1, 8 }, { 1, 8 }, { 1, 4 }, { 1, 4 }, { 2, 8 }, { 1, 4 },
		{ 2, 8 }, { 3, 12 }, { 1, 8 }, { 1, 8 }, { 1, 4 }, { 1, 4 }, { 2, 8 }, { 1, 4 }, { 2, 8 }, { 1, 8 }, { 1, 8 }, { 1, 8 }, { 1, 4 }, { 1, 4 }, { 2, 8 }, { 1, 4 },
		{ 2, 8 }, { 3, 12 }, { 1, 8 }, { 1, 8 }, { 1, 12 }, { 1, 12 }, { 2, 12 }, { 1, 4 }, { 2, 8 }, { 1, 8 }, { 1, 8 }, { 1, 8 }, { 1, 4 }, { 1, 4 }, { 2, 8 }, { 1, 4 },
		{ 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 8 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 8 }, { 1, 4 },
		{ 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 8 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 8 }, { 1, 4 },
		{ 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 8 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 8 }, { 1, 4 },
		{ 1, 8 }, { 1, 8 }, { 1, 8 }, { 1, 8 }, { 1, 8 }, { 1, 8 }, { 1, 4 }, { 1, 8 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 8 }, { 1, 4 },
		{ 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 8 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 8 }, { 1, 4 },
		{ 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 8 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 8 }, { 1, 4 },
		{ 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 8 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 8 }, { 1, 4 },
		{ 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 8 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 8 }, { 1, 4 },
		{ 1, 8 }, { 1, 12 }, { 3, 12 }, { 3, 16 }, { 3, 12 }, { 1, 16 }, { 2, 8 }, { 1, 16 }, { 1, 8 }, { 1, 16 }, { 3, 12 }, { 1, 4 }, { 3, 12 }, { 3, 24 }, { 2, 8 }, { 1, 16 },
		{ 1, 8 }, { 1, 12 }, { 3, 12 }, { 1, 1 }, { 3, 12 }, { 1, 16 }, { 2, 8 }, { 1, 16 }, { 1, 8 }, { 1, 16 }, { 3, 12 }, { 1, 1 }, { 3, 12 }, { 1, 1 }, { 2, 8 }, { 1, 16 },
		{ 2, 12 }, { 1, 12 }, { 1, 8 }, { 1, 1 }, { 1, 1 }, { 1, 16 }, { 2, 8 }, { 1, 16 }, { 2, 16 }, { 1, 4 }, { 3, 16 }, { 1, 1 }, { 1, 1 }, { 1, 1 }, { 2, 8 }, { 1, 16 },
		{ 2, 12 }, { 1, 12 }, { 1, 8 }, { 1, 4 }, { 1, 1 }, { 1, 16 }, { 2, 8 }, { 1, 16 }, { 2, 12 }, { 1, 8 }, { 3, 16 }, { 1, 4 }, { 1, 1 }, { 1, 1 }, { 2, 8 }, { 1, 16 },
	};

	cb_glossary = {
		{ 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 },
		{ 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 },
		{ 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 },
		{ 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 },
		{ 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 },
		{ 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 },
		{ 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 },
		{ 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 },
		{ 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 },
		{ 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 },
		{ 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 },
		{ 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 },
		{ 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 },
		{ 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 },
		{ 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 },
		{ 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 8 }, { 2, 16 }, { 2, 8 },
	};	
	
	names = {
		"NOP", "LD BC,d16", "LD (BC),A", "INC BC", "INC B", "DEC B", "LD B,d8", "RLCA", "LD (a16),SP", "ADD HL,BC", "LD A,(BC)", "DEC BC", "INC C", "DEC C", "LD C,d8", "RRCA",
		"STOP 0", "LD DE,d16", "LD (DE),A", "INC DE", "INC D", "DEC D", "LD D,d8", "RLA", "JR r8", "ADD HL,DE", "LD A,(DE)", "DEC DE", "INC E", "DEC E", "LD E,d8", "RRA",
		"JR NZ,r8", "LD HL,d16", "LD (HL+),A", "INC HL", "INC H", "DEC H", "LD H,d8", "DAA", "JR Z,r8", "ADD HL,HL", "LD A,(HL+)", "DEC HL", "INC L", "DEC L", "LD L,d8", "CPL",
		"JR NC,r8", "LD SP,d16", "LD (HL-),A", "INC SP", "INC (HL)", "DEC (HL)", "LD (HL),d8", "SCF", "JR C,r8", "ADD HL,SP", "LD A,(HL-)", "DEC SP", "INC A", "DEC A", "LD A,d8", "CCF",
		"LD B,B", "LD B,C", "LD B,D", "LD B,E", "LD B,H", "LD B,L", "LD B,(HL)", "LD B,A", "LD C,B", "LD C,C", "LD C,D", "LD C,E", "LD C,H", "LD C,L", "LD C,(HL)", "LD C,A",
		"LD D,B", "LD D,C", "LD D,D", "LD D,E", "LD D,H", "LD D,L", "LD D,(HL)", "LD D,A", "LD E,B", "LD E,C", "LD E,D", "LD E,E", "LD E,H", "LD E,L", "LD E,(HL)", "LD E,A",
		"LD H,B", "LD H,C", "LD H,D", "LD H,E", "LD H,H", "LD H,L", "LD H,(HL)", "LD H,A", "LD L,B", "LD L,C", "LD L,D", "LD L,E", "LD L,H", "LD L,L", "LD L,(HL)", "LD L,A",
		"LD (HL),B", "LD (HL),C", "LD (HL),D", "LD (HL),E", "LD (HL),H", "LD (HL),L", "HALT", "LD (HL),A", "LD A,B", "LD A,C", "LD A,D", "LD A,E", "LD A,H", "LD A,L", "LD A,(HL)", "LD A,A",
		"ADD A,B", "ADD A,C", "ADD A,D", "ADD A,E", "ADD A,H", "ADD A,L", "ADD A,(HL)", "ADD A,A", "ADC A,B", "ADC A,C", "ADC A,D", "ADC A,E", "ADC A,H", "ADC A,L", "ADC A,(HL)", "ADC A,A",
		"SUB B", "SUB C", "SUB D", "SUB E", "SUB H", "SUB L", "SUB (HL)", "SUB A", "SBC A,B", "SBC A,C", "SBC A,D", "SBC A,E", "SBC A,H", "SBC A,L", "SBC A,(HL)", "SBC A,A",
		"AND B", "AND C", "AND D", "AND E", "AND H", "AND L", "AND (HL)", "AND A", "XOR B", "XOR C", "XOR D", "XOR E", "XOR H", "XOR L", "XOR (HL)", "XOR A",
		"OR B", "OR C", "OR D", "OR E", "OR H", "OR L", "OR (HL)", "OR A", "CP B", "CP C", "CP D", "CP E", "CP H", "CP L", "CP (HL)", "CP A",
		"RET NZ", "POP BC", "JP NZ,a16", "JP a16", "CALL NZ,a16", "PUSH BC", "ADD A,d8", "RST 00H", "RET Z", "RET", "JP Z,a16", "PREFIX CB", "CALL Z,a16", "CALL a16", "ADC A,d8", "RST 08H",
		"RET NC", "POP DE", "JP NC,a16", "???", "CALL NC,a16", "PUSH DE", "SUB d8", "RST 10H", "RET C", "RETI", "JP C,a16", "???", "CALL C,a16", "???", "SBC A,d8", "RST 18H",
		"LDH (a8),A", "POP HL", "LD (C),A", "???", "???", "PUSH HL", "AND d8", "RST 20H", "ADD SP,r8", "JP (HL)", "LD (a16),A", "???", "???", "???", "XOR d8", "RST 28H",
		"LDH A,(a8)", "POP AF", "LD A,(C)", "DI", "???", "PUSH AF", "OR d8", "RST 30H", "LD HL,SP+r8", "LD SP,HL", "LD A,(a16)", "EI", "???", "???", "CP d8", "RST 38H",
		"RLC B", "RLC C", "RLC D", "RLC E", "RLC H", "RLC L", "RLC (HL)", "RLC A", "RRC B", "RRC C", "RRC D", "RRC E", "RRC H", "RRC L", "RRC (HL)", "RRC A",
		"RL B", "RL C", "RL D", "RL E", "RL H", "RL L", "RL (HL)", "RL A", "RR B", "RR C", "RR D", "RR E", "RR H", "RR L", "RR (HL)", "RR A",
		"SLA B", "SLA C", "SLA D", "SLA E", "SLA H", "SLA L", "SLA (HL)", "SLA A", "SRA B", "SRA C", "SRA D", "SRA E", "SRA H", "SRA L", "SRA (HL)", "SRA A",
		"SWAP B", "SWAP C", "SWAP D", "SWAP E", "SWAP H", "SWAP L", "SWAP (HL)", "SWAP A", "SRL B", "SRL C", "SRL D", "SRL E", "SRL H", "SRL L", "SRL (HL)", "SRL A",
		"BIT 0,B", "BIT 0,C", "BIT 0,D", "BIT 0,E", "BIT 0,H", "BIT 0,L", "BIT 0,(HL)", "BIT 0,A", "BIT 1,B", "BIT 1,C", "BIT 1,D", "BIT 1,E", "BIT 1,H", "BIT 1,L", "BIT 1,(HL)", "BIT 1,A",
		"BIT 2,B", "BIT 2,C", "BIT 2,D", "BIT 2,E", "BIT 2,H", "BIT 2,L", "BIT 2,(HL)", "BIT 2,A", "BIT 3,B", "BIT 3,C", "BIT 3,D", "BIT 3,E", "BIT 3,H", "BIT 3,L", "BIT 3,(HL)", "BIT 3,A",
		"BIT 4,B", "BIT 4,C", "BIT 4,D", "BIT 4,E", "BIT 4,H", "BIT 4,L", "BIT 4,(HL)", "BIT 4,A", "BIT 5,B", "BIT 5,C", "BIT 5,D", "BIT 5,E", "BIT 5,H", "BIT 5,L", "BIT 5,(HL)", "BIT 5,A",
		"BIT 6,B", "BIT 6,C", "BIT 6,D", "BIT 6,E", "BIT 6,H", "BIT 6,L", "BIT 6,(HL)", "BIT 6,A", "BIT 7,B", "BIT 7,C", "BIT 7,D", "BIT 7,E", "BIT 7,H", "BIT 7,L", "BIT 7,(HL)", "BIT 7,A",
		"RES 0,B", "RES 0,C", "RES 0,D", "RES 0,E", "RES 0,H", "RES 0,L", "RES 0,(HL)", "RES 0,A", "RES 1,B", "RES 1,C", "RES 1,D", "RES 1,E", "RES 1,H", "RES 1,L", "RES 1,(HL)", "RES 1,A",
		"RES 2,B", "RES 2,C", "RES 2,D", "RES 2,E", "RES 2,H", "RES 2,L", "RES 2,(HL)", "RES 2,A", "RES 3,B", "RES 3,C", "RES 3,D", "RES 3,E", "RES 3,H", "RES 3,L", "RES 3,(HL)", "RES 3,A",
		"RES 4,B", "RES 4,C", "RES 4,D", "RES 4,E", "RES 4,H", "RES 4,L", "RES 4,(HL)", "RES 4,A", "RES 5,B", "RES 5,C", "RES 5,D", "RES 5,E", "RES 5,H", "RES 5,L", "RES 5,(HL)", "RES 5,A",
		"RES 6,B", "RES 6,C", "RES 6,D", "RES 6,E", "RES 6,H", "RES 6,L", "RES 6,(HL)", "RES 6,A", "RES 7,B", "RES 7,C", "RES 7,D", "RES 7,E", "RES 7,H", "RES 7,L", "RES 7,(HL)", "RES 7,A",
		"SET 0,B", "SET 0,C", "SET 0,D", "SET 0,E", "SET 0,H", "SET 0,L", "SET 0,(HL)", "SET 0,A", "SET 1,B", "SET 1,C", "SET 1,D", "SET 1,E", "SET 1,H", "SET 1,L", "SET 1,(HL)", "SET 1,A",
		"SET 2,B", "SET 2,C", "SET 2,D", "SET 2,E", "SET 2,H", "SET 2,L", "SET 2,(HL)", "SET 2,A", "SET 3,B", "SET 3,C", "SET 3,D", "SET 3,E", "SET 3,H", "SET 3,L", "SET 3,(HL)", "SET 3,A",
		"SET 4,B", "SET 4,C", "SET 4,D", "SET 4,E", "SET 4,H", "SET 4,L", "SET 4,(HL)", "SET 4,A", "SET 5,B", "SET 5,C", "SET 5,D", "SET 5,E", "SET 5,H", "SET 5,L", "SET 5,(HL)", "SET 5,A",
		"SET 6,B", "SET 6,C", "SET 6,D", "SET 6,E", "SET 6,H", "SET 6,L", "SET 6,(HL)", "SET 6,A", "SET 7,B", "SET 7,C", "SET 7,D", "SET 7,E", "SET 7,H", "SET 7,L", "SET 7,(HL)", "SET 7,A",
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
		fetch();
		execute(current_instruction);
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
		fetch(true);
		std::string name = names[debug_instruction.opcode == 0xCB ? (debug_instruction.param8 + 0x100) : debug_instruction.opcode];
		size_t d8pos = name.find("d8");
		size_t r8pos = name.find("r8");
		size_t a8pos = name.find("a8");
		size_t a16pos = name.find("a16");
		size_t d16pos = name.find("d16");
		if (d8pos != std::string::npos) {
			std::stringstream s;
			s << "$" << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)debug_instruction.param8;
			name.replace(d8pos, 2, s.str());
		}
		if (r8pos != std::string::npos) {
			std::stringstream s;
			s << "$" << std::setfill('0') << std::setw(4) << std::hex << (int8_t)debug_instruction.param8 + pc;
			name.replace(r8pos, 2, s.str());
		}
		if (a8pos != std::string::npos) {
			std::stringstream s;
			s << "$" << std::setfill('0') << std::setw(4) << std::hex << (0xFF00 + debug_instruction.param8);
			name.replace(a8pos, 2, s.str());
		}
		if (a16pos != std::string::npos) {
			std::stringstream s;
			s << "$" << std::setfill('0') << std::setw(4) << std::hex << debug_instruction.param16;
			name.replace(a16pos, 3, s.str());
		}
		if (d16pos != std::string::npos) {
			std::stringstream s;
			s << "$" << std::setfill('0') << std::setw(4) << std::hex << debug_instruction.param16;
			name.replace(d16pos, 3, s.str());
		}

		std::stringstream s;
		s << "$" << std::setfill('0') << std::setw(4) << std::hex << debug_instruction.addr << ": " << name;
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
	write(0xFF50, 0);
}

void CPU::fetch(bool debug) {
	uint16_t addr = pc;
	uint8_t opcode = read(pc++);
	Instruction* i;
	if (debug) {
		i = &debug_instruction;
	}
	else {
		i = &current_instruction;
	}
	*i = glossary[opcode];
	if (opcode == 0xCB) {
		uint8_t cb_opcode = read(pc++);
		*i = cb_glossary[cb_opcode];
		i->param8 = cb_opcode;
	}
	else {
		if (i->bytes == 2) {
			i->param8 = read(pc++);
		}
		else if (i->bytes == 3) {
			i->param16 = read(pc++) + (read(pc++) << 8);
		}
	}
	i->opcode = opcode;
	i->addr = addr;
	if (!debug) {
		cyclesRemaining = i->cycles - 1;
	}
}

void CPU::execute(Instruction& ins)
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
	if ((opcode & 0b11111000) == 0b00000000) { // RLC
		flags.N = 0;
		flags.H = 0;
		flags.C = data & 0b10000000;
		data = data << 1;
		data = data | flags.C;
		flags.Z = data == 0 ? 0 : 1;
	}
	else if ((opcode & 0b11111000) == 0b00001000) { // RRC
		flags.N = 0;
		flags.H = 0;
		flags.C = data & 0b00000001;
		data = data >> 1;
		data = data | (flags.C << 7);
		flags.Z = data == 0 ? 0 : 1;
	}
	else if ((opcode & 0b11111000) == 0b00010000) { // RL
		tmp = flags.C;
		flags.N = 0;
		flags.H = 0;
		flags.C = data & 0b10000000;
		data = data << 1;
		data = data | tmp;
		flags.Z = data == 0 ? 0 : 1;
	}
	else if ((opcode & 0b11111000) == 0b00011000) { // RR
		tmp = flags.C;
		flags.N = 0;
		flags.H = 0;
		flags.C = data & 0b00000001;
		data = data >> 1;
		data = data | (tmp << 7);
		flags.Z = data == 0 ? 0 : 1;
	}
	else if ((opcode & 0b11111000) == 0b00100000) { // SLA
		flags.N = 0;
		flags.H = 0;
		flags.C = data & 0b10000000;
		data = data << 1;
		flags.Z = data == 0 ? 0 : 1;
	}
	else if ((opcode & 0b11111000) == 0b00101000) { // SRA
		tmp = data & 0b10000000;
		flags.N = 0;
		flags.H = 0;
		flags.C = data & 0b00000001;
		data = data >> 1;
		data = data | tmp;
		flags.Z = data == 0 ? 0 : 1;
	}
	else if ((opcode & 0b11111000) == 0b00110000) { // SWAP
		flags.N = 0;
		flags.H = 0;
		flags.C = 0;
		tmp = data & 0b00001111;
		data = data >> 4;
		data = data & (tmp << 4);
		flags.Z = data == 0 ? 0 : 1;
	}
	else if ((opcode & 0b11111000) == 0b00111000) { // SRL

		flags.N = 0;
		flags.H = 0;
		flags.C = data & 0b00000001;
		data = data >> 1;
		flags.Z = data == 0 ? 0 : 1;
	}
	else if ((opcode & 0b11000000) == 0b01000000) { // BIT
		flags.N = 0;
		flags.H = 1;
		flags.Z = data & (1 << bit);
		flags.Z = 1 - flags.Z;
	}
	else if ((opcode & 0b11000000) == 0b10000000) { // RES
		data = data & (~(1 << bit));
	}
	else if ((opcode & 0b11000000) == 0b11000000) { // SET
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
