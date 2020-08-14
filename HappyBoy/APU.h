#pragma once

#include <cstdint>
#include <atomic>

class APU
{
public:
	uint8_t getSoundValue();
	void divFlipped(uint8_t bit);
	uint8_t shift_channel4_reg();
	union {
		struct {
			uint8_t sweep_shift : 3;
			bool increase_decrease : 1;
			uint8_t sweep_time : 3;
			bool unused : 1;
		};
		uint8_t reg = 0x80;
	} NR10;
	union {
		struct {
			uint8_t sound_length : 6;
			uint8_t duty : 2;
		};
		uint8_t reg = 0xBF;
	} NR11;
	union {
		struct {
			uint8_t sweep : 3;
			bool direction : 1;
			uint8_t volume : 4;
		};
		uint8_t reg = 0xF3;
	} NR12;
	uint8_t NR13;
	union {
		struct {
			uint8_t frequency : 3;
			uint8_t unused : 3;
			bool counter : 1;
			bool initial : 1;
		};
		uint8_t reg = 0xBF;
	} NR14;

	union {
		struct {
			uint8_t length : 6;
			uint8_t duty : 2;
		};
		uint8_t reg = 0x3F;
	} NR21;
	union {
		struct {
			uint8_t sweep : 3;
			bool direction : 1;
			uint8_t volume : 4;
		};
		uint8_t reg = 0x00;
	} NR22;
	uint8_t NR23;
	union {
		struct {
			uint8_t frequency : 3;
			uint8_t unused : 3;
			bool counter : 1;
			bool initial : 1;
		};
		uint8_t reg = 0xBF;
	} NR24;

	union {
		struct {
			uint8_t unused : 7;
			bool enable : 1;
		};
		uint8_t reg = 0x7F;
	} NR30;
	uint8_t NR31 = 0xFF;
	union {
		struct {
			uint8_t unused : 5;
			uint8_t volume : 2;
			bool unused2 : 1;
		};
		uint8_t reg = 0x9F;
	} NR32;
	uint8_t NR33 = 0xBF;
	union {
		struct {
			uint8_t frequency : 3;
			uint8_t unused : 3;
			bool counter : 1;
			bool initial : 1;
		};
		uint8_t reg;
	} NR34;
	uint8_t waveram[16];

	union {
		struct {
			uint8_t length : 6;
			uint8_t unused : 2;
		};
		uint8_t reg = 0xFF;
	} NR41;
	union {
		struct {
			uint8_t sweep : 3;
			bool direction : 1;
			uint8_t volume : 4;
		};
		uint8_t reg = 0x00;
	} NR42;
	union {
		struct {
			uint8_t ratio : 3;
			bool width : 1;
			uint8_t frequency : 4;
		};
		uint8_t reg = 0x00;
	} NR43;
	union {
		struct {
			uint8_t unused : 6;
			bool counter : 1;
			bool initial : 1;
		};
		uint8_t reg = 0xBF;
	} NR44;
	std::atomic<uint16_t> noise_shift_reg = 0x1234;

	union {
		struct {
			uint8_t so1_volume : 3;
			bool so1_vin : 1;
			uint8_t so2_volume : 3;
			bool so2_vin : 1;
		};
		uint8_t reg = 0x77;
	} NR50;
	union {
		struct {
			bool sound1_to_so1 : 1;
			bool sound2_to_so1 : 1;
			bool sound3_to_so1 : 1;
			bool sound4_to_so1 : 1;
			bool sound1_to_so2 : 1;
			bool sound2_to_so2 : 1;
			bool sound3_to_so2 : 1;
			bool sound4_to_so2 : 1;
		};
		uint8_t reg = 0xF3;
	} NR51;
	union {
		struct {
			bool sound1_on : 1;
			bool sound2_on : 1;
			bool sound3_on : 1;
			bool sound4_on : 1;
			uint8_t unused : 3;
			bool allsound_on : 1;
		};
		uint8_t reg;
	} NR52;

private:
	uint8_t channel1_sweep_count = 0;
	uint8_t channel1_envelope_count = 0;
	uint8_t channel2_envelope_count = 0;
	uint8_t channel4_envelope_count = 0;
	uint8_t channel4_frequency_count = 0;
};

