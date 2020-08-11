#include "Bus.h"
#include "CPU.h"

#include <sstream>
#include <iomanip>

Bus::Bus(std::shared_ptr<PPU> ppu, std::shared_ptr<APU> apu) : ppu(ppu), apu(apu)
{
	
}

uint8_t Bus::read(uint16_t addr)
{
	if (0x0000 <= addr && addr < 0x8000) {
		if (0x0000 <= addr && addr < 0x0100) {
			if (bootRomControl) {
				return cart->read(addr);
			}
			else {
				return bootrom[addr];
			}
		}
		else {
			return cart->read(addr);
		}
	}
	else if (0x8000 <= addr && addr < 0xA000) {
		return ram[addr];
	}
	else if (0xA000 <= addr && addr < 0xC000) {
		return cart->read(addr);
	}
	else if (0xC000 <= addr && addr < 0xE000) {
		return ram[addr];
	}
	else if (0xE000 <= addr && addr < 0xFE00) {
		return ram[addr - 0x2000];
	}
	else if (0xFE00 <= addr && addr < 0xFEA0) {
		return ram[addr];
	}
	else if (0xFEA0 <= addr && addr < 0xFF00) {
		return 0xFF;
	}
	else if (0xFF00 <= addr && addr < 0xFF80) {
		if (addr == 0xFF00) {
			if (!(joypad & 0x20)) {
				return joypad & buttons.reg;
			}
			else if (!(joypad & 0x10)) {
				return joypad & dpad.reg;
			}
			else {
				return 0xFF;
			}
		}
		else if (addr == 0xFF04) {
			return cpu->counter >> 8;
		}
		else if (addr == 0xFF05) {
			return cpu->TIMA;
		}
		else if (addr == 0xFF06) {
			return cpu->TMA;
		}
		else if (addr == 0xFF07) {
			return cpu->TAC.reg;
		}
		else if (addr == 0xFF0F) {
			return cpu->IF.reg;
		}
		else if (addr == 0xFF10) {
			return apu->NR10.reg | 0x80;
		}
		else if (addr == 0xFF11) {
			return apu->NR11.reg | 0x3F;
		}
		else if (addr == 0xFF12) {
			return apu->NR12.reg;
		}
		else if (addr == 0xFF13) {
			return 0xFF;
		}
		else if (addr == 0xFF14) {
			return apu->NR14.reg | 0xBF;
		}
		else if (addr == 0xFF16) {
			return apu->NR21.reg | 0x3F;
		}
		else if (addr == 0xFF17) {
			return apu->NR22.reg;
		}
		else if (addr == 0xFF18) {
			return 0xFF;
		}
		else if (addr == 0xFF19) {
			return apu->NR24.reg | 0xBF;
		}
		else if (addr == 0xFF1A) {
			return apu->NR30.reg | 0x7F;
		}
		else if (addr == 0xFF1B) {
			return 0xFF;
		}
		else if (addr == 0xFF1C) {
			return apu->NR32.reg | 0x9F;
		}
		else if (addr == 0xFF1D) {
			return 0xFF;
		}
		else if (addr == 0xFF1E) {
			return apu->NR34.reg | 0xBF;
		}
		else if (addr == 0xFF20) {
			return 0xFF;
		}
		else if (addr == 0xFF21) {
			return apu->NR42.reg;
		}
		else if (addr == 0xFF22) {
			return apu->NR43.reg;
		}
		else if (addr == 0xFF23) {
			return apu->NR44.reg | 0xBF;
		}
		else if (addr == 0xFF24) {
			return apu->NR50.reg;
		}
		else if (addr == 0xFF25) {
			return apu->NR51.reg;
		}
		else if (addr == 0xFF26) {
			return apu->NR52.reg | 0x70;
		}
		else if (0xFF30 <= addr && addr < 0xFF40) {
			return apu->waveram[addr & 0x0F];
		}
		else if (addr == 0xFF40) {
			return ppu->LCDC.reg;
		}
		else if (addr == 0xFF41) {
			return ppu->STAT.reg;
		}
		else if (addr == 0xFF42) {
			return ppu->SCY;
		}
		else if (addr == 0xFF43) {
			return ppu->SCX;
		}
		else if (addr == 0xFF44) {
			return ppu->LY;
		}
		else if (addr == 0xFF45) {
			return ppu->LYC;
		}
		else if (addr == 0xFF47) {
			return ppu->BGP.reg;
		}
		else if (addr == 0xFF48) {
			return ppu->OBP0.reg;
		}
		else if (addr == 0xFF49) {
			return ppu->OBP1.reg;
		}
		else {
			return 0xFF;
		}
	}
	else if (0xFF80 <= addr && addr < 0xFFFF) {
	    return ram[addr];
	}
	else if (addr == 0xFFFF) {
		return cpu->IE.reg;
	}
}

