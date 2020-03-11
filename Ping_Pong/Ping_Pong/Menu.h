#pragma once

#include <SDL_ttf.h>
#include <stdint.h>
#include <exception>

#define MENU_PLAY_EN "Play"
#define MENU_TWO_PLAY_EN "Play with friends"
#define MENU_EXIT_EN "Exit"

class Menu
{
public:

	Menu(TTF_Font *font_menu, int32_t icolor_on = 0xFF0000, int32_t icolor_off = 0xF0F0F0);
	~Menu();

	SDL_Surface	*play_txt[2];
	SDL_Surface	*two_play_txt[2];
	SDL_Surface	*exit_txt[2];

private:

	SDL_Color	color_on;
	SDL_Color	color_off;

};

SDL_Color	int_to_rgb(int32_t icolor);
