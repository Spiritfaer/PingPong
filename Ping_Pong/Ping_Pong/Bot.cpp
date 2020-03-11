#include "Bot.h"

Bot::Bot(uint16_t x, uint16_t y, SDL_Scancode up, SDL_Scancode down, int32_t color) :
	Player(x, y, up, down, color) {}
Bot::~Bot() {}

void Bot::move(const SDL_Event & event, int32_t max_w, int32_t max_h, float speed)
{
	this->speed = speed;

	switch (move_to_dest()) {
	case Side::DOWN:
		move_down(max_h, 1);
		break;
	case Side::UP:
		move_up(-1);
		break;
	default:
		break;
	}
}

Side Bot::move_to_dest()
{
	if (dest > y + h) {
		return Side::DOWN;
	}
	else if (dest < y) {
		return Side::UP;
	}
	else
		return Side::SPACE;
}

void Bot::refresh_dest(const Boll* boll)
{
	dest = (boll->y + boll->h / 2);
}
