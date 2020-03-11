#pragma once

#include <SDL_ttf.h>
#include <stdint.h>
#include <exception>

#define MENU_PLAY_EN "Play"
#define MENU_TWO_PLAY_EN "Play with friends"
#define MENU_EXIT_EN "Exit"

#define END_WIN_EN "You win!"
#define END_LOSE_EN "Try again!"
#define END_WIN_PL_ONE_EN "Player one win!"
#define END_WIN_PL_TWO_EN "Player two win!"
#define END_PRESS "Press the button"
#define END_AUTHOR "Created by Igor Stalevskiy"

class Menu
{
public:

	Menu(TTF_Font *font_menu, TTF_Font *font_info,
		int32_t icolor_on = 0xFF0000, int32_t icolor_off = 0xF0F0F0);
	~Menu();

	SDL_Surface	*play_txt[2];
	SDL_Surface	*two_play_txt[2];
	SDL_Surface	*exit_txt[2];

	SDL_Surface *end_win;
	SDL_Surface *end_lose;
	SDL_Surface *end_author;
	SDL_Surface *end_press;
	SDL_Surface *end_win_one;
	SDL_Surface *end_win_two;

private:
	SDL_Color	color_on;
	SDL_Color	color_off;
};

SDL_Color	int_to_rgb(int32_t icolor);
