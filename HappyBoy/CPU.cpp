#include <iomanip>
#include <iostream>
#include <sstream>

#include "CPU.h"

template<AddressingMode mode, class T>
T CPU::getOperand()
{
	switch (mode) {
	case AddressingMode::RegisterA:
		return AF.A;
		break;
	case AddressingMode::RegisterB:
		return BC.B;
		break;
	case AddressingMode::RegisterC:
		return BC.C;
		break;
	case AddressingMode::RegisterD:
		return DE.D;
		break;
	case AddressingMode::RegisterE:
		return DE.E;
		break;
	case AddressingMode::RegisterF:
		return AF.F.reg;
		break;
	case AddressingMode::RegisterH:
		return HL.H;
		break;
	case AddressingMode::RegisterL:
		return HL.L;
		break;
	default:
		throw std::logic_error{ "Not implemented" };
	}
}

template <AddressingMode mode, class T>
T CPU::getWriteTarget() {
	switch (mode) {
	case AddressingMode::RegisterA:
		return AF.A;
		break;
	case AddressingMode::RegisterB:
		return BC.B;
		break;
	case AddressingMode::RegisterC:
		return BC.C;
		break;
	case AddressingMode::RegisterD:
		return DE.D;
		break;
	case AddressingMode::RegisterE:
		return DE.E;
		break;
	case AddressingMode::RegisterF:
		return AF.F.reg;
		break;
	case AddressingMode::RegisterH:
		return HL.H;
		break;
	case AddressingMode::RegisterL:
		return HL.L;
		break;
	default:
		throw std::logic_error{ "Not implemented" };
	}
}

template<AddressingMode mode, class T>
void CPU::writeValue(T value)
{
	switch (mode) {
	case AddressingMode::RegisterA:
		AF.A = value;
		break;
	case AddressingMode::RegisterB:
		BC.B = value;
		break;
	case AddressingMode::RegisterC:
		BC.C = value;
		break;
	case AddressingMode::RegisterD:
		DE.D = value;
		break;
	case AddressingMode::RegisterE:
		DE.E = value;
		break;
	case AddressingMode::RegisterF:
		AF.F.reg = value & 0xF0;
		break;
	case AddressingMode::RegisterH:
		HL.H = value;
		break;
	case AddressingMode::RegisterL:
		HL.L = value;
		break;
	default:
		throw std::logic_error{ "Not implemented" };
	}
}

template <ConditionMode mode>
bool CPU::getConditional() {
	switch (mode) {
	case ConditionMode::Always:
		return true;
		break;
	case ConditionMode::NZ:
		return !AF.F.Z;
		break;
	case ConditionMode::Z:
		return AF.F.Z;
		break;
	case ConditionMode::NC:
		return !AF.F.C;
		break;
	case ConditionMode::C:
		return AF.F.C;
		break;
	}
}

void CPU::NOP() {
	return;
}

template <AddressingMode readMode>
void CPU::INC()
{
	uint8_t op = getOperand<readMode>();
	op++;
	AF.F.N = 0;
	if (op == 0)
	{
		AF.F.Z = 1;
	}
	else
	{
		AF.F.Z = 0;
	}
	if ((op & 0b00001111) == 0)
	{
		AF.F.H = 1;
	}
	else
	{
		AF.F.H = 0;
	}
}

template <AddressingMode readMode>
void CPU::DEC()
{
	uint8_t op = getOperand<readMode>();
	op--;
	AF.F.N = 0;
	if (op == 0)
	{
		AF.F.Z = 1;
	}
	else
	{
		AF.F.Z = 0;
	}
	if ((op & 0b00001111) == 0b1111)
	{
		AF.F.H = 1;
	}
	else
	{
		AF.F.H = 0;
	}
}

void CPU::RLCA() {
	AF.F.C = AF.A & 0x80;
	AF.A <<= 1;
	AF.A |= AF.F.C;
	AF.F.reg &= 0x80;
}

template <AddressingMode writeMode, AddressingMode readMode>
void CPU::ADD()
{
	AF.F.N = 0;
	uint8_t target = getWriteTarget<writeMode>();
	target += getOperand<readMode>();
	/*if (((target & 0xF + (val & 0xF)) & 0x10) == 0x10)
	{
		AF.F.H = 1;
	}
	else
	{
		AF.F.H = 0;
	}
	if (*reg > 255)
	{
		*reg -= 256;
		AF.F.C = 1;
	}
	else if (*reg < 0)
	{
		*reg += 256;
		AF.F.C = 1;
	}
	else
	{
		AF.F.C = 0;
	}
	if (*reg == 0)
	{
		AF.F.Z = 1;
	}
	else
	{
		AF.F.Z = 0;
	}*/
	writeValue<writeMode>(target);
}

void CPU::RRCA() {
	AF.F.C = AF.A & 0x01;
	AF.A >>= 1;
	AF.A |= (AF.F.C << 7);
	AF.F.reg &= 0x80;
}

void CPU::STOP() {
	stopped = true;
}

void CPU::RLA() {
	bool tmp = AF.F.C;
	AF.F.C = AF.A & 0x80;
	AF.A <<= 1;
	AF.A |= tmp;
	AF.F.reg &= 0x80;
}

template <ConditionMode conditionMode, AddressingMode readMode>
void CPU::JR() {
	if (getConditional<conditionMode>()) {
		PC += (int8_t)getOperand<readMode>();
	}
}

void CPU::RRA() {
	bool tmp = AF.F.C;
	AF.F.C = AF.A & 0x01;
	AF.A >>= 1;
	AF.A |= (tmp << 7);
	AF.F.reg &= 0x80;
}

void CPU::DAA() {
	if (AF.F.N) {
		if (AF.F.C) {
			if (AF.F.H) {
				AF.A += 0x9A;
			}
			else {
				AF.A += 0xA0;
			}
		}
		else {
			if (AF.F.H) {
				AF.A += 0xFA;
			}
			else {
				AF.A += 0x00;
			}
		}
	}
	else {
		if (AF.F.C) {
			if ((AF.A >> 4) < 0x03) {
				if ((AF.A & 0x0F) < 0x0A) {
					AF.A += 0x60;
				}
				else {
					AF.A += 0x66;
				}
			}
			else {
				AF.A += 0x66;
			}
		}
		else {
			if (AF.F.H) {
				if ((AF.A >> 4) < 0x0A) {
					AF.A += 0x06;
				}
				else {
					AF.F.C = true;
					AF.A += 0x66;
				}
			}
			else {
				if ((AF.A & 0x0F) < 0x0A) {
					if ((AF.A >> 4) < 0x0A) {
						AF.A += 0x00;
					}
					else {
						AF.F.C = true;
						AF.A += 0x60;
					}
				}
				else {
					if ((AF.A >> 4) < 0x09) {
						AF.A += 0x06;
					}
					else {
						AF.F.C = true;
						AF.A += 0x66;
					}
				}
			}
		}
	}
	AF.F.Z = !AF.A;
}

void CPU::CPL() {
	AF.A = ~AF.A;
}

void CPU::SCF() {
	AF.F.C = 1;
}

void CPU::CCF() {
	AF.F.C = !AF.F.C;
}

template <AddressingMode writeMode, AddressingMode readMode>
void CPU::LD() {
	writeValue<writeMode>(getOperand<readMode>());
}

void CPU::HALT() {
	halted = true;
}

template <AddressingMode writeMode, AddressingMode readMode>
void CPU::ADC() {
	AF.F.N = 0;
	uint8_t target = getWriteTarget<writeMode>();
	target += getOperand<readMode>() + AF.F.C;
	writeValue<writeMode>(target);
}

