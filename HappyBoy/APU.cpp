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
