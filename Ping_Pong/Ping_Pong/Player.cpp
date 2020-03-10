#include "Player.h"

Player::Player(uint16_t x, uint16_t y, SDL_Scancode up, SDL_Scancode down, int32_t color) :
	Obj(x, y, 15, 100, color)
{
	score = 0;
	speed = 8;
	this->up = up;
	this->down = down;
}
Player::~Player(){}

void Player::move(const SDL_Event & event, int32_t max_w, int32_t max_h, float speed)
{
	this->speed = speed;
	const uint8_t *state = SDL_GetKeyboardState(nullptr);

	if (state[up]) {
		move_up(-1);
	}
	if (state[down]) {
		move_down(max_h, 1);
	}
}

int32_t Player::get_score() const
{
	return score;
}

void Player::up_score()
{
	++score;
}
