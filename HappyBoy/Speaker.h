#pragma once

#include <memory>

#include "APU.h";
#include <SDL.h>

struct {
	double length = 0;
	double start_time;
	double freq;
	bool constant = false;
	bool initial = false;
	double l_volume = 0;
	double r_volume = 0;
	bool playing = false;

	int16_t get_sample(double time) {
		if (!playing) {
			return 0;
		}
		if (time < start_time + length || constant) {
			return (sin(time * freq * 2 * M_PI) > 0 ? 1 : -1) * ((l_volume + r_volume) / 14.0) * 1000;
		}
		else {
			return 0;
		}
	}
} channel1;

struct {
	double length = 0;
	double start_time;
	double freq;
	bool constant = false;
	bool initial = false;
	double l_volume = 0;
	double r_volume = 0;
	bool playing = false;

	int16_t get_sample(double time) {
		if (!playing) {
			return 0;
		}
		if (time < start_time + length || constant) {
			return (sin(time * freq * 2 * M_PI) > 0 ? 1 : -1) * ((l_volume + r_volume) / 14.0) * 1000;
		}
		else {
			return 0;
		}
	}
} channel2;

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

