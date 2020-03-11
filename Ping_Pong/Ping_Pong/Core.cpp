#include <ctime>
#include <sstream>
#include "Core.h"

Core::Core()
{
	srand(static_cast<uint32_t>(std::time(nullptr)));
	
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		throw std::exception("ERROR! SDL_INIT\n");
	}
	else {
		if (SDL_GetDisplayBounds(0, &screen) != 0) {
			throw std::exception(SDL_GetError());
		}
		screen.y = (screen.h / 2) - (screen.h / WINDOW_COF) / 2;
		screen.x = (screen.w / 2) - (screen.w / WINDOW_COF) / 2;
		screen.h /= WINDOW_COF;
		screen.w /= WINDOW_COF;
		window = SDL_CreateWindow(win_name, screen.x, screen.y,
			screen.w, screen.h, SDL_WINDOW_SHOWN);
		if (!window) {
			throw std::exception("ERROR! CREATED WINDOWS FAIL\n");
		}
		else {
			render = SDL_CreateRenderer(window, -1,
				SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		}
	}
	canvas = SDL_CreateRGBSurface(0, screen.w, screen.h, 32,
		0, 0, 0, 0);
	texture = SDL_CreateTextureFromSurface(render, canvas);

	//---------------------------------------------- FONT INIT --------------------

	if (TTF_Init() == -1) {
		throw std::exception("ERROR! TTF_INIT\n");
	}
	font_score = TTF_OpenFont("O.K.Retro.otf", FONT_SIZE_SCORE);
	if (!font_score) {
		throw std::exception("ERROR! TTF_OpenFont");
	}
	font_menu = TTF_OpenFont("O.K.Retro.otf", FONT_SIZE_MENU);
	if (!font_menu) {
		throw std::exception("ERROR! TTF_OpenFont");
	}

	//---------------------------------------------- AUDIO INIT --------------------
	int		initted;
	int		flags = MIX_INIT_OGG;

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		printf("SDL_mixer Error: %s\n", Mix_GetError());
	initted = Mix_Init(flags);
	if ((initted & flags) != flags)
		printf("SDL_mixer Error: %s\n", Mix_GetError());
	load_audio();
	//------------------------------------------------------------------------------
	my_menu = make_menu(font_menu);
	if (!my_menu)
		throw std::exception("My menu init error!");

	status = AppStatus::MENU;
}

Core::~Core()
{
	std::cout << "DB INFO : ~Core()" << std::endl;
	delete my_menu;
	TTF_CloseFont(font_score);
	TTF_CloseFont(font_menu);
	if (TTF_WasInit())
		TTF_Quit();
	if (beeep)
		Mix_FreeChunk(beeep);
	if (peeeeeep)
		Mix_FreeChunk(peeeeeep);
	if (plop)
		Mix_FreeChunk(plop);
	while (Mix_Init(0))
		Mix_Quit();
	Mix_CloseAudio();
	if (canvas)
		SDL_FreeSurface(canvas);
	if (texture)
		SDL_DestroyTexture(texture);
	if (render)
		SDL_DestroyRenderer(render);
	if (window)
		SDL_DestroyWindow(window);
	SDL_Quit();
}

void Core::run()
{
	do {
		if (status == AppStatus::MENU) {
			menu();
		}
		else if (status == AppStatus::GAME) {
			game();
		}
		else if (status == AppStatus::GAME_TWO_PLAYERS) {
			game();
		}
	} while (status != AppStatus::EXIT);
}

void Core::menu()
{
	menu_flag = 0;

	while (status == AppStatus::MENU) {
		refresh_event();
		fps.tick(true);
		if (is_exit()) {
			status = AppStatus::EXIT;
			return;
		}
		clear_window();
		draw_menu();
		refresh_window();
	}
}

void Core::game()
{
	if (status == AppStatus::GAME) {
		init_one_game();
	}
	else if (status == AppStatus::GAME_TWO_PLAYERS) {
		init_two_game();
	}
	while (true) {
		refresh_event();
		fps.tick(true);
		if (event.type == SDL_QUIT) {
			status = AppStatus::EXIT;
			break;
		}
		if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
			status = AppStatus::MENU;
			break;
		}
		clear_window();
		refresh_fild();
		refresh_obj();
		refresh_window();
	}
	objects.clear();
}

void *Core::getPixels()
{
	return canvas->pixels;
}

void Core::refresh_event()
{
	if (status == AppStatus::MENU) {
		SDL_WaitEvent(&event);
		const Uint8 *state = SDL_GetKeyboardState(nullptr);
		if (event.type == SDL_KEYDOWN) {
			play_audio(plop);
			if (state[SDL_SCANCODE_UP]) {
				menu_flag = (menu_flag == 0) ? 0 : menu_flag - 1;
			}
			if (state[SDL_SCANCODE_DOWN]) {
				menu_flag = (menu_flag == 2) ? 2 : menu_flag + 1;
			}
			if (state[SDL_SCANCODE_RETURN]) {
				switch (menu_flag)
				{
				case 0:
					status = AppStatus::GAME;
					break;
				case 1:
					status = AppStatus::GAME_TWO_PLAYERS;
					break;
				case 2:
					status = AppStatus::EXIT;
					break;
				default:
					break;
				}
			}
		}	
	}
	else
		SDL_PollEvent(&event);
}

