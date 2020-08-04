#pragma once

#include <cstdint>

class APU
{
public:
	union {
		struct {
			uint8_t sweep_shift : 3;
			bool increase_decrease : 1;
			uint8_t sweep_time : 3;
			bool unused : 1;
		};
		uint8_t reg;
	} NR10;
	union {
		struct {
			uint8_t sound_length : 6;
			uint8_t duty : 2;
		};
		uint8_t reg;
	} NR11;
	union {
		struct {
			uint8_t sweep : 3;
			bool direction : 1;
			uint8_t volume : 4;
		};
		uint8_t reg;
	} NR12;
	uint8_t NR13;
	union {
		struct {
			uint8_t frequency : 3;
			uint8_t unused : 3;
			bool counter : 1;
			bool initial : 1;
		};
		uint8_t reg;
	} NR14;

	union {
		struct {
			uint8_t length : 6;
			uint8_t duty : 2;
		};
		uint8_t reg;
	} NR21;
	union {
		struct {
			uint8_t sweep : 3;
			bool direction : 1;
			uint8_t volume : 4;
		};
		uint8_t reg;
	} NR22;
	uint8_t NR23;
	union {
		struct {
			uint8_t frequency : 3;
			uint8_t unused : 3;
			bool counter : 1;
			bool initial : 1;
		};
		uint8_t reg;
	} NR24;

	union {
		struct {
			uint8_t unused : 7;
			bool enable : 1;
		};
		uint8_t reg;
	} NR30;
	uint8_t NR31;
	union {
		struct {
			uint8_t unused : 5;
			uint8_t volume : 2;
			bool unused2 : 1;
		};
		uint8_t reg;
	} NR32;
	uint8_t NR33;
	union {
		struct {
			uint8_t frequency : 3;
			uint8_t unused : 3;
			bool counter : 1;
			bool initial : 1;
		};
		uint8_t reg;
	} NR34;

	union {
		struct {
			uint8_t length : 6;
			uint8_t unused : 2;
		};
		uint8_t reg;
	} NR41;
	union {
		struct {
			uint8_t sweep : 3;
			bool direction : 1;
			uint8_t volume : 4;
		};
		uint8_t reg;
	} NR42;
	union {
		struct {
			uint8_t ratio : 3;
			bool width : 1;
			uint8_t frequency : 4;
		};
		uint8_t reg;
	} NR43;
	union {
		struct {
			uint8_t unused : 6;
			bool counter : 1;
			bool initial : 1;
		};
		uint8_t reg;
	} NR44;

	union {
		struct {
			uint8_t so1_volume : 3;
			bool so1_vin : 1;
			uint8_t so2_volume : 3;
			bool so2_vin : 1;
		};
		uint8_t reg;
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
		uint8_t reg;
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
};

