#include "Screen.h"

#include <thread>
#include <string>
#include <chrono>
#include <algorithm>
#include <iostream>

Screen::Screen(std::shared_ptr<Clock> clock, std::shared_ptr<CPU> cpu, std::shared_ptr<Bus> bus, std::shared_ptr<PPU> ppu) : clock(clock), cpu(cpu), bus(bus), ppu(ppu) {
	width = 1800;
	height = 900;
	//Start();
	std::thread screenThread(&Screen::Start, this);
	screenThread.detach();
}

void Screen::Start()
{
	float scale = 1;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("HappyBoy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width*scale, height*scale, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderSetScale(renderer, scale, scale);
	long long frames = 0;

	TTF_Init();
	font = TTF_OpenFont("Hack-Regular.ttf", 16);

	auto t1 = std::chrono::steady_clock::now();
	bool quit = false;
	SDL_Event e;

	tileset = new color[384 * 64];
	tilemap = new color[32 * 32 * 64];
	uint16_t addr = 0x9800;


	std::string text;
	std::string composition;
	Sint32 cursor;
	Sint32 selection_len;

	while (!quit){
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_EventType::SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_EventType::SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_SPACE) {
					clock->step();
					dirtyData = true;
				}
				if (e.key.keysym.sym == SDLK_b) {
					SDL_Rect rect = { 800, 800, 80, 16 };
					SDL_StartTextInput();
					SDL_SetTextInputRect(&rect);
				}
				if (e.key.keysym.sym == SDLK_f) {
					clock->frame();
					dirtyData = true;
				}
				if (e.key.keysym.sym == SDLK_p) {
					clock->paused = !clock->paused;
					if (clock->paused) {
						SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
					}
					else {
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
					}
					SDL_Rect pause1 = { width - 16, 4, 12, 24 };
					SDL_Rect pause2 = { width - 32, 4, 12, 24 };
					SDL_RenderFillRect(renderer, &pause1);
					SDL_RenderFillRect(renderer, &pause2);
				}
				if (e.key.keysym.sym == SDLK_PAGEDOWN) {
					dirtyData = true;
					addr += 0x100;
				}
				if (e.key.keysym.sym == SDLK_PAGEUP) {
					dirtyData = true;
					addr -= 0x100;
				}
				if (e.key.keysym.sym == SDLK_RETURN) {
					SDL_StopTextInput();
				}
			}
			/*else if (e.type == SDL_EventType::SDL_TEXTINPUT) {
				text += e.text.text;
			}*/
			else if (e.type == SDL_EventType::SDL_TEXTEDITING) {
				composition = e.edit.text;
			}
		}

		if (clock->paused) {
			if (dirtyData) {
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderClear(renderer);
				DrawDisasm(height * 144 / 160, 0, 40);
				DrawMemory(height * 144 / 160 + 256, 154, addr);
				DrawRegisters(height * 144 / 160 + 256, 0);
				DrawTileset(height * 144 / 160 + 256 + 2, 584);
				DrawTilemap(height * 144 / 160 + 256 + 8 * 16 + 32, 584);
				DrawScreen(0, 0);
				dirtyData = false;
			}
		}
		else {
			DrawScreen(0, 0);
			dirtyData = true;
		}

		SDL_Color color;
		color.r = 255;
		color.g = 255;
		color.b = 255;
		color.a = 255;

		SDL_Rect rect = { width - 32, 4, 12, 24 };


		auto t2 = std::chrono::steady_clock::now();

		if (!(frames % 10)) {
			SDL_SetWindowTitle(window, ("HappyBoy - FPS: " + std::to_string((int)(1 / ((t2 - t1).count() / 1000000000.0f)))).c_str());
		}

		t1 = t2;

		SDL_RenderPresent(renderer);
		frames++;
	}

	delete tileset;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Screen::DrawScreen(int x, int y)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = height * 144 / 160;
	rect.h = height;
	int pixelsize = std::min(rect.w / 160, rect.h / 144);

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	//SDL_RenderDrawRect(renderer, &rect);

	color* screen = ppu->getScreen();
	for (int cx = 0; cx < 160; cx++) {
		for (int cy = 0; cy < 144; cy++) {
			SDL_SetRenderDrawColor(renderer, screen[cy*160 + cx].r, screen[cy * 160 + cx].g, screen[cy * 160 + cx].b, 255);
			SDL_Rect r = { x + cx*pixelsize, y + cy*pixelsize, pixelsize, pixelsize};
			SDL_RenderFillRect(renderer, &r);
		}
	}

	SDL_Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	//render_text(x, y, "Screen", font, &rect, &color);
}

