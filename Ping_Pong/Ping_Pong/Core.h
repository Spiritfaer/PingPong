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
#include "Bot.h"
#include "Menu.h"

// Specify the number of points to win
#define SCORE_TO_WIN 5

// You can resize the window using the macro MEDIUM_WINDOW or BIG_WINDOW
// Be careful, this functionality has not been tested.
#define WINDOW_COF MEDIUM_WINDOW

// If you enable db mode in std::cout will be displayed fps
#define DB false
#define FONT_SIZE_SCORE 100
#define FONT_SIZE_MENU 75
#define FONT_SIZE_INFO 25
#define MEDIUM_WINDOW 2
#define BIG_WINDOW 1

//You need to specify the correct paths for sound and font.
#define FONT_PATH "O.K.Retro.otf"
#define PATH_BEEEP "C:\\Users\\User.000\\source\\repos\\TEST_ping_pong\\Ping_Pong\\Ping_Pong\\sounds_ping_pong_8bit\\ping_pong_8bit_beeep.ogg"
#define PATH_PEEEEEEP "C:\\Users\\User.000\\source\\repos\\TEST_ping_pong\\Ping_Pong\\Ping_Pong\\sounds_ping_pong_8bit\\ping_pong_8bit_peeeeeep.ogg"
#define PATH_PLOP "C:\\Users\\User.000\\source\\repos\\TEST_ping_pong\\Ping_Pong\\Ping_Pong\\sounds_ping_pong_8bit\\ping_pong_8bit_plop.ogg"

using v_uptr_obj = std::vector< std::unique_ptr<Obj> >;

enum class AppStatus {GAME, GAME_TWO_PLAYERS, EXIT, MENU, END};
enum class EndStatus {WIN, LOSE, WIN_ONE, WIN_TWO};

class Core
{
public:
	Core();
	~Core();

	void run();

private:
	SDL_Rect		screen;
	SDL_Event		event;
	FrameRate		fps;
	SDL_Renderer	*render;
	SDL_Window		*window;
	SDL_Surface		*canvas;
	SDL_Surface		*pl1_score = nullptr;
	SDL_Surface		*pl2_score = nullptr;
	SDL_Texture		*texture;
	TTF_Font		*font_score;
	TTF_Font		*font_menu;
	TTF_Font		*font_info;
	const char		*win_name = "Ping pong";
	v_uptr_obj		objects;
	Mix_Chunk		*beeep;
	Mix_Chunk		*peeeeeep;
	Mix_Chunk		*plop;
	Menu			*my_menu;
	int32_t			menu_flag = 0;
	EndStatus		status_end;
	AppStatus		status;
	bool			db;

	void			menu();
	void			game();
	void			end();
	bool			is_end();
	void			add_new_obj(std::unique_ptr<Obj> ptr);
	void			init_one_game();
	void			init_two_game();
	void			*getPixels();
	void			refresh_event();
	void			refresh_window();
	void			clear_window();
	void			refresh_fild(int32_t icolor = 0xFFFFFF);
	void			refresh_obj();
	void			refresh_player();
	bool			is_exit();
	void			draw_midle_line(int32_t icolor = 0xFFFFFF);
	void			draw_score(int32_t icolor = 0xFFFFFF);
	void			draw_menu();
	void			draw_end();
	void			refresh_scope(int32_t icolor = 0xFFFFFF);
	void			check_collision();
	Side			is_collision(const Boll *boll, Player *pl1, Player *pl2);
	void			load_audio();
	void			play_audio(Mix_Chunk *chunk);
	Menu			*make_menu(TTF_Font *font_menu, TTF_Font * font_info);
};