template <AddressingMode readMode>
void CPU::SUB() {
	AF.F.N = 1;
	AF.A -= getOperand<readMode>();
}

template <AddressingMode writeMode, AddressingMode readMode>
void CPU::SBC() {
	AF.F.N = 1;
	uint8_t target = getWriteTarget<writeMode>();
	target -= getOperand<readMode>() + AF.F.C;
	writeValue<writeMode>(target);
}

template <AddressingMode readMode>
void CPU::AND() {
	AF.F.C = 0;
	AF.F.H = 1;
	AF.F.N = 0;
	AF.A &= getOperand<readMode>();
	AF.F.Z = !(AF.A);
}

template <AddressingMode readMode>
void CPU::XOR() {
	AF.F.C = 0;
	AF.F.H = 0;
	AF.F.N = 0;
	AF.A ^= getOperand<readMode>();
	AF.F.Z = !(AF.A);
}

template <AddressingMode readMode>
void CPU::OR() {
	AF.F.C = 0;
	AF.F.H = 0;
	AF.F.N = 0;
	AF.A |= getOperand<readMode>();
	AF.F.Z = !(AF.A);
}

template <AddressingMode readMode>
void CPU::CP() {
	uint8_t operand = getOperand<readMode>();
	if (AF.A == operand) {
		AF.F.Z = 1;
		AF.F.H = 0;
		AF.F.C = 0;
	}
	else if (AF.A < operand) {
		AF.F.Z = 0;
		AF.F.H = 1;
		AF.F.C = 0;
	}
	else if (AF.A > operand) {
		AF.F.Z = 0;
		AF.F.H = 0;
		AF.F.C = 1;
	}
	AF.F.N = 1;
}

template <ConditionMode mode>
void CPU::RET() {
	if (getConditional<mode>()) {
		PC = readBus(SP++);
		PC |= readBus(SP++) << 8;
	}
}

template <AddressingMode writeMode>
void CPU::POP() {
	uint16_t operand = readBus(--SP);
	operand |= readBus(--SP);
	writeValue<writeMode, uint16_t>(operand);
}

template <ConditionMode conditionMode, AddressingMode readMode>
void CPU::JP() {
	if (getConditional<conditionMode>()) {
		PC = getOperand<readMode, uint16_t>();
	}
}

template <ConditionMode conditionMode, AddressingMode readMode>
void CPU::CALL() {
	if (getConditional<conditionMode>()) {
		writeBus(--SP, PC >> 8);
		writeBus(--SP, PC & 0xFF);
		PC = getOperand<readMode, uint16_t>();
	}
}

template <AddressingMode readMode>
void CPU::PUSH() {
	uint16_t operand = getOperand<readMode, uint16_t>();
	writeBus(SP++, operand & 0xFF);
	writeBus(SP++, operand >> 8);
}

template <uint16_t address>
void CPU::RST() {
	writeBus(--SP, PC >> 8);
	writeBus(--SP, PC & 0xFF);
	PC = address;
}

void CPU::RETI() {
	PC = readBus(SP++);
	PC |= readBus(SP++) << 8;
}

void CPU::DI() {
	ime = false;
}

void CPU::EI() {
	ime = true;
}

void CPU::undefined() {
	throw std::logic_error{ "Undefined opcode" };
}

uint8_t CPU::readBus(uint16_t addr)
{
	return bus->read(addr);
}

void CPU::writeBus(uint16_t addr, uint8_t data)
{
	bus->write(addr, data);
}

