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
	if (TTF_Init() == -1) {
		throw std::exception("ERROR! TTF_INIT\n");
	}
	font = TTF_OpenFont("O.K.Retro.otf", FONT_SIZE);
	if (!font) {
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
}


Core::~Core()
{
	std::cout << "DB INFO : ~Core()" << std::endl;
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

void *Core::getPixels()
{
	return canvas->pixels;
}

void Core::refresh_event()
{
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
	if (event.type == SDL_QUIT
		|| event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
		return true;
	return false;
}

void Core::add_new_obj(std::unique_ptr<Obj> ptr)
{
	objects.push_back(std::move(ptr));
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

	if (!(pl1_score = TTF_RenderText_Blended(font, pl1_score_str.c_str(), color))) {
		throw std::exception("Render TTF error!");
	}
	if (!(pl2_score = TTF_RenderText_Blended(font, pl2_score_str.c_str(), color))) {
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
		refresh_scope();
		boll->reset(screen.w, screen.h);
		play_audio(peeeeeep);
		break;
	case Side::RIGHT:
		pl1->up_score();
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
