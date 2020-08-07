#include "Speaker.h"

#include <thread>
#include <iostream>
#include <SDL.h>

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
	audio_spec.samples = 512;
	audio_spec.callback = NULL;
	double time_step = 1.0 / audio_spec.freq;
	double global_time = 0;

	audio_device = SDL_OpenAudioDevice(NULL, 0, &audio_spec, NULL, 0);
	while (true) {
		for (int i = 0; i < audio_spec.samples; i++) {
			int16_t sample = (sin(global_time * 440 * 2 * M_PI)) * 3000;
			SDL_QueueAudio(audio_device, &sample, sizeof(int16_t));
			global_time += time_step;
		}
		SDL_PauseAudioDevice(audio_device, 0);
	}
}