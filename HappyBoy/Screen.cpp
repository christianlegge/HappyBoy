#include "Screen.h"

#include <thread>
#include <string>
#include <chrono>
#include <algorithm>
#include <iostream>

Screen::Screen(int width, int height, std::shared_ptr<Clock> clock, std::shared_ptr<CPU> cpu, std::shared_ptr<Bus> bus, std::shared_ptr<PPU> ppu) : width(width), height(height), clock(clock), cpu(cpu), bus(bus), ppu(ppu) {
	std::thread screenThread(&Screen::Start, this);
	screenThread.detach();
}

void Screen::Start()
{
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("HappyBoy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, 160, 144);
	screen_rect = { 0, 0, height * 160 / 144, height };
	long long frames = 0;

	TTF_Init();
	font = TTF_OpenFont("Hack-Regular.ttf", 16);

	auto t1 = std::chrono::steady_clock::now();
	bool quit = false;
	SDL_Event e;

	tileset = new color[384 * 64];
	tilemap = new color[32 * 32 * 64];
	uint16_t addr = 0x9800;

	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Color black = { 0, 0, 0, 255 };

	std::string text;
	std::string composition;
	Sint32 cursor;
	Sint32 selection_len;
	bool typing = false;
	bool lastPauseState = false;

	screen_pixels = (uint8_t*)ppu->getScreen();

	while (!quit){
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_EventType::SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_EventType::SDL_TEXTINPUT) {
				if (typing && (std::string)e.text.text != "i") {
					dirtyData = true;
					text += e.text.text;
				}
			}
			else if (e.type == SDL_EventType::SDL_KEYUP) {
				if (e.key.keysym.sym == SDLK_UP) {
					bus->button(GBButton::Up, false);
				}
				else if (e.key.keysym.sym == SDLK_DOWN) {
					bus->button(GBButton::Down, false);
				}
				else if (e.key.keysym.sym == SDLK_LEFT) {
					bus->button(GBButton::Left, false);
				}
				else if (e.key.keysym.sym == SDLK_RIGHT) {
					bus->button(GBButton::Right, false);
				}
				else if (e.key.keysym.sym == SDLK_z) {
					bus->button(GBButton::B, false);
				}
				else if (e.key.keysym.sym == SDLK_x) {
					bus->button(GBButton::A, false);
				}
				else if (e.key.keysym.sym == SDLK_RETURN) {
					bus->button(GBButton::Start, false);
				}
				else if (e.key.keysym.sym == SDLK_RSHIFT) {
					bus->button(GBButton::Select, false);
				}
			}
			else if (e.type == SDL_EventType::SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_UP) {
					bus->button(GBButton::Up, true);
				}
				else if (e.key.keysym.sym == SDLK_DOWN) {
					bus->button(GBButton::Down, true);
				}
				else if (e.key.keysym.sym == SDLK_LEFT) {
					bus->button(GBButton::Left, true);
				}
				else if (e.key.keysym.sym == SDLK_RIGHT) {
					bus->button(GBButton::Right, true);
				}
				else if (e.key.keysym.sym == SDLK_z) {
					bus->button(GBButton::B, true);
				}
				else if (e.key.keysym.sym == SDLK_x) {
					bus->button(GBButton::A, true);
				}
				else if (e.key.keysym.sym == SDLK_RETURN) {
					bus->button(GBButton::Start, true);
					if (typing) {
						clock->breakpoint = std::stoi(text, nullptr, 0x10);
						typing = false;
					}
				}
				else if (e.key.keysym.sym == SDLK_RSHIFT) {
					bus->button(GBButton::Select, true);
				}
				else if (e.key.keysym.sym == SDLK_r) {
					cpu->reset();
				}
				else if (e.key.keysym.sym == SDLK_SPACE) {
					clock->step();
					dirtyData = true;
				}
				else if (e.key.keysym.sym == SDLK_i) {
					if (clock->paused) {
						typing = !typing;
						text = "";
					}
				}
				else if (e.key.keysym.sym == SDLK_f && !typing) {
					clock->frame();
					dirtyData = true;
				}
				else if (e.key.keysym.sym == SDLK_p) {
					clock->paused = !clock->paused;
					if (clock->paused) {
						SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
						dirtyData = true;
					}
					else {
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
					}
					SDL_Rect pause1 = { width - 16, 4, 12, 24 };
					SDL_Rect pause2 = { width - 32, 4, 12, 24 };
					SDL_RenderFillRect(renderer, &pause1);
					SDL_RenderFillRect(renderer, &pause2);
				}
				else if (e.key.keysym.sym == SDLK_PAGEDOWN) {
					dirtyData = true;
					addr += 0x100;
				}
				else if (e.key.keysym.sym == SDLK_PAGEUP) {
					dirtyData = true;
					addr -= 0x100;
				}
			}
		}

		if (clock->paused && !lastPauseState) {
			dirtyData = true;
		}
		lastPauseState = clock->paused;

		if (clock->paused) {
			if (dirtyData) {
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderClear(renderer);
				render_text(800, 800, ("Breakpoint: " + text).c_str(), font, &white);
				DrawDisasm(height * 160 / 144, 0, 40);
				DrawMemory(height * 160 / 144 + 256, 154, addr);
				DrawRegisters(height * 160 / 144 + 256, 0);
				DrawTileset(height * 160 / 144 + 256 + 2, 584);
				DrawTilemap(height * 160 / 144 + 256 + 8 * 16 + 32, 584);
				DrawScreen();
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_Rect pause1 = { width - 16, 4, 12, 24 };
				SDL_Rect pause2 = { width - 32, 4, 12, 24 };
				SDL_RenderFillRect(renderer, &pause1);
				SDL_RenderFillRect(renderer, &pause2);
				dirtyData = false;
			}
		}
		else if (ppu->getScreenFrameReady()) {
			frames++;
			DrawScreen();
			dirtyData = true;
			auto t2 = std::chrono::steady_clock::now();
			if (!(frames % 10)) {
				SDL_SetWindowTitle(window, ("HappyBoy - FPS: " + std::to_string((int)(1 / ((t2 - t1).count() / 1000000000.0f)))).c_str());
			}
			t1 = t2;
		}

		SDL_RenderPresent(renderer);
	}

	delete tileset;

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Screen::DrawScreen()
{
	SDL_UpdateTexture(texture, NULL, screen_pixels, 160*3); 
	SDL_RenderCopy(renderer, texture, NULL, &screen_rect);
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
		render_text(x, y + i * 16 + 1, disasm[i].c_str(), font, &color);
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
		render_text(x, y + i * 16 + 1, mem[i].c_str(), font, &color);
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
		render_text(x, y + i * 16 + 1, reg[i].c_str(), font, &color);
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

void Screen::render_text(int x, int y, const char* text, TTF_Font* font, SDL_Color* color)
{
	SDL_Surface* surface = TTF_RenderText_Solid(font, text, *color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!surface || !texture) {
		return;
	}
	SDL_Rect rect = { x, y, surface->w, surface->h };
	SDL_FreeSurface(surface);
	SDL_RenderCopy(renderer, texture, NULL, &rect);
	SDL_DestroyTexture(texture);
}
