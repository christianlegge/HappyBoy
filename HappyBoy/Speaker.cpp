#include "Speaker.h"

#include <thread>
#include <iostream>

void audio_callback(void* userdata, Uint8* stream, int len)
{
	Speaker* s = (Speaker*)userdata;
	if (!s->apu->NR52.allsound_on) {
		return;
	}

	channel1.envelope_volume = s->apu->NR12.volume;
	channel1.l_volume = s->apu->NR51.sound1_to_so1 ? s->apu->NR50.so1_volume : 0;
	channel1.r_volume = s->apu->NR51.sound1_to_so2 ? s->apu->NR50.so2_volume : 0;
	channel1.freq = 131072.0 / (2048 - (((s->apu->NR14.reg & 0x07) << 8) + s->apu->NR13));
	
	channel2.envelope_volume = s->apu->NR22.volume;
	channel2.l_volume = s->apu->NR51.sound2_to_so1 ? s->apu->NR50.so1_volume : 0;
	channel2.r_volume = s->apu->NR51.sound2_to_so2 ? s->apu->NR50.so2_volume : 0;
	channel2.freq = 131072.0 / (2048 - (((s->apu->NR24.reg & 0x07) << 8) + s->apu->NR23));

	channel3.output_level = s->apu->NR32.volume;
	channel3.l_volume = s->apu->NR51.sound3_to_so1 ? s->apu->NR50.so1_volume : 0;
	channel3.r_volume = s->apu->NR51.sound3_to_so2 ? s->apu->NR50.so2_volume : 0;
	channel3.freq = 131072.0 / (2048 - (((s->apu->NR34.reg & 0x07) << 8) + s->apu->NR33)); 

	channel4.envelope_volume = s->apu->NR42.volume;
	channel4.l_volume = s->apu->NR51.sound4_to_so1 ? s->apu->NR50.so1_volume : 0;
	channel4.r_volume = s->apu->NR51.sound4_to_so2 ? s->apu->NR50.so2_volume : 0;
	channel4.value = (s->apu->noise_shift_reg >> (s->apu->NR43.width ? 7 : 15)) % 2;
	channel4.apu = s->apu;
	channel4.freq = 524288.0 / (s->apu->NR43.ratio == 0 ? 0.5 : s->apu->NR43.ratio) / (1 << (s->apu->NR43.frequency + 1));

	for (int i = 0; i < len / 2; i++) {
		((Uint16*)stream)[i] = (
			(s->apu->NR52.sound1_on ? channel1.get_sample(s->global_time) : 0) +
			(s->apu->NR52.sound2_on ? channel2.get_sample(s->global_time) : 0) + 
			(s->apu->NR52.sound3_on ? channel3.get_sample(s->global_time, s->apu->waveram) : 0) + 
			(s->apu->NR52.sound4_on ? channel4.get_sample(s->global_time) : 0)
			);
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