CPU::CPU(std::shared_ptr<Bus> bus) : bus(bus)
{
	opcode_funcs = {
 /*0x*/	&CPU::NOP, &CPU::LD<AddressingMode::RegisterBC, AddressingMode::Immediate16>, &CPU::LD<AddressingMode::AddressBC, AddressingMode::RegisterA>, &CPU::INC<AddressingMode::RegisterBC>, &CPU::INC<AddressingMode::RegisterB>, &CPU::DEC<AddressingMode::RegisterB>, &CPU::LD<AddressingMode::RegisterB, AddressingMode::Immediate8>, &CPU::RLCA, &CPU::LD<AddressingMode::Absolute16, AddressingMode::RegisterSP>, &CPU::ADD<AddressingMode::RegisterHL, AddressingMode::RegisterBC>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::AbsoluteBC>, &CPU::DEC<AddressingMode::RegisterBC>, &CPU::INC<AddressingMode::RegisterC>, &CPU::DEC<AddressingMode::RegisterC>, &CPU::LD<AddressingMode::RegisterC, AddressingMode::Immediate8>, &CPU::RRCA,
		&CPU::STOP, &CPU::LD<AddressingMode::RegisterDE, AddressingMode::Immediate16>, &CPU::LD<AddressingMode::AbsoluteDE, AddressingMode::RegisterA>, &CPU::INC<AddressingMode::RegisterDE>, &CPU::INC<AddressingMode::RegisterD>, &CPU::DEC<AddressingMode::RegisterD>, &CPU::LD<AddressingMode::RegisterD, AddressingMode::Immediate8>, &CPU::RLA, &CPU::JR<ConditionMode::Always, AddressingMode::Immediate8>, &CPU::ADD<AddressingMode::RegisterHL, AddressingMode::RegisterDE>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::AbsoluteDE>, &CPU::DEC<AddressingMode::RegisterDE>, &CPU::INC<AddressingMode::RegisterE>, &CPU::DEC<AddressingMode::RegisterE>, &CPU::LD<AddressingMode::RegisterE, AddressingMode::Immediate8>, &CPU::RRA,
		&CPU::JR<ConditionMode::NZ, AddressingMode::Immediate8>, &CPU::LD<AddressingMode::RegisterHL, AddressingMode::Immediate16>, &CPU::LD<AddressingMode::AbsoluteHLI, AddressingMode::RegisterA>, &CPU::INC<AddressingMode::RegisterHL>, &CPU::INC<AddressingMode::RegisterH>, &CPU::DEC<AddressingMode::RegisterH>, &CPU::LD<AddressingMode::RegisterH, AddressingMode::Immediate8>, &CPU::DAA, &CPU::JR<ConditionMode::Z, AddressingMode::Immediate8>, &CPU::ADD<AddressingMode::RegisterHL, AddressingMode::RegisterHL>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::AbsoluteHLI>, &CPU::DEC<AddressingMode::RegisterHL>, &CPU::INC<AddressingMode::RegisterL>, &CPU::DEC<AddressingMode::RegisterL>, &CPU::LD<AddressingMode::RegisterL, AddressingMode::Immediate8>, &CPU::CPL,
		&CPU::JR<ConditionMode::NC, AddressingMode::Immediate8>, &CPU::LD<AddressingMode::RegisterSP, AddressingMode::Immediate16>, &CPU::LD<AddressingMode::AbsoluteHLD, AddressingMode::RegisterA>, &CPU::INC<AddressingMode::RegisterSP>, &CPU::INC<AddressingMode::AbsoluteHL>, &CPU::DEC<AddressingMode::AbsoluteHL>, &CPU::LD<AddressingMode::AbsoluteHL, AddressingMode::Immediate8>, &CPU::SCF, &CPU::JR<ConditionMode::C, AddressingMode::Immediate8>, &CPU::ADD<AddressingMode::RegisterHL, AddressingMode::RegisterSP>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::AbsoluteHLD>, &CPU::DEC<AddressingMode::RegisterSP>, &CPU::INC<AddressingMode::RegisterA>, &CPU::DEC<AddressingMode::RegisterA>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::Immediate8>, &CPU::CCF,
		&CPU::LD<AddressingMode::RegisterB, AddressingMode::RegisterB>, &CPU::LD<AddressingMode::RegisterB, AddressingMode::RegisterC>, &CPU::LD<AddressingMode::RegisterB, AddressingMode::RegisterD>, &CPU::LD<AddressingMode::RegisterB, AddressingMode::RegisterE>, &CPU::LD<AddressingMode::RegisterB, AddressingMode::RegisterH>, &CPU::LD<AddressingMode::RegisterB, AddressingMode::RegisterL>, &CPU::LD<AddressingMode::RegisterB, AddressingMode::AbsoluteHL>, &CPU::LD<AddressingMode::RegisterB, AddressingMode::RegisterA>, &CPU::LD<AddressingMode::RegisterC, AddressingMode::RegisterB>, &CPU::LD<AddressingMode::RegisterC, AddressingMode::RegisterC>, &CPU::LD<AddressingMode::RegisterC, AddressingMode::RegisterD>, &CPU::LD<AddressingMode::RegisterC, AddressingMode::RegisterE>, &CPU::LD<AddressingMode::RegisterC, AddressingMode::RegisterH>, &CPU::LD<AddressingMode::RegisterC, AddressingMode::RegisterL>, &CPU::LD<AddressingMode::RegisterC, AddressingMode::AbsoluteHL>, &CPU::LD<AddressingMode::RegisterC, AddressingMode::RegisterA>,
		&CPU::LD<AddressingMode::RegisterD, AddressingMode::RegisterB>, &CPU::LD<AddressingMode::RegisterD, AddressingMode::RegisterC>, &CPU::LD<AddressingMode::RegisterD, AddressingMode::RegisterD>, &CPU::LD<AddressingMode::RegisterD, AddressingMode::RegisterE>, &CPU::LD<AddressingMode::RegisterD, AddressingMode::RegisterH>, &CPU::LD<AddressingMode::RegisterD, AddressingMode::RegisterL>, &CPU::LD<AddressingMode::RegisterD, AddressingMode::AbsoluteHL>, &CPU::LD<AddressingMode::RegisterD, AddressingMode::RegisterA>, &CPU::LD<AddressingMode::RegisterE, AddressingMode::RegisterB>, &CPU::LD<AddressingMode::RegisterE, AddressingMode::RegisterC>, &CPU::LD<AddressingMode::RegisterE, AddressingMode::RegisterD>, &CPU::LD<AddressingMode::RegisterE, AddressingMode::RegisterE>, &CPU::LD<AddressingMode::RegisterE, AddressingMode::RegisterH>, &CPU::LD<AddressingMode::RegisterE, AddressingMode::RegisterL>, &CPU::LD<AddressingMode::RegisterE, AddressingMode::AbsoluteHL>, &CPU::LD<AddressingMode::RegisterE, AddressingMode::RegisterA>,
		&CPU::LD<AddressingMode::RegisterH, AddressingMode::RegisterB>, &CPU::LD<AddressingMode::RegisterH, AddressingMode::RegisterC>, &CPU::LD<AddressingMode::RegisterH, AddressingMode::RegisterD>, &CPU::LD<AddressingMode::RegisterH, AddressingMode::RegisterE>, &CPU::LD<AddressingMode::RegisterH, AddressingMode::RegisterH>, &CPU::LD<AddressingMode::RegisterH, AddressingMode::RegisterL>, &CPU::LD<AddressingMode::RegisterH, AddressingMode::AbsoluteHL>, &CPU::LD<AddressingMode::RegisterH, AddressingMode::RegisterA>, &CPU::LD<AddressingMode::RegisterL, AddressingMode::RegisterB>, &CPU::LD<AddressingMode::RegisterL, AddressingMode::RegisterC>, &CPU::LD<AddressingMode::RegisterL, AddressingMode::RegisterD>, &CPU::LD<AddressingMode::RegisterL, AddressingMode::RegisterE>, &CPU::LD<AddressingMode::RegisterL, AddressingMode::RegisterH>, &CPU::LD<AddressingMode::RegisterL, AddressingMode::RegisterL>, &CPU::LD<AddressingMode::RegisterL, AddressingMode::AbsoluteHL>, &CPU::LD<AddressingMode::RegisterL, AddressingMode::RegisterA>,
		&CPU::LD<AddressingMode::AbsoluteHL, AddressingMode::RegisterB>, &CPU::LD<AddressingMode::AbsoluteHL, AddressingMode::RegisterC>, &CPU::LD<AddressingMode::AbsoluteHL, AddressingMode::RegisterD>, &CPU::LD<AddressingMode::AbsoluteHL, AddressingMode::RegisterE>, &CPU::LD<AddressingMode::AbsoluteHL, AddressingMode::RegisterH>, &CPU::LD<AddressingMode::AbsoluteHL, AddressingMode::RegisterL>, &CPU::HALT, &CPU::LD<AddressingMode::AbsoluteHL, AddressingMode::RegisterA>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::RegisterB>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::RegisterC>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::RegisterD>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::RegisterE>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::RegisterH>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::RegisterL>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::AbsoluteHL>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::RegisterA>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::RegisterA>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::RegisterA>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::RegisterA>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::RegisterA>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::RegisterA>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::RegisterA>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::RegisterA>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::RegisterA>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::RegisterA>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::RegisterA>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::RegisterA>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::RegisterA>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::RegisterA>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::RegisterA>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::RegisterA>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::RegisterA>,
		&CPU::ADD<AddressingMode::RegisterA, AddressingMode::RegisterB>, &CPU::ADD<AddressingMode::RegisterA, AddressingMode::RegisterC>, &CPU::ADD<AddressingMode::RegisterA, AddressingMode::RegisterD>, &CPU::ADD<AddressingMode::RegisterA, AddressingMode::RegisterE>, &CPU::ADD<AddressingMode::RegisterA, AddressingMode::RegisterH>, &CPU::ADD<AddressingMode::RegisterA, AddressingMode::RegisterL>, &CPU::ADD<AddressingMode::RegisterA, AddressingMode::AbsoluteHL>, &CPU::ADD<AddressingMode::RegisterA, AddressingMode::RegisterA>, &CPU::ADC<AddressingMode::RegisterA, AddressingMode::RegisterB>, &CPU::ADC<AddressingMode::RegisterA, AddressingMode::RegisterC>, &CPU::ADC<AddressingMode::RegisterA, AddressingMode::RegisterD>, &CPU::ADC<AddressingMode::RegisterA, AddressingMode::RegisterE>, &CPU::ADC<AddressingMode::RegisterA, AddressingMode::RegisterH>, &CPU::ADC<AddressingMode::RegisterA, AddressingMode::RegisterL>, &CPU::ADC<AddressingMode::RegisterA, AddressingMode::AbsoluteHL>, &CPU::ADC<AddressingMode::RegisterA, AddressingMode::RegisterA>,
		&CPU::SUB<AddressingMode::RegisterB>, &CPU::SUB<AddressingMode::RegisterC>, &CPU::SUB<AddressingMode::RegisterD>, &CPU::SUB<AddressingMode::RegisterE>, &CPU::SUB<AddressingMode::RegisterH>, &CPU::SUB<AddressingMode::RegisterL>, &CPU::SUB<AddressingMode::AbsoluteHL>, &CPU::SUB<AddressingMode::RegisterA>, &CPU::SBC<AddressingMode::RegisterA, AddressingMode::RegisterB>, &CPU::SBC<AddressingMode::RegisterA, AddressingMode::RegisterC>, &CPU::SBC<AddressingMode::RegisterA, AddressingMode::RegisterD>, &CPU::SBC<AddressingMode::RegisterA, AddressingMode::RegisterE>, &CPU::SBC<AddressingMode::RegisterA, AddressingMode::RegisterH>, &CPU::SBC<AddressingMode::RegisterA, AddressingMode::RegisterL>, &CPU::SBC<AddressingMode::RegisterA, AddressingMode::AbsoluteHL>, &CPU::SBC<AddressingMode::RegisterA, AddressingMode::RegisterA>,
		&CPU::AND<AddressingMode::RegisterB>, &CPU::AND<AddressingMode::RegisterC>, &CPU::AND<AddressingMode::RegisterD>, &CPU::AND<AddressingMode::RegisterE>, &CPU::AND<AddressingMode::RegisterH>, &CPU::AND<AddressingMode::RegisterL>, &CPU::AND<AddressingMode::AbsoluteHL>, &CPU::AND<AddressingMode::RegisterA>, &CPU::XOR<AddressingMode::RegisterB>, &CPU::XOR<AddressingMode::RegisterC>, &CPU::XOR<AddressingMode::RegisterD>, &CPU::XOR<AddressingMode::RegisterE>, &CPU::XOR<AddressingMode::RegisterH>, &CPU::XOR<AddressingMode::RegisterL>, &CPU::XOR<AddressingMode::AbsoluteHL>, &CPU::XOR<AddressingMode::RegisterA>,
		&CPU::OR<AddressingMode::RegisterB>, &CPU::OR<AddressingMode::RegisterC>, &CPU::OR<AddressingMode::RegisterD>, &CPU::OR<AddressingMode::RegisterE>, &CPU::OR<AddressingMode::RegisterH>, &CPU::OR<AddressingMode::RegisterL>, &CPU::OR<AddressingMode::AbsoluteHL>, &CPU::OR<AddressingMode::RegisterA>, &CPU::CP<AddressingMode::RegisterB>, &CPU::CP<AddressingMode::RegisterC>, &CPU::CP<AddressingMode::RegisterD>, &CPU::CP<AddressingMode::RegisterE>, &CPU::CP<AddressingMode::RegisterH>, &CPU::CP<AddressingMode::RegisterL>, &CPU::CP<AddressingMode::AbsoluteHL>, &CPU::CP<AddressingMode::RegisterA>,
		&CPU::RET<ConditionMode::NZ>, &CPU::POP<AddressingMode::RegisterBC>, &CPU::JP<ConditionMode::NZ, AddressingMode::Immediate16>, &CPU::JP<ConditionMode::Always, AddressingMode::Immediate16>, &CPU::CALL<ConditionMode::NZ, AddressingMode::Immediate16>, &CPU::PUSH<AddressingMode::RegisterBC>, &CPU::ADD<AddressingMode::RegisterA, AddressingMode::Immediate8>, &CPU::RST<0x00>, &CPU::RET<ConditionMode::Z>, &CPU::RET<ConditionMode::Always>, &CPU::JP<ConditionMode::Z, AddressingMode::Immediate16>, &CPU::undefined, &CPU::CALL<ConditionMode::Z, AddressingMode::Immediate16>, &CPU::CALL<ConditionMode::Always, AddressingMode::Immediate16>, &CPU::ADC<AddressingMode::RegisterA, AddressingMode::Immediate8>, &CPU::RST<0x08>,
		&CPU::RET<ConditionMode::NC>, &CPU::POP<AddressingMode::RegisterDE>, &CPU::JP<ConditionMode::NC, AddressingMode::Immediate16>, &CPU::undefined, &CPU::CALL<ConditionMode::NC, AddressingMode::Immediate16>, &CPU::PUSH<AddressingMode::RegisterDE>, &CPU::SUB<AddressingMode::Immediate8>, &CPU::RST<0x10>, &CPU::RET<ConditionMode::C>, &CPU::RETI, &CPU::JP<ConditionMode::C, AddressingMode::Immediate16>, &CPU::undefined, &CPU::CALL<ConditionMode::C, AddressingMode::Immediate16>, &CPU::undefined, &CPU::SBC<AddressingMode::RegisterA, AddressingMode::Immediate8>, &CPU::RST<0x18>,
		&CPU::LD<AddressingMode::ImmediateHighPage, AddressingMode::RegisterA>, &CPU::POP<AddressingMode::RegisterHL>, &CPU::LD<AddressingMode::AbsoluteHighPageC, AddressingMode::RegisterA>, &CPU::undefined, &CPU::undefined, &CPU::PUSH<AddressingMode::RegisterHL>, &CPU::AND<AddressingMode::Immediate8>, &CPU::RST<0x20>, &CPU::ADD<AddressingMode::RegisterSP, AddressingMode::Immediate8>, &CPU::JP<ConditionMode::Always, AddressingMode::AbsoluteHL>, &CPU::LD<AddressingMode::Absolute16, AddressingMode::RegisterA>, &CPU::undefined, &CPU::undefined, &CPU::undefined, &CPU::XOR<AddressingMode::Immediate8>, &CPU::RST<0x28>,
		&CPU::LD<AddressingMode::RegisterA, AddressingMode::ImmediateHighPage>, &CPU::POP<AddressingMode::RegisterAF>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::AbsoluteHighPageC>, &CPU::DI, &CPU::undefined, &CPU::PUSH<AddressingMode::RegisterAF>, &CPU::OR<AddressingMode::Immediate8>, &CPU::RST<0x30>, &CPU::LD<AddressingMode::RegisterHL, AddressingMode::StackPlusImmediate>, &CPU::LD<AddressingMode::RegisterSP, AddressingMode::RegisterHL>, &CPU::LD<AddressingMode::RegisterA, AddressingMode::Absolute16>, &CPU::EI, &CPU::undefined, &CPU::undefined, &CPU::CP<AddressingMode::Immediate8>, &CPU::RST<0x38>,
	};

	instruction_cycles = {
		4, 12, 8, 8, 4, 4, 8, 4, 20, 8, 8, 8, 4, 4, 8, 4,
		4, 12, 8, 8, 4, 4, 8, 4, 12, 8, 8, 8, 4, 4, 8, 4,
		8, 12, 8, 8, 4, 4, 8, 4, 8, 8, 8, 8, 4, 4, 8, 4,
		8, 12, 8, 8, 12, 12, 12, 4, 8, 8, 8, 8, 4, 4, 8, 4,
		4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
		4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
		4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
		8, 8, 8, 8, 8, 8, 4, 8, 4, 4, 4, 4, 4, 4, 8, 4,
		4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
		4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
		4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
		4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
		8, 12, 12, 16, 12, 16, 8, 16, 8, 16, 12, 4, 12, 24, 8, 16,
		8, 12, 12, 1, 12, 16, 8, 16, 8, 16, 12, 1, 12, 1, 8, 16,
		12, 12, 8, 1, 1, 16, 8, 16, 16, 4, 16, 1, 1, 1, 8, 16,
		12, 12, 8, 4, 1, 16, 8, 16, 12, 8, 16, 4, 1, 1, 8, 16,
	};

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
			writeBus(--SP, PC >> 8);
			writeBus(--SP, PC & 0b11111111);
			PC = addr;
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

	return PC;
}

