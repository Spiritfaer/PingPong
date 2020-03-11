#include "Menu.h"

Menu::Menu(TTF_Font *font_menu, int32_t icolor_on, int32_t icolor_off)
{
	color_on = int_to_rgb(icolor_on);
	color_off = int_to_rgb(icolor_off);

	if (!(play_txt[0] = TTF_RenderText_Blended(font_menu, MENU_PLAY_EN, color_on))) {
		throw std::exception("Render TTF error!");
	}
	if (!(play_txt[1] = TTF_RenderText_Blended(font_menu, MENU_PLAY_EN, color_off))) {
		throw std::exception("Render TTF error!");
	}

	if (!(two_play_txt[0] = TTF_RenderText_Blended(font_menu, MENU_TWO_PLAY_EN, color_on))) {
		throw std::exception("Render TTF error!");
	}
	if (!(two_play_txt[1] = TTF_RenderText_Blended(font_menu, MENU_TWO_PLAY_EN, color_off))) {
		throw std::exception("Render TTF error!");
	}

	if (!(exit_txt[0] = TTF_RenderText_Blended(font_menu, MENU_EXIT_EN, color_on))) {
		throw std::exception("Render TTF error!");
	}
	if (!(exit_txt[1] = TTF_RenderText_Blended(font_menu, MENU_EXIT_EN, color_off))) {
		throw std::exception("Render TTF error!");
	}
}

Menu::~Menu()
{
	SDL_FreeSurface(play_txt[0]);
	SDL_FreeSurface(play_txt[1]);
	SDL_FreeSurface(two_play_txt[0]);
	SDL_FreeSurface(two_play_txt[1]);
	SDL_FreeSurface(exit_txt[0]);
	SDL_FreeSurface(exit_txt[1]);
}

SDL_Color int_to_rgb(int32_t icolor)
{
	SDL_Color tmp;
	tmp.r = (icolor >> 16) & 0xFF;
	tmp.g = (icolor >> 8) & 0xFF;
	tmp.b = icolor & 0xFF;
	return tmp;
}
