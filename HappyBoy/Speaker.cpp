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
	audio_spec.samples = 1024;
	audio_spec.callback = NULL;

	audio_device = SDL_OpenAudioDevice(NULL, 0, &audio_spec, NULL, 0);
	SDL_PauseAudioDevice(audio_device, 0);
}