void Core::refresh_window()
{
	SDL_UpdateTexture(texture, NULL, canvas->pixels, screen.w << 2u);
	SDL_RenderCopy(render, texture, 0, 0);
	SDL_RenderPresent(render);
}

void Core::clear_window()
{
	//TODO оптимизировать первые две строки
	SDL_Rect win{ 0,0,0,0 };
	SDL_GetWindowSize(window, &win.w, &win.h);
	SDL_FillRect(canvas, &win, 0x000000);
}

void Core::refresh_fild(int32_t icolor)
{
	draw_midle_line(icolor);
}

void Core::refresh_obj()
{
	check_collision();
	for (int i = 0; i < objects.size(); ++i) {
		if (dynamic_cast<Bot*>(objects[i].get()))
			dynamic_cast<Bot*>(objects[i].get())->refresh_dest(dynamic_cast<Boll*>(objects[0].get()));
		objects[i]->move(event, screen.w, screen.h, objects[i]->speed);
	}
	for (int i = 0; i < objects.size(); ++i) {
		objects[i]->obj_draw(screen, static_cast<int32_t*>(getPixels()));
	}
	draw_score();

}

void Core::refresh_player()
{
	refresh_scope();
}

bool Core::is_exit()
{
	if (event.type == SDL_QUIT) {
		status = AppStatus::EXIT;
		return true;
	}
	if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
		status = AppStatus::MENU;
	}
	return false;
}

void Core::add_new_obj(std::unique_ptr<Obj> ptr)
{
	objects.push_back(std::move(ptr));
}

void Core::init_one_game()
{
	std::unique_ptr<Obj> boll(new Boll((screen.w / 2 - 25 / 2), (screen.h / 2 - 25 / 2), 25, 25, 0xFFD600));
	std::unique_ptr<Obj> player1(new Player(25, (screen.h / 2 - 100 / 2), SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, 0xFFFFFF));
	std::unique_ptr<Obj> bot(new Bot(screen.w - (25 + 15), (screen.h / 2 - 100 / 2), SDL_SCANCODE_W, SDL_SCANCODE_S, 0xFFFFFF));
	add_new_obj(std::move(boll));
	add_new_obj(std::move(player1));
	add_new_obj(std::move(bot));
	refresh_player();
}

void Core::init_two_game()
{
	std::unique_ptr<Obj> boll(new Boll((screen.w / 2 - 25 / 2), (screen.h / 2 - 25 / 2), 25, 25, 0xFFD600));
	std::unique_ptr<Obj> player1(new Player(25, (screen.h / 2 - 100 / 2), SDL_SCANCODE_W, SDL_SCANCODE_S, 0xFFFFFF));
	std::unique_ptr<Obj> player2(new Player(screen.w - (25 + 15), (screen.h / 2 - 100 / 2), SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, 0xFFFFFF));
	add_new_obj(std::move(boll));
	add_new_obj(std::move(player1));
	add_new_obj(std::move(player2));
	refresh_player();
}

void Core::draw_midle_line(int32_t icolor)
{
	int32_t *pixels = static_cast<int32_t*>(getPixels());

	int32_t half_line_w = 2;
	int32_t half_line_h = half_line_w * 6;

	for (int32_t i = screen.w / 2 - half_line_w; i < screen.w / 2 + half_line_w; ++i) {
		for (int32_t j = 0; j < screen.h; j += half_line_h) {
			for (int32_t stop = j + half_line_h; j < stop; ++j) {
				pixels[i + j * screen.w] = icolor;
			}
		}
	}
}

void Core::draw_score(int32_t icolor)
{
	if (pl1_score == nullptr || pl2_score == nullptr)
		return ;
	SDL_Rect dest_pl1{ (screen.w / 4 * 1) - (pl1_score->w / 2), 10, 0, 0 };
	SDL_BlitSurface(pl1_score, NULL, canvas, &dest_pl1);

	SDL_Rect dest_pl2{ (screen.w / 4 * 3) - (pl2_score->w / 2), 10, 0, 0 };
	SDL_BlitSurface(pl2_score, NULL, canvas, &dest_pl2);
}

