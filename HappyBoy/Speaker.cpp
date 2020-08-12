#include "Speaker.h"

#include <thread>
#include <iostream>

void audio_callback(void* userdata, Uint8* stream, int len)
{
	Speaker* s = (Speaker*)userdata;
	if (!s->apu->NR52.allsound_on) {
		return;
	}

	if (s->apu->channel1_reset) {
		s->apu->channel1_reset = false;
		channel1.playing = true;
		channel1.envelope_step = s->apu->NR12.sweep;
		channel1.envelope_initial_volume = s->apu->NR12.volume;
		channel1.envelope_volume = channel1.envelope_initial_volume;
		channel1.envelope_increase = s->apu->NR12.direction;
		channel1.start_time = s->global_time;
		channel1.length = s->apu->NR11.sound_length;
	}
	channel1.l_volume = s->apu->NR51.sound1_to_so1 ? s->apu->NR50.so1_volume : 0;
	channel1.r_volume = s->apu->NR51.sound1_to_so2 ? s->apu->NR50.so2_volume : 0;
	channel1.initial = s->apu->NR14.initial;
	channel1.constant = !s->apu->NR14.counter;
	channel1.freq = 131072.0 / (2048 - (((s->apu->NR14.reg & 0x07) << 8) + s->apu->NR13));

	if (s->apu->channel2_reset) {
		s->apu->channel2_reset = false;
		channel2.playing = true;
		channel2.envelope_step = s->apu->NR22.sweep;
		channel2.envelope_initial_volume = s->apu->NR22.volume;
		channel2.envelope_volume = channel2.envelope_initial_volume;
		channel2.envelope_increase = s->apu->NR22.direction;
		channel2.start_time = s->global_time;
		channel2.length = s->apu->NR21.length;
	}
	channel2.l_volume = s->apu->NR51.sound2_to_so1 ? s->apu->NR50.so1_volume : 0;
	channel2.r_volume = s->apu->NR51.sound2_to_so2 ? s->apu->NR50.so2_volume : 0;
	channel2.initial = s->apu->NR24.initial;
	channel2.constant = !s->apu->NR24.counter;
	channel2.freq = 131072.0 / (2048 - (((s->apu->NR24.reg & 0x07) << 8) + s->apu->NR23));

	if (s->apu->channel3_reset) {
		s->apu->channel3_reset = false;
		channel3.playing = true;
		channel3.start_time = s->global_time;
		channel3.length = s->apu->NR31;
	}
	channel3.l_volume = s->apu->NR51.sound3_to_so1 ? s->apu->NR50.so1_volume : 0;
	channel3.r_volume = s->apu->NR51.sound3_to_so2 ? s->apu->NR50.so2_volume : 0;
	channel3.initial = s->apu->NR34.initial;
	channel3.constant = !s->apu->NR34.counter;
	channel3.freq = 131072.0 / (2048 - (((s->apu->NR34.reg & 0x07) << 8) + s->apu->NR33));

	if (s->apu->channel4_reset) {
		s->apu->channel4_reset = false;
		channel4.playing = true;		
		channel4.envelope_step = s->apu->NR42.sweep;
		channel4.envelope_initial_volume = s->apu->NR42.volume;
		channel4.envelope_volume = channel2.envelope_initial_volume;
		channel4.envelope_increase = s->apu->NR42.direction;
		channel4.start_time = s->global_time;
		channel4.length = s->apu->NR41.length;
	}	
	channel4.l_volume = s->apu->NR51.sound4_to_so1 ? s->apu->NR50.so1_volume : 0;
	channel4.r_volume = s->apu->NR51.sound4_to_so2 ? s->apu->NR50.so2_volume : 0;
	channel4.initial = s->apu->NR44.initial;
	channel4.constant = !s->apu->NR44.counter;
	channel4.freq = 524288 / (s->apu->NR43.ratio == 0 ? 0.5 : s->apu->NR43.ratio) / (1 << (s->apu->NR43.frequency + 1));

	for (int i = 0; i < len / 2; i++) {
		((Uint16*)stream)[i] = (channel1.get_sample(s->global_time) + channel2.get_sample(s->global_time) + channel3.get_sample(s->global_time, s->apu->waveram) + channel4.get_sample(s->global_time)) * 4;
		s->global_time += s->time_step;
	}
}



Speaker::Speaker(std::shared_ptr<APU> apu) : apu(apu)
{
	std::thread speakerThread(&Speaker::Start, this);
	speakerThread.detach();
}


void Speaker::Start() {
	SDL_Init(SDL_INIT_AUDIO);
	SDL_AudioDeviceID audio_device;
	SDL_AudioSpec audio_spec;
	SDL_zero(audio_spec);
	audio_spec.freq = 44100;
	audio_spec.format = AUDIO_S16SYS;
	audio_spec.channels = 1;
	audio_spec.samples = 1024;
	audio_spec.callback = audio_callback;
	audio_spec.userdata = this;
	time_step = 1.0 / audio_spec.freq;
	global_time = 0;

	audio_device = SDL_OpenAudioDevice(NULL, 0, &audio_spec, NULL, 0);

	SDL_PauseAudioDevice(audio_device, 0);
}