std::vector<std::string> CPU::getDisassembly(int lines)
{
	std::vector<std::string> disasm;
	uint16_t pc_tmp = PC;
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
			s << "$" << std::setfill('0') << std::setw(4) << std::hex << (int8_t)debug_instruction.param8 + PC;
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
	PC = pc_tmp;
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
	registers.push_back(registerString(BC.B, "BC.B") + registerString(BC.C, "BC.C"));
	registers.push_back(registerString(DE.D, "DE.D") + registerString(DE.E, "DE.E"));
	registers.push_back(registerString(HL.H, "HL.H") + registerString(HL.L, "HL.L"));
	registers.push_back(registerString(AF.A, "AF.A") + registerString(AF.F.reg, "f"));
	s << "SP: $" << std::setfill('0') << std::setw(4) << std::hex << SP << "        PC: $" << std::setfill('0') << std::setw(4) << std::hex << PC;
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
	stopped = false;
	PC = 0x0000;
	writeBus(0xFF50, 0);
}

void CPU::fetch(bool debug) {
	uint16_t addr = PC;
	uint8_t opcode = readBus(PC++);
	Instruction* i;
	if (debug) {
		i = &debug_instruction;
	}
	else {
		i = &current_instruction;
	}
	*i = glossary[opcode];
	if (opcode == 0xCB) {
		uint8_t cb_opcode = readBus(PC++);
		*i = cb_glossary[cb_opcode];
		i->param8 = cb_opcode;
	}
	else {
		if (i->bytes == 2) {
			i->param8 = readBus(PC++);
		}
		else if (i->bytes == 3) {
			i->param16 = readBus(PC++) | (readBus(PC++) << 8);
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
		BC.C = ins.param16 & 0xFF;
		BC.B = ins.param16 >> 8;
		break;
	case 2:
		writeBus((BC.B << 8) + BC.C, AF.A);
		break;
	case 3:
		BC.C++;
		if (BC.C == 0)
		{
			BC.B++;
		}
		break;
	case 4:
		////inc\(&BC.B);
		break;
	case 5:
		break;
	case 6:
		BC.B = ins.param8;
		break;
	case 7:
		AF.F.Z = 0;
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = AF.A & 0b10000000;
		AF.A = AF.A << 1;
		AF.A = AF.A | AF.F.C;
		break;
	case 8:
		writeBus(ins.param16, (SP >> 8));
		writeBus(ins.param16 + 1, SP);
		break;
	case 9:
	{
		tmp = HL.H & 0b00001000;
		AF.F.N = 0;
		uint16_t hl = (HL.H << 8) | HL.L;
		uint16_t bc = (BC.B << 8) | BC.C;
		uint16_t res = hl + bc;
		HL.H = (res & 0xFF00) >> 8;
		HL.L = res & 0xFF;
		if (hl + bc > 0xFFFF)
		{
			AF.F.C = 1;
		}
		else
		{
			AF.F.C = 0;
		}
		if ((hl & 0x0FFF) + bc > 0x0FFF)
		{
			AF.F.H = 1;
		}
		else
		{
			AF.F.H = 0;
		}
	}
	break;
	case 10:
		AF.A = readBus((BC.B << 8) + BC.C);
		break;
	case 11:
		BC.C--;
		if (BC.C == 255)
		{
			BC.B--;
		}
		break;
	case 12:
		//inc\(&BC.C);
		break;
	case 13:
		//dec(&BC.C);
		break;
	case 14:
		BC.C = ins.param8;
		break;
	case 15:
		AF.F.Z = 0;
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = AF.A & 0b00000001;
		AF.A = AF.A >> 1;
		AF.A = AF.A | (AF.F.C << 7);
		break;
	case 16:
		stopped = true;
		break;
	case 17:
		DE.E = ins.param16 & 0xFF;
		DE.D = ins.param16 >> 8;
		break;
	case 18:
		writeBus((DE.D << 8) + DE.E, AF.A);
		break;
	case 19:
		DE.E++;
		if (DE.E == 0)
		{
			DE.D++;
		}
		break;
	case 20:
		//inc\(&DE.D);
		break;
	case 21:
		//dec(&DE.D);
		break;
	case 22:
		DE.D = ins.param8;
		break;
	case 23:
		tmp = AF.F.C;
		AF.F.Z = 0;
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = AF.A & 0b10000000;
		AF.A = AF.A << 1;
		AF.A = AF.A | tmp;
		break;
	case 24:
		PC += (int8_t)ins.param8;
		break;
	case 25:
	{
		tmp = HL.H & 0b00001000;
		AF.F.N = 0;
		uint16_t hl = (HL.H << 8) | HL.L;
		uint16_t de = (BC.B << 8) | BC.C;
		uint16_t res = hl + de;
		HL.H = (res & 0xFF00) >> 8;
		HL.L = res & 0xFF;
		if (hl + de > 0xFFFF)
		{
			AF.F.C = 1;
		}
		else
		{
			AF.F.C = 0;
		}
		if ((hl & 0x0FFF) + de > 0x0FFF)
		{
			AF.F.H = 1;
		}
		else
		{
			AF.F.H = 0;
		}
	}
	break;
	case 26:
		AF.A = readBus((DE.D << 8) + DE.E);
		break;
	case 27:
		DE.E--;
		if (DE.E == -1)
		{
			DE.E = 255;
			DE.D--;
			if (DE.D == -1)
			{
				DE.D = 255;
			}
		}
		break;
	case 28:
		//inc\(&DE.E);
		break;
	case 29:
		//dec(&DE.E);
		break;
	case 30:
		DE.E = ins.param8;
		break;
	case 31:
		tmp = AF.F.C;
		AF.F.Z = 0;
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = AF.A & 0b00000001;
		AF.A = AF.A >> 1;
		AF.A = AF.A | (tmp << 7);
		break;
	case 32:
		if (AF.F.Z == 0)
		{
			PC += (int8_t)ins.param8;
			ins.cycles += 4;
		}
		break;
	case 33:
		HL.L = ins.param16 & 0xFF;
		HL.H = ins.param16 >> 8;
		break;
	case 34:
		writeBus((HL.H << 8) + HL.L, AF.A);
		HL.L++;
		if (HL.L == 0)
		{
			HL.H++;
		}
		break;
	case 35:
		HL.L++;
		if (HL.L == 0)
		{
			HL.H++;
		}
		break;
	case 36:
		//inc\(&HL.H);
		break;
	case 37:
		//dec(&HL.H);
		break;
	case 38:
		HL.H = ins.param8;
		break;
	case 39:
		break;
	case 40:
		if (AF.F.Z == 1)
		{
			PC += (int8_t)ins.param8;
			ins.cycles += 4;
		}
		break;
	case 41:
	{
		tmp = HL.H & 0b00001000;
		AF.F.N = 0;
		uint16_t hl = (HL.H << 8) | HL.L;
		uint16_t res = hl + hl;
		HL.H = (res & 0xFF00) >> 8;
		HL.L = res & 0xFF;
		if (hl + hl > 0xFFFF)
		{
			AF.F.C = 1;
		}
		else
		{
			AF.F.C = 0;
		}
		if ((hl & 0x0FFF) + hl > 0x0FFF)
		{
			AF.F.H = 1;
		}
		else
		{
			AF.F.H = 0;
		}
	}
		break;
	case 42:
		AF.A = readBus((HL.H << 8) + HL.L);
		HL.L++;
		if (HL.L == 0)
		{
			HL.H++;
		}
		break;
	case 43:
		HL.L--;
		if (HL.L == 255)
		{
			HL.H--;
		}
		break;
	case 44:
		//inc\(&HL.L);
		break;
	case 45:
		//dec(&HL.L);
		break;
	case 46:
		HL.L = ins.param8;
		break;
	case 47:
		AF.A = AF.A ^ AF.A;
		AF.F.N = 1;
		AF.F.H = 1;
		break;
	case 48:
		if (AF.F.C == 0)
		{
			PC += (int8_t)ins.param8;
			ins.cycles += 4;
		}
		break;
	case 49:
		SP = ins.param16;
		break;
	case 50:
		writeBus((HL.H << 8) + HL.L, AF.A);
		HL.L--;
		if (HL.L == 255)
		{
			HL.H--;
		}
		break;
	case 51:
		SP++;
		break;
	case 52:
		//incMem((HL.H << 8) + HL.L);
		break;
	case 53:
		//decMem((HL.H << 8) + HL.L);
		break;
	case 54:
		writeBus((HL.H << 8) + HL.L, ins.param8);
		break;
	case 55:
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = 1;
		break;
	case 56:
		if (AF.F.C == 1)
		{
			PC += (int8_t)ins.param8;
			ins.cycles += 4;
		}
		break;
	case 57:
	{
		tmp = HL.H & 0b00001000;
		AF.F.N = 0;
		uint16_t hl = (HL.H << 8) | HL.L;
		uint16_t res = hl + SP;
		HL.H = (res & 0xFF00) >> 8;
		HL.L = res & 0xFF;
		if (hl + SP > 0xFFFF)
		{
			AF.F.C = 1;
		}
		else
		{
			AF.F.C = 0;
		}
		if ((hl & 0x0FFF) + SP > 0x0FFF)
		{
			AF.F.H = 1;
		}
		else
		{
			AF.F.H = 0;
		}
	}
		break;
	case 58:
		AF.A = readBus((HL.H << 8) + HL.L);
		HL.L--;
		if (HL.L == 255)
		{
			HL.H--;
		}
		break;
	case 59:
		SP--;
		break;
	case 60:
		//inc\(&AF.A);
		break;
	case 61:
		//dec(&AF.A);
		break;
	case 62:
		AF.A = ins.param8;
		break;
	case 63:
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = 1 - AF.F.C;
		break;
	case 64:
		BC.B = BC.B;
		break;
	case 65:
		BC.B = BC.C;
		break;
	case 66:
		BC.B = DE.D;
		break;
	case 67:
		BC.B = DE.E;
		break;
	case 68:
		BC.B = HL.H;
		break;
	case 69:
		BC.B = HL.L;
		break;
	case 70:
		BC.B = readBus((HL.H << 8) + HL.L);
		break;
	case 71:
		BC.B = AF.A;
		break;
	case 72:
		BC.C = BC.B;
		break;
	case 73:
		BC.C = BC.C;
		break;
	case 74:
		BC.C = DE.D;
		break;
	case 75:
		BC.C = DE.E;
		break;
	case 76:
		BC.C = HL.H;
		break;
	case 77:
		BC.C = HL.L;
		break;
	case 78:
		BC.C = readBus((HL.H << 8) + HL.L);
		break;
	case 79:
		BC.C = AF.A;
		break;
	case 80:
		DE.D = BC.B;
		break;
	case 81:
		DE.D = BC.C;
		break;
	case 82:
		DE.D = DE.D;
		break;
	case 83:
		DE.D = DE.E;
		break;
	case 84:
		DE.D = HL.H;
		break;
	case 85:
		DE.D = HL.L;
		break;
	case 86:
		DE.D = readBus((HL.H << 8) + HL.L);
		break;
	case 87:
		DE.D = AF.A;
		break;
	case 88:
		DE.E = BC.B;
		break;
	case 89:
		DE.E = BC.C;
		break;
	case 90:
		DE.E = DE.D;
		break;
	case 91:
		DE.E = DE.E;
		break;
	case 92:
		DE.E = HL.H;
		break;
	case 93:
		DE.E = HL.L;
		break;
	case 94:
		DE.E = readBus((HL.H << 8) + HL.L);
		break;
	case 95:
		DE.E = AF.A;
		break;
	case 96:
		HL.H = BC.B;
		break;
	case 97:
		HL.H = BC.C;
		break;
	case 98:
		HL.H = DE.D;
		break;
	case 99:
		HL.H = DE.E;
		break;
	case 100:
		HL.H = HL.H;
		break;
	case 101:
		HL.H = HL.L;
		break;
	case 102:
		HL.H = readBus((HL.H << 8) + HL.L);
		break;
	case 103:
		HL.H = AF.A;
		break;
	case 104:
		HL.L = BC.B;
		break;
	case 105:
		HL.L = BC.C;
		break;
	case 106:
		HL.L = DE.D;
		break;
	case 107:
		HL.L = DE.E;
		break;
	case 108:
		HL.L = HL.H;
		break;
	case 109:
		HL.L = HL.L;
		break;
	case 110:
		HL.L = readBus((HL.H << 8) + HL.L);
		break;
	case 111:
		HL.L = AF.A;
		break;
	case 112:
		writeBus((HL.H << 8) + HL.L, BC.B);
		break;
	case 113:
		writeBus((HL.H << 8) + HL.L, BC.C);
		break;
	case 114:
		writeBus((HL.H << 8) + HL.L, DE.D);
		break;
	case 115:
		writeBus((HL.H << 8) + HL.L, DE.E);
		break;
	case 116:
		writeBus((HL.H << 8) + HL.L, HL.H);
		break;
	case 117:
		writeBus((HL.H << 8) + HL.L, HL.L);
		break;
	case 118:
		throw "Not implemented";
		break;
	case 119:
		writeBus((HL.H << 8) + HL.L, AF.A);
		break;
	case 120:
		AF.A = BC.B;
		break;
	case 121:
		AF.A = BC.C;
		break;
	case 122:
		AF.A = DE.D;
		break;
	case 123:
		AF.A = DE.E;
		break;
	case 124:
		AF.A = HL.H;
		break;
	case 125:
		AF.A = HL.L;
		break;
	case 126:
		AF.A = readBus((HL.H << 8) + HL.L);
		break;
	case 127:
		AF.A = AF.A;
		break;
	case 128:
		break;
	case 129:
		break;
	case 130:
		break;
	case 131:
		break;
	case 132:
		break;
	case 133:
		break;
	case 134:
		break;
	case 135:
		break;
	case 136:
		break;
	case 137:
		break;
	case 138:
		break;
	case 139:
		break;
	case 140:
		break;
	case 141:
		break;
	case 142:
		break;
	case 143:
		break;
	case 144:
		//sub(&AF.A, BC.B);
		break;
	case 145:
		//sub(&AF.A, BC.C);
		break;
	case 146:
		//sub(&AF.A, DE.D);
		break;
	case 147:
		//sub(&AF.A, DE.E);
		break;
	case 148:
		//sub(&AF.A, HL.H);
		break;
	case 149:
		//sub(&AF.A, HL.L);
		break;
	case 150:
		//sub(&AF.A, readBus((HL.H << 8) + HL.L));
		break;
	case 151:
		//sub(&AF.A, AF.A);
		break;
	case 152:
		//sub(&AF.A, BC.B + AF.F.C);
		break;
	case 153:
		//sub(&AF.A, BC.C + AF.F.C);
		break;
	case 154:
		//sub(&AF.A, DE.D + AF.F.C);
		break;
	case 155:
		//sub(&AF.A, DE.E + AF.F.C);
		break;
	case 156:
		//sub(&AF.A, HL.H + AF.F.C);
		break;
	case 157:
		//sub(&AF.A, HL.L + AF.F.C);
		break;
	case 158:
		//sub(&AF.A, readBus((HL.H << 8) + HL.L) + AF.F.C);
		break;
	case 159:
		//sub(&AF.A, AF.A + AF.F.C);
		break;
	case 160:
		AF.A = AF.A & BC.B;
		AF.F.Z = AF.A == 0 ? 1 : 0;
		AF.F.N = 0;
		AF.F.H = 1;
		AF.F.C = 0;
		break;
	case 161:
		AF.A = AF.A & BC.C;
		AF.F.Z = AF.A == 0 ? 1 : 0;
		AF.F.N = 0;
		AF.F.H = 1;
		AF.F.C = 0;
		break;
	case 162:
		AF.A = AF.A & DE.D;
		AF.F.Z = AF.A == 0 ? 1 : 0;
		AF.F.N = 0;
		AF.F.H = 1;
		AF.F.C = 0;
		break;
	case 163:
		AF.A = AF.A & DE.E;
		AF.F.Z = AF.A == 0 ? 1 : 0;
		AF.F.N = 0;
		AF.F.H = 1;
		AF.F.C = 0;
		break;
	case 164:
		AF.A = AF.A & HL.H;
		AF.F.Z = AF.A == 0 ? 1 : 0;
		AF.F.N = 0;
		AF.F.H = 1;
		AF.F.C = 0;
		break;
	case 165:
		AF.A = AF.A & HL.L;
		AF.F.Z = AF.A == 0 ? 1 : 0;
		AF.F.N = 0;
		AF.F.H = 1;
		AF.F.C = 0;
		break;
	case 166:
		AF.A = AF.A & readBus((HL.H << 8) + HL.L);
		AF.F.Z = AF.A == 0 ? 1 : 0;
		AF.F.N = 0;
		AF.F.H = 1;
		AF.F.C = 0;
		break;
	case 167:
		AF.A = AF.A & AF.A;
		AF.F.Z = AF.A == 0 ? 1 : 0;
		AF.F.N = 0;
		AF.F.H = 1;
		AF.F.C = 0;
		break;
	case 168:
		AF.A = AF.A ^ BC.B;
		AF.F.Z = AF.A == 0 ? 1 : 0;
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = 0;
		break;
	case 169:
		AF.A = AF.A ^ BC.C;
		AF.F.Z = AF.A == 0 ? 1 : 0;
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = 0;
		break;
	case 170:
		AF.A = AF.A ^ DE.D;
		AF.F.Z = AF.A == 0 ? 1 : 0;
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = 0;
		break;
	case 171:
		AF.A = AF.A ^ DE.E;
		AF.F.Z = AF.A == 0 ? 1 : 0;
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = 0;
		break;
	case 172:
		AF.A = AF.A ^ HL.H;
		AF.F.Z = AF.A == 0 ? 1 : 0;
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = 0;
		break;
	case 173:
		AF.A = AF.A ^ HL.L;
		AF.F.Z = AF.A == 0 ? 1 : 0;
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = 0;
		break;
	case 174:
		AF.A = AF.A ^ readBus((HL.H << 8) | HL.L);
		AF.F.Z = AF.A == 0 ? 1 : 0;
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = 0;
		break;
	case 175:
		AF.A = AF.A ^ AF.A;
		AF.F.Z = AF.A == 0 ? 1 : 0;
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = 0;
		break;
	case 176:
		AF.A = AF.A | BC.B;
		AF.F.Z = AF.A == 0 ? 1 : 0;
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = 0;
		break;
	case 177:
		AF.A = AF.A | BC.C;
		AF.F.Z = AF.A == 0 ? 1 : 0;
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = 0;
		break;
	case 178:
		AF.A = AF.A | DE.D;
		AF.F.Z = AF.A == 0 ? 1 : 0;
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = 0;
		break;
	case 179:
		AF.A = AF.A | DE.E;
		AF.F.Z = AF.A == 0 ? 1 : 0;
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = 0;
		break;
	case 180:
		AF.A = AF.A | HL.H;
		AF.F.Z = AF.A == 0 ? 1 : 0;
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = 0;
		break;
	case 181:
		AF.A = AF.A | HL.L;
		AF.F.Z = AF.A == 0 ? 1 : 0;
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = 0;
		break;
	case 182:
		AF.A = AF.A | readBus((HL.H << 8) + HL.L);
		AF.F.Z = AF.A == 0 ? 1 : 0;
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = 0;
		break;
	case 183:
		AF.A = AF.A | AF.A;
		AF.F.Z = AF.A == 0 ? 1 : 0;
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = 0;
		break;
	case 184:
		//cp(BC.B);
		break;
	case 185:
		//cp(BC.C);
		break;
	case 186:
		//cp(DE.D);
		break;
	case 187:
		//cp(DE.E);
		break;
	case 188:
		//cp(HL.H);
		break;
	case 189:
		//cp(HL.L);
		break;
	case 190:
		//cp(readBus((HL.H << 8) + HL.L));
		break;
	case 191:
		//cp(AF.A);
		break;
	case 192:
		if (AF.F.Z == 0)
		{
			PC = readBus(SP++);
			PC |= readBus(SP++) << 8;
			ins.cycles += 12;
		}
		break;
	case 193:
		BC.C = readBus(SP++);
		BC.B = readBus(SP++);
		break;
	case 194:
		if (AF.F.Z == 0)
		{
			PC = ins.param16;
			ins.cycles += 4;
		}
		break;
	case 195:
		PC = ins.param16;
		break;
	case 196:
		if (AF.F.Z == 0)
		{
			writeBus(--SP, PC >> 8);
			writeBus(--SP, PC & 0b11111111);
			PC = ins.param16;
			ins.cycles += 12;
		}
		break;
	case 197:
		writeBus(--SP, BC.B);
		writeBus(--SP, BC.C);
		break;
	case 198:
		break;
	case 199:
		writeBus(--SP, PC >> 8);
		writeBus(--SP, PC & 0b11111111);
		PC = 0x0000;
		break;
	case 200:
		if (AF.F.Z == 1)
		{
			PC = readBus(SP++);
			PC |= readBus(SP++) << 8;
			ins.cycles += 12;
		}
		break;
	case 201:
		PC = readBus(SP++);
		PC |= readBus(SP++) << 8;
		break;
	case 202:
		if (AF.F.Z == 1)
		{
			PC = ins.param16;
			ins.cycles += 4;
		}
		break;
	case 203: // CB PREFIX
		cb(ins.param8);
		break;
	case 204:
		if (AF.F.Z == 1)
		{
			writeBus(--SP, PC >> 8);
			writeBus(--SP, PC & 0b11111111);
			PC = ins.param16;
			ins.cycles += 12;
		}
		break;
	case 205:
		writeBus(--SP, PC >> 8);
		writeBus(--SP, PC & 0b11111111);
		PC = ins.param16;
		break;
	case 206:
		break;
	case 207:
		writeBus(--SP, PC >> 8);
		writeBus(--SP, PC & 0b11111111);
		PC = 0x0008;
		break;
	case 208:
		if (AF.F.C == 0)
		{
			PC = readBus(SP++);
			PC |= readBus(SP++) << 8;
			ins.cycles += 12;
		}
		break;
	case 209:
		DE.E = readBus(SP++);
		DE.D = readBus(SP++);
		break;
	case 210:
		if (AF.F.C == 0)
		{
			PC = ins.param16;
			ins.cycles += 4;
		}
		break;
	case 211:
		throw "Invalid opcode";
		break;
	case 212:
		if (AF.F.C == 0)
		{
			writeBus(--SP, PC >> 8);
			writeBus(--SP, PC & 0b11111111);
			PC = ins.param16;
			ins.cycles += 12;
		}
		break;
	case 213:
		writeBus(--SP, DE.D);
		writeBus(--SP, DE.E);
		break;
	case 214:
		//sub(&AF.A, ins.param8);
		break;
	case 215:
		writeBus(--SP, PC >> 8);
		writeBus(--SP, PC & 0b11111111);
		PC = 0x0010;
		break;
	case 216:
		if (AF.F.C == 1)
		{
			PC = readBus(SP++);
			PC |= readBus(SP++) << 8;
			ins.cycles += 12;
		}
		break;
	case 217:
		ime = true;
		PC = readBus(SP++);
		PC |= readBus(SP++) << 8;
		break;
	case 218:
		if (AF.F.C == 1)
		{
			PC = ins.param16;
			ins.cycles += 4;
		}
		break;
	case 219:
		throw "Invalid opcode";
		break;
	case 220:
		if (AF.F.C == 1)
		{
			writeBus(SP--, PC >> 8);
			writeBus(SP--, PC & 0b11111111);
			PC = ins.param16;
			ins.cycles += 12;
		}
		break;
	case 221:
		throw "Invalid opcode";
		break;
	case 222:
		//sub(&AF.A, ins.param8 + AF.F.C);
		break;
	case 223:
		writeBus(--SP, PC >> 8);
		writeBus(--SP, PC & 0b11111111);
		PC = 0x0018;
		break;
	case 224:
		writeBus(0xFF00 + ins.param8, AF.A);
		break;
	case 225:
		HL.L = readBus(SP++);
		HL.H = readBus(SP++);
		break;
	case 226:
		writeBus(0xFF00 + BC.C, AF.A);
		break;
	case 227:
		throw "Invalid opcode";
		break;
	case 228:
		throw "Invalid opcode";
		break;
	case 229:
		writeBus(--SP, HL.H);
		writeBus(--SP, HL.L);
		break;
	case 230:
		AF.A = AF.A & ins.param8;
		AF.F.Z = AF.A == 0 ? 1 : 0;
		AF.F.N = 0;
		AF.F.H = 1;
		AF.F.C = 0;
		break;
	case 231:
		writeBus(--SP, PC >> 8);
		writeBus(--SP, PC & 0b11111111);
		PC = 0x0020;
		break;
	case 232:
		AF.F.Z = 0;
		AF.F.N = 0;
		if ((SP & 0x0FFF) + ins.param8 > 0x0FFF)
		{
			AF.F.H = 1;
		}
		else
		{
			AF.F.H = 0;
		}
		SP += ins.param8; 
		if (SP < ins.param8)
		{
			AF.F.C = 1;
		}
		else
		{
			AF.F.C = 0;
		}
		break;
	case 233:
		PC = HL.L;
		PC |= HL.H << 8;
		break;
	case 234:
		writeBus(ins.param16, AF.A);
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
		AF.A = AF.A ^ ins.param8;
		AF.F.Z = AF.A == 0 ? 1 : 0;
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = 0;
		break;
	case 239:
		writeBus(--SP, PC >> 8);
		writeBus(--SP, PC & 0b11111111);
		PC = 0x0028;
		break;
	case 240:
		AF.A = readBus(0xFF00 | ins.param8);
		break;
	case 241:
		AF.F.reg = readBus(SP++);
		AF.F.reg &= 0xF0;
		AF.A = readBus(SP++);
		break;
	case 242:
		AF.A = readBus(0xFF00 | BC.C);
		break;
	case 243:
		ime = false;
		break;
	case 244:
		throw "Invalid opcode";
		break;
	case 245:
		writeBus(--SP, AF.A);
		writeBus(--SP, AF.F.reg);
		break;
	case 246:
		AF.A = AF.A | ins.param8;
		AF.F.Z = AF.A == 0 ? 1 : 0;
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = 0;
		break;
	case 247:
		writeBus(--SP, PC >> 8);
		writeBus(--SP, PC & 0b11111111);
		PC = 0x0030;
		break;
	case 248:
	{
		uint16_t init = SP;
		uint16_t res = SP + (int8_t)ins.param8;
		HL.H = res >> 8;
		HL.L = res & 0xFF;
		AF.F.Z = 0;
		AF.F.H = (init & 0x0FFF) + (int8_t)ins.param8 > 0x0FFF;
		AF.F.N = 0;
		AF.F.C = (init & 0x8000) && !(res & 0x8000);
	}
		break;
	case 249:
		SP = HL.L;
		SP |= HL.H << 8;
		break;
	case 250:
		AF.A = readBus(ins.param16);
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
		//cp(ins.param8);
		break;
	case 255:
		writeBus(--SP, PC >> 8);
		writeBus(--SP, PC & 0b11111111);
		PC = 0x0038;
		break;
	}
}

void CPU::cb(uint8_t opcode)
{
	uint8_t bit = (opcode & 0b00111000) >> 3;

	uint8_t data;
	switch (opcode & 0b111) {
	case 0:
		data = BC.B;
		break;
	case 1:
		data = BC.C;
		break;
	case 2:
		data = DE.D;
		break;
	case 3:
		data = DE.E;
		break;
	case 4:
		data = HL.H;
		break;
	case 5:
		data = HL.L;
		break;
	case 6:
		data = readBus((HL.H << 8) + HL.L);
		break;
	case 7:
		data = AF.A;
		break;
	}
	int tmp;
	if ((opcode & 0b11111000) == 0b00000000) { // RLC
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = data & 0b10000000;
		data = data << 1;
		data = data | AF.F.C;
		AF.F.Z = data == 0;
	}
	else if ((opcode & 0b11111000) == 0b00001000) { // RRC
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = data & 0b00000001;
		data = data >> 1;
		data = data | (AF.F.C << 7);
		AF.F.Z = data == 0;
	}
	else if ((opcode & 0b11111000) == 0b00010000) { // RL
		tmp = AF.F.C;
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = data & 0b10000000;
		data = data << 1;
		data = data | tmp;
		AF.F.Z = data == 0;
	}
	else if ((opcode & 0b11111000) == 0b00011000) { // RR
		tmp = AF.F.C;
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = data & 0b00000001;
		data = data >> 1;
		data = data | (tmp << 7);
		AF.F.Z = data == 0;
	}
	else if ((opcode & 0b11111000) == 0b00100000) { // SLA
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = data & 0b10000000;
		data = data << 1;
		AF.F.Z = data == 0;
	}
	else if ((opcode & 0b11111000) == 0b00101000) { // SRA
		tmp = data & 0b10000000;
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = data & 0b00000001;
		data = data >> 1;
		data = data | tmp;
		AF.F.Z = data == 0;
	}
	else if ((opcode & 0b11111000) == 0b00110000) { // SWAP
		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = 0;
		tmp = data & 0b00001111;
		data = data >> 4;
		data = data | (tmp << 4);
		AF.F.Z = data == 0;
	}
	else if ((opcode & 0b11111000) == 0b00111000) { // SRL

		AF.F.N = 0;
		AF.F.H = 0;
		AF.F.C = data & 0b00000001;
		data = data >> 1;
		AF.F.Z = data == 0;
	}
	else if ((opcode & 0b11000000) == 0b01000000) { // BIT
		AF.F.N = 0;
		AF.F.H = 1;
		AF.F.Z = data & (1 << bit);
		AF.F.Z = 1 - AF.F.Z;
	}
	else if ((opcode & 0b11000000) == 0b10000000) { // RES
		data = data & (~(1 << bit));
	}
	else if ((opcode & 0b11000000) == 0b11000000) { // SET
		data = data | (1 << bit);
	}

	switch (opcode & 0b111) {
	case 0:
		BC.B = data;
		break;
	case 1:
		BC.C = data;
		break;
	case 2:
		DE.D = data;
		break;
	case 3:
		DE.E = data;
		break;
	case 4:
		HL.H = data;
		break;
	case 5:
		HL.L = data;
		break;
	case 6:
		writeBus((HL.H << 8) + HL.L, data);
		break;
	case 7:
		AF.A = data;
		break;
	}
}