void Core::draw_menu()
{
	static SDL_Rect dest_play_txt{ (screen.w / 2) - (my_menu->play_txt[0]->w / 2), 10, 0, 0 };
	static SDL_Rect dest_two_play_txt{ (screen.w / 2) - (my_menu->two_play_txt[0]->w / 2), 50, 0, 0 };
	static SDL_Rect dest_exit_txt{ (screen.w / 2) - (my_menu->exit_txt[0]->w / 2), 100, 0, 0 };

	if (menu_flag == 0) {
		SDL_BlitSurface(my_menu->play_txt[1], NULL, canvas, &dest_play_txt);
	}
	else {
		SDL_BlitSurface(my_menu->play_txt[0], NULL, canvas, &dest_play_txt);
	}

	if (menu_flag == 1) {
		SDL_BlitSurface(my_menu->two_play_txt[1], NULL, canvas, &dest_two_play_txt);
	}
	else {
		SDL_BlitSurface(my_menu->two_play_txt[0], NULL, canvas, &dest_two_play_txt);
	}

	if (menu_flag == 2) {
		SDL_BlitSurface(my_menu->exit_txt[1], NULL, canvas, &dest_exit_txt);
	}
	else {
		SDL_BlitSurface(my_menu->exit_txt[0], NULL, canvas, &dest_exit_txt);
	}
}

void Core::refresh_scope(int32_t icolor)
{
	if (pl1_score != nullptr) {
		SDL_FreeSurface(pl1_score);
	}

	if (pl2_score != nullptr) {
		SDL_FreeSurface(pl2_score);
	}

	int32_t	pl1 = dynamic_cast<Player*>(objects[1].get())->get_score();
	int32_t	pl2 = dynamic_cast<Player*>(objects[2].get())->get_score();

	std::string pl1_score_str = std::to_string(pl1);
	std::string pl2_score_str = std::to_string(pl2);

	SDL_Color color{ 0xFF, 0xFF, 0xFF };

	if (!(pl1_score = TTF_RenderText_Blended(font_score, pl1_score_str.c_str(), color))) {
		throw std::exception("Render TTF error!");
	}
	if (!(pl2_score = TTF_RenderText_Blended(font_score, pl2_score_str.c_str(), color))) {
		throw std::exception("Render TTF error!");
	}
}

void Core::check_collision()
{
	Boll*	boll = dynamic_cast<Boll*>(objects[0].get());
	Player*	pl1 = dynamic_cast<Player*>(objects[1].get());
	Player*	pl2 = dynamic_cast<Player*>(objects[2].get());

	switch (is_collision(boll, pl1, pl2))
	{
	case Side::LEFT:
		pl2->up_score();
		pl1->speed_up();
		refresh_scope();
		boll->reset(screen.w, screen.h);
		play_audio(peeeeeep);
		break;
	case Side::RIGHT:
		pl1->up_score();
		pl2->speed_up();
		refresh_scope();
		boll->reset(screen.w, screen.h);
		play_audio(peeeeeep);
		break;
	case Side::UP:
		boll->speed_up();
		boll->origin = reflect(boll->origin, Vec2(0, 1));
		play_audio(plop);
		break;
	case Side::DOWN:
		boll->speed_up();
		boll->origin = reflect(boll->origin, Vec2(0, -1));
		play_audio(plop);
		break;
	case Side::PL_ONE:
		boll->speed_up();
		boll->x = pl1->x + pl1->w;
		boll->origin = reflect(boll->origin, Vec2(-1, 0));
		play_audio(beeep);
		break;
	case Side::PL_TWO:
		boll->speed_up();
		boll->x = pl2->x - boll->w;
		boll->origin = reflect(boll->origin, Vec2(1, 0));
		play_audio(beeep);
		break;
	default:
		break;
	}
}

Side Core::is_collision(const Boll *boll, Player *pl1, Player *pl2)
{
	if (boll->x == 0) {
		return Side::LEFT;
	}
	else if (boll->x == screen.w - boll->w) {
		return Side::RIGHT;
	}
	else if (boll->y == 0) {
		return Side::UP;
	}
	else if (boll->y == screen.h - boll->h) {
		return Side::DOWN;
	}
	else if (Obj::AABB_Collision(*pl1, *boll)) {
		return Side::PL_ONE;
	}
	else if (Obj::AABB_Collision(*pl2, *boll)) {
		return Side::PL_TWO;
	}
	else {
		return Side::SPACE;
	}
	return Side::SPACE;
}

void Core::load_audio()
{
	beeep = Mix_LoadWAV(PATH_BEEEP);
	if (!beeep) {
		printf("Mix_LoadWAV: %s\n", Mix_GetError());
		// handle error
	}
	peeeeeep = Mix_LoadWAV(PATH_PEEEEEEP);
	if (!peeeeeep) {
		printf("Mix_LoadWAV: %s\n", Mix_GetError());
		// handle error
	}
	plop = Mix_LoadWAV(PATH_PLOP);
	if (!plop) {
		printf("Mix_LoadWAV: %s\n", Mix_GetError());
		// handle error
	}
}

void Core::play_audio(Mix_Chunk * chunk)
{
	if (Mix_PlayChannel(-1, chunk, 0) == -1) {
		printf("Mix_PlayChannel: %s\n", Mix_GetError());
		// may be critical error, or maybe just no channels were free.
		// you could allocated another channel in that case...
	}
}

Menu * Core::make_menu(TTF_Font * font_menu)
{
	return new Menu{ font_menu };
}