void Bus::write(uint16_t addr, uint8_t data)
{
	if (0x0000 <= addr && addr < 0x8000) {
		cart->write(addr, data);
	}
	else if (0x8000 <= addr && addr < 0xA000) {
		ram[addr] = data;
	}
	else if (0xA000 <= addr && addr < 0xC000) {
		return cart->write(addr, data);
	}
	else if (0xC000 <= addr && addr < 0xE000) {
		ram[addr] = data;
	}
	else if (0xE000 <= addr && addr < 0xFE00) {
		ram[addr - 0x2000] = data;
	}
	else if (0xFE00 <= addr && addr < 0xFEA0) {
		ram[addr] = data;
	}
	else if (0xFEA0 <= addr && addr < 0xFF00) {
		return;
	}
	else if (0xFF00 <= addr && addr < 0xFF80) {
		if (addr == 0xFF00) {
			joypad = data | 0xF;
		}
		else if (addr == 0xFF04) {
			cpu->counter &= 0xFF;
		}
		else if (addr == 0xFF05) {
			cpu->TIMA = data;
		}
		else if (addr == 0xFF06) {
			cpu->TMA = data;
		}
		else if (addr == 0xFF07) {
			cpu->TAC.reg = data;
		}
		else if (addr == 0xFF0F) {
			cpu->IF.reg = data;
		}
		else if (0xFF10 <= addr && addr < 0xFF26) {
			if (!apu->NR52.allsound_on) {
				return;
			}
			if (addr == 0xFF10) {
				apu->NR10.reg = data;
			}
			else if (addr == 0xFF11) {
				apu->NR11.reg = data;
			}
			else if (addr == 0xFF12) {
				apu->NR12.reg = data;
			}
			else if (addr == 0xFF13) {
				apu->NR13 = data;
			}
			else if (addr == 0xFF14) {
				apu->NR14.reg = data;
				if (apu->NR14.initial) {
					apu->channel1_reset = true;
				}
			}
			else if (addr == 0xFF16) {
				apu->NR21.reg = data;
			}
			else if (addr == 0xFF17) {
				apu->NR22.reg = data;
			}
			else if (addr == 0xFF18) {
				apu->NR23 = data;
			}
			else if (addr == 0xFF19) {
				apu->NR24.reg = data;
				if (apu->NR24.initial) {
					apu->channel2_reset = true;
				}
			}
			else if (addr == 0xFF1A) {
				apu->NR30.reg = data;
			}
			else if (addr == 0xFF1B) {
				apu->NR31 = data;
			}
			else if (addr == 0xFF1C) {
				apu->NR32.reg = data;
			}
			else if (addr == 0xFF1D) {
				apu->NR33 = data;
			}
			else if (addr == 0xFF1E) {
				apu->NR34.reg = data;
				if (apu->NR34.initial) {
					apu->channel3_reset = true;
				}
			}
			else if (addr == 0xFF20) {
				apu->NR41.reg = data;
			}
			else if (addr == 0xFF21) {
				apu->NR42.reg = data;
			}
			else if (addr == 0xFF22) {
				apu->NR43.reg = data;
			}
			else if (addr == 0xFF23) {
				apu->NR44.reg = data;
				if (apu->NR44.initial) {
					apu->channel4_reset = true;
				}
			}
			else if (addr == 0xFF24) {
				apu->NR50.reg = data;
			}
			else if (addr == 0xFF25) {
				apu->NR51.reg = data;
			}
		}
		else if (addr == 0xFF26) {
			apu->NR52.allsound_on = data & 0x80;
			if (!(data & 0x80)) {
				apu->NR10.reg = 0;
				apu->NR11.reg = 0;
				apu->NR12.reg = 0;
				apu->NR13 = 0;
				apu->NR14.reg = 0;
				apu->NR21.reg = 0;
				apu->NR22.reg = 0;
				apu->NR23 = 0;
				apu->NR24.reg = 0;
				apu->NR30.reg = 0;
				apu->NR31 = 0;
				apu->NR32.reg = 0;
				apu->NR33 = 0;
				apu->NR34.reg = 0;
				//apu->NR41.reg = 0;
				apu->NR42.reg = 0;
				apu->NR43.reg = 0;
				apu->NR44.reg = 0;
				apu->NR50.reg = 0;
				apu->NR51.reg = 0;
			}
		}
		else if (0xFF30 <= addr && addr < 0xFF40) {
			apu->waveram[addr & 0x0F] = data;
		}
		else if (addr == 0xFF40) {
			ppu->LCDC.reg = data;
		}
		else if (addr == 0xFF41) {
			ppu->STAT.reg = data;
		}
		else if (addr == 0xFF42) {
			ppu->SCY = data;
		}
		else if (addr == 0xFF43) {
			ppu->SCX = data;
		}
		else if (addr == 0xFF44) {
			ppu->LY = data;
		}
		else if (addr == 0xFF45) {
			ppu->LYC = data;
		}
		else if (addr == 0xFF46) {
			memcpy(ram + (data << 8), ram + 0xFE00, 160);
		}
		else if (addr == 0xFF47) {
			ppu->BGP.reg = data;
		}
		else if (addr == 0xFF48) {
			ppu->OBP0.reg = data;
		}
		else if (addr == 0xFF49) {
			ppu->OBP1.reg = data;
		}
		else if (addr == 0xFF50) {
			bootRomControl = data;
		}
	}
	else if (0xFF80 <= addr && addr < 0xFFFF) {
	    ram[addr] = data;
    }
	else if (addr == 0xFFFF) {
		cpu->IE.reg = data;
	}
}