void Screen::DrawDisasm(int x, int y, int lines)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = 256;
	rect.h = lines * 17 - 1;

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	//SDL_RenderDrawRect(renderer, &rect);

	rect.h = 16;

	SDL_SetRenderDrawColor(renderer, 66, 135, 245, 255);
	SDL_RenderFillRect(renderer, &rect);

	SDL_Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	std::vector<std::string> disasm = cpu->getDisassembly(lines);

	for (int i = 0; i < disasm.size(); i++) {
		render_text(x, y + i * 16 + 1, disasm[i].c_str(), font, &rect, &color);
	}
}

void Screen::DrawMemory(int x, int y, uint16_t addr)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = 256;
	rect.h = 256;

	SDL_Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	std::vector<std::string> mem = bus->getMemory(addr);

	for (int i = 0; i < mem.size(); i++) {
		render_text(x, y + i * 16 + 1, mem[i].c_str(), font, &rect, &color);
	}
}

void Screen::DrawRegisters(int x, int y)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = 256;
	rect.h = 256;

	SDL_Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	std::vector<std::string> reg = cpu->getRegisters();

	for (int i = 0; i < reg.size(); i++) {
		render_text(x, y + i * 16 + 1, reg[i].c_str(), font, &rect, &color);
	}
}

void Screen::DrawTileset(int x, int y)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = height * 144 / 160;
	rect.h = height;

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	//SDL_RenderDrawRect(renderer, &rect);

	ppu->getTileset(tileset);
		
	for (int cx = 0; cx < 8 * 16; cx++) {
		for (int cy = 0; cy < 8 * 24; cy++) {
			SDL_SetRenderDrawColor(renderer, tileset[cy * 8 * 16 + cx].r, tileset[cy * 8 * 16 + cx].g, tileset[cy * 8 * 16 + cx].b, 255);
			SDL_RenderDrawPoint(renderer, x + cx + cx/8, y + cy + cy/8);
		}
	}

	SDL_Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	//render_text(x, y, "Screen", font, &rect, &color);
}

void Screen::DrawTilemap(int x, int y)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = height * 144 / 160;
	rect.h = height;

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	//SDL_RenderDrawRect(renderer, &rect);

	ppu->getTilemap(tilemap);

	for (int cx = 0; cx < 8 * 32; cx++) {
		for (int cy = 0; cy < 8 * 32; cy++) {
			SDL_SetRenderDrawColor(renderer, tilemap[cy * 8 * 32 + cx].r, tilemap[cy * 8 * 32 + cx].g, tilemap[cy * 8 * 32 + cx].b, 255);
			SDL_RenderDrawPoint(renderer, x + cx, y + cy);
		}
	}

	SDL_Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	//render_text(x, y, "Screen", font, &rect, &color);
}

void Screen::render_text(int x, int y, const char* text, TTF_Font* font, SDL_Rect* rect, SDL_Color* color)
{
	SDL_Surface* surface = TTF_RenderText_Solid(font, text, *color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!surface || !texture) {
		return;
	}
	rect->x = x;
	rect->y = y;
	rect->w = surface->w;
	rect->h = surface->h;
	SDL_FreeSurface(surface);
	SDL_RenderCopy(renderer, texture, NULL, rect);
	SDL_DestroyTexture(texture);
}
