#pragma once

#include <stdint.h>
#include <iostream>
#include <SDL.h>

enum class Side { SPACE, LEFT, RIGHT, UP, DOWN, PL_ONE, PL_TWO };

const float SPEED_UP_VALUE = 0.50f;
const float BASE_SPEED = 5.0f;

class Obj
{
public:
	Obj(uint16_t in_x, uint16_t in_y, uint16_t in_w, uint16_t in_h, int32_t color = 0xFF0000, float in_speed = BASE_SPEED);
	Obj(const Obj & ref);
	virtual ~Obj();

	void obj_draw(const SDL_Rect &screen, int32_t *pixels);
	void obj_clean(const SDL_Rect &screen, int32_t *pixels, uint32_t BG_icolor = 0x000000);

	virtual void move(const SDL_Event & event, int32_t max_w, int32_t max_h, float speed);

	static bool AABB_Collision(const Obj& aa, const Obj& bb);

	void move_up(float value = 1);
	void move_down(int32_t max_h, float value = 1);
	void move_left(float value = 1);
	void move_right(int32_t max_w, float value = 1);
	void speed_up();

	float	x, y, w, h;
	float	speed;
	int32_t	icolor;
};
