#pragma once

#include <memory>
#include <algorithm>

#include "APU.h";
#include <SDL.h>

struct {
	double freq;
	double l_volume;
	double r_volume;
	uint8_t envelope_volume;

	int16_t get_sample(double time) {
		double volume_mod = envelope_volume / 15.0;
		return (sin(time * freq * 2 * M_PI) > 0 ? 1 : -1) * ((l_volume + r_volume) / 14.0) * 1000 * volume_mod;
	}
} channel1;

struct {
	double freq;
	double l_volume;
	double r_volume;
	uint8_t envelope_volume;

	int16_t get_sample(double time) {
		double volume_mod = envelope_volume / 15.0;
		return (sin(time * freq * 2 * M_PI) > 0 ? 1 : -1) * ((l_volume + r_volume) / 14.0) * 1000 * volume_mod;
	}
} channel2;

struct {
	double length = 0;
	double freq;
	double l_volume = 0;
	double r_volume = 0;
	uint8_t output_level = 0;

	int16_t get_sample(double time, uint8_t* waveram) {
		if (output_level == 0) {
			return 0;
		}
		uint8_t wavebyte = (uint8_t)(time * freq * 32 / 2) % 32;
		uint8_t sample = (wavebyte % 2 == 0) ? waveram[wavebyte/2] >> 4 : waveram[wavebyte/2] & 0x0F;
		sample >>= (output_level - 1);
		return (sample) * ((l_volume + r_volume) / 14.0) * 1000 / 16.0;
	}
} channel3;

struct {
	double freq;
	double l_volume;
	double r_volume;
	uint8_t envelope_volume;
	uint8_t value;
	std::shared_ptr<APU> apu;

	int16_t get_sample(double time) {
		double volume_mod = envelope_volume / 15.0;
		return apu->shift_channel4_reg(freq/44100) * volume_mod * 1000;
	}
} channel4;

class Speaker
{
public:
	Speaker(std::shared_ptr<APU> apu);
	std::shared_ptr<APU> apu;
	double global_time;
	double time_step;

private:
	void Start();
};

