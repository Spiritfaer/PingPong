#pragma once

#include "Player.h"
#include "Boll.h"

class Bot :	public Player
{
public:
	Bot(uint16_t x, uint16_t y, SDL_Scancode up, SDL_Scancode down, int32_t color = 0xFF0000);
	~Bot();

	void	move(const SDL_Event & event, int32_t max_w, int32_t max_h, float speed) override;
	void	refresh_dest(const Boll* boll);

private:
	Side	move_to_dest();
	int32_t dest;
};
