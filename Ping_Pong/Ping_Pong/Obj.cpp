#include "Obj.h"



Obj::Obj(uint16_t in_x, uint16_t in_y, uint16_t in_w, uint16_t in_h, int32_t color, float in_speed) :
	x(in_x), y(in_y), w(in_w), h(in_h), icolor(color), speed(in_speed)
{}

Obj::Obj(const Obj & ref)
	: x(ref.x), y(ref.y), w(ref.w), h(ref.h), icolor(ref.icolor), speed(ref.speed){}


Obj::~Obj()
{
	std::cout << "DB INFO : ~Obj()" << std::endl;
}

void Obj::obj_draw(const SDL_Rect & screen, int32_t * pixels)
{
	int32_t temp_x = static_cast<int32_t>(x);
	int32_t temp_y = static_cast<int32_t>(y);

	for (int32_t i = temp_x; i < w + temp_x; ++i) {
		for (int32_t j = temp_y; j < h + temp_y; ++j) {
			pixels[i + j * screen.w] = icolor;
		}
	}
}

void Obj::obj_clean(const SDL_Rect &screen, int32_t *pixels, uint32_t BG_icolor)
{
	int32_t temp_x = static_cast<int32_t>(x);
	int32_t temp_y = static_cast<int32_t>(y);

	for (int32_t i = temp_x; i < w + temp_x; ++i) {
		for (int32_t j = temp_y; j < h + temp_y; ++j) {
			pixels[i + j * screen.w] = BG_icolor;
		}
	}
}

void Obj::move(const SDL_Event & event, int32_t max_w, int32_t max_h, float speed)
{
	this->speed = speed;
	const uint8_t *state = SDL_GetKeyboardState(nullptr);

	if (state[SDL_SCANCODE_UP]) {
		move_up(-1);
	}
	if (state[SDL_SCANCODE_DOWN]) {
		move_down(max_h, 1);
	}
	if (state[SDL_SCANCODE_LEFT]) {
		move_left(-1);
	}
	if (state[SDL_SCANCODE_RIGHT]) {
		move_right(max_w, 1);
	}
}

bool Obj::AABB_Collision(const Obj &aa, const Obj &bb)
{
	if (aa.x < bb.x + bb.w &&
		aa.x + aa.w > bb.x &&
		aa.y < bb.y + bb.h &&
		aa.y + aa.h > bb.y) {
		return true;
	}
	return false;
}

void Obj::move_up(float value)
{
	if (y + value * speed >= 0 && value < 0) {
		y += value * speed;
	}
	else {
		y = 0;
	}
}

void Obj::move_down(int32_t max_h, float value)
{
	if (y + h < max_h) {
		if (y + h + value * speed > max_h)
			y = max_h - h;
		else
			y += value * speed;
	}
}

void Obj::move_left(float value)
{
	if (x + value * speed >= 0 && value < 0) {
		x += value * speed;
	}
	else {
		x = 0;
	}
}

void Obj::move_right(int32_t max_w, float value)
{
	if (x + w < max_w) {
		if (x + w + value * speed > max_w)
			x = max_w - w;
		else
			x += value * speed;
	}
}

void Obj::speed_up()
{
	speed += SPEED_UP_VALUE;
}
