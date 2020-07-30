#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <memory>

#include "Clock.h"

class Screen
{
public:
	Screen(std::shared_ptr<Clock> clock, std::shared_ptr<CPU> cpu, std::shared_ptr<Bus> bus, std::shared_ptr<PPU> ppu);
private:
	void Start();
	void DrawScreen(int x, int y);
	void DrawDisasm(int x, int y, int lines);
	void DrawMemory(int x, int y, uint16_t addr);
	void DrawRegisters(int x, int y);
	void DrawTileset(int x, int y);
	void DrawTilemap(int x, int y);
	void render_text(int x, int y, const char* text, TTF_Font* font, SDL_Rect* rect, SDL_Color* color);
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font;
	color* tileset;
	color* tilemap;
	std::shared_ptr<Clock> clock;
	std::shared_ptr<CPU> cpu;
	std::shared_ptr<Bus> bus;
	std::shared_ptr<PPU> ppu;
	bool dirtyData = true;
	int width;
	int height;
};

