#pragma once

#include <exception>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include <memory>
#include "FrameRate.h"
#include "Obj.h"
#include "Player.h"
#include "Boll.h"

#define WINDOW_COF 2
#define FONT_SIZE 100

#define PATH_BEEEP "C:\\Users\\User.000\\source\\repos\\TEST_ping_pong\\Ping_Pong\\Ping_Pong\\sounds_ping_pong_8bit\\ping_pong_8bit_beeep.ogg"
#define PATH_PEEEEEEP "C:\\Users\\User.000\\source\\repos\\TEST_ping_pong\\Ping_Pong\\Ping_Pong\\sounds_ping_pong_8bit\\ping_pong_8bit_peeeeeep.ogg"
#define PATH_PLOP "C:\\Users\\User.000\\source\\repos\\TEST_ping_pong\\Ping_Pong\\Ping_Pong\\sounds_ping_pong_8bit\\ping_pong_8bit_plop.ogg"

using v_uptr_obj = std::vector< std::unique_ptr<Obj> >;

class Core
{
public:
	Core();
	~Core();

	void *getPixels();

	void refresh_event();
	void refresh_window();
	void clear_window();
	void refresh_fild(int32_t icolor = 0xFFFFFF);
	void refresh_obj();
	void refresh_player();
	bool is_exit();

	void add_new_obj(std::unique_ptr<Obj> ptr);

	SDL_Rect		screen;
	SDL_Event		event;
	FrameRate		fps;

private:
	SDL_Renderer	*render;
	SDL_Window		*window;
	SDL_Surface		*canvas;
	SDL_Surface		*pl1_score = nullptr;
	SDL_Surface		*pl2_score = nullptr;
	SDL_Texture		*texture;
	TTF_Font		*font;
	const char		*win_name = "Ping pong";
	v_uptr_obj		objects;
	Mix_Chunk		*beeep;
	Mix_Chunk		*peeeeeep;
	Mix_Chunk		*plop;


	void	draw_midle_line(int32_t icolor = 0xFFFFFF);
	void	draw_score(int32_t icolor = 0xFFFFFF);
	void	refresh_scope(int32_t icolor = 0xFFFFFF);
	void	check_collision();
	Side	is_collision(const Boll *boll, Player *pl1, Player *pl2);
	void	load_audio();
	void	play_audio(Mix_Chunk *chunk);
};
