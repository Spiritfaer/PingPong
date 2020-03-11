#pragma once

#include "Obj.h"

class Player : public Obj
{
public:
	
	Player(uint16_t x, uint16_t y, SDL_Scancode up, SDL_Scancode down, int32_t color = 0xFF0000);
	~Player();

	virtual void	move(const SDL_Event & event, int32_t max_w, int32_t max_h, float speed) override;
	int32_t			get_score() const;
	void			up_score();
protected:
	SDL_Scancode	up;
	SDL_Scancode	down;
	int32_t			score;
};
