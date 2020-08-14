#include "APU.h"

uint8_t APU::getSoundValue()
{
	if (!NR52.allsound_on) {
		return 0;
	}
	else {
		return 1;
	}
}

void APU::divFlipped(uint8_t bit)
{
	if (bit >= 15) {
		// 64 Hz
		if (NR12.sweep && NR52.sound1_on) {
			channel1_envelope_count++;
			if (channel1_envelope_count >= NR12.sweep) {
				channel1_envelope_count = 0;
				if (NR12.direction) {
					if (NR12.volume != 0x0F) {
						NR12.volume++;
					}
				}
				else {
					if (NR12.volume != 0x00) {
						NR12.volume--;
					}
					if (NR12.volume == 0x00) {
						NR52.sound1_on = false;
					}
				}
			}
		}
		if (NR22.sweep && NR52.sound2_on) {
			channel2_envelope_count++;
			if (channel2_envelope_count >= NR22.sweep) {
				channel2_envelope_count = 0;
				if (NR22.direction) {
					if (NR22.volume != 0x0F) {
						NR22.volume++;
					}
				}
				else {
					if (NR22.volume != 0x00) {
						NR22.volume--;
					}
					if (NR22.volume == 0x00) {
						NR52.sound2_on = false;
					}
				}
			}
		}
		if (NR42.sweep && NR52.sound4_on) {
			channel4_envelope_count++;
			if (channel4_envelope_count >= NR42.sweep) {
				channel4_envelope_count = 0;
				if (NR42.direction) {
					if (NR42.volume != 0x0F) {
						NR42.volume++;
					}
				}
				else {
					if (NR42.volume != 0x00) {
						NR42.volume--;
					}
					if (NR42.volume == 0x00) {
						NR52.sound4_on = false;
					}
				}
			}
		}
	}
	if (bit >= 14) {
		// 128 Hz
		if (NR10.sweep_shift && NR52.sound1_on) {
			if (channel1_sweep_count >= NR10.sweep_time) {
				channel1_sweep_count = 0;
				uint16_t freq = (NR14.frequency << 8) | NR13;
				freq += NR10.increase_decrease ? freq/(1 << NR10.sweep_shift) : -freq/(1 << NR10.sweep_shift);
				NR13 = freq & 0xFF;
				NR14.frequency = freq >> 8;
			}
			channel1_sweep_count++;
		}
	}
	if (bit >= 13) {
		// 256 Hz
		if (NR14.counter) {
			NR11.sound_length++;
			if (NR11.sound_length == 0) {
				NR52.sound1_on = false;
			}
		}
		if (NR24.counter) {
			NR21.length++;
			if (NR21.length == 0) {
				NR52.sound2_on = false;
			}
		}
		if (NR34.counter) {
			NR31++;
			if (NR31 == 0) {
				NR52.sound3_on = false;
			}
		}
		if (NR44.counter) {
			NR41.length++;
			if (NR41.length == 0) {
				NR52.sound4_on = false;
			}
		}
	}
}

uint8_t APU::shift_channel4_reg() {
	uint8_t bit = ((noise_shift_reg >> 0) ^ (noise_shift_reg >> 1)) & 0x01;
	noise_shift_reg = (noise_shift_reg >> 1) | (bit << 15);
	return bit;
}