std::vector<std::string> Bus::getMemory(uint16_t addr)
{
	std::vector<std::string> mem;
	int lines = 16;
	for (int i = 0; i < lines; i++) {
		std::stringstream s;
		s << "$" << std::setfill('0') << std::setw(4) << std::hex << addr << ": ";
		for (int j = 0; j < 16; j++) {
			s << " " << std::setw(2) << (int)ram[addr];
			addr++;
		}
		mem.push_back(s.str());
	}
	return mem;
}

void Bus::button(GBButton button, bool pressed)
{
	if (pressed) {
		cpu->interrupt(0x60);
	}
	switch (button) {
	case GBButton::Up:
		dpad.up = !pressed;
		break;
	case GBButton::Down:
		dpad.down = !pressed;
		break;
	case GBButton::Left:
		dpad.left = !pressed;
		break;
	case GBButton::Right:
		dpad.right = !pressed;
		break;
	case GBButton::B:
		buttons.b = !pressed;
		break;
	case GBButton::A:
		buttons.a = !pressed;
		break;
	case GBButton::Select:
		buttons.select = !pressed;
		break;
	case GBButton::Start:
		buttons.start = !pressed;
		break;
	}
}

uint8_t* Bus::getOamPointer()
{
	return ram+0xFE00;
}
