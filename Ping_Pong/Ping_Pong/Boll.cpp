#include <algorithm>
#include "boll.h"

Boll::Boll(float x, float y, float w, float h, int32_t color)
	: Obj(static_cast<uint16_t>(x), static_cast<uint16_t>(y), static_cast<uint16_t>(w), static_cast<uint16_t>(h), color)
{
	origin.x = static_cast<float>(RAND_MAX / 2 - rand());
	origin.y = static_cast<float>(RAND_MAX / 2 - rand());
	origin.norm();
	wedge(origin, MIN_WEDGE, MAX_WEDGE);
}

Boll::Boll(const SDL_Rect & ref)
	: Boll(static_cast<uint16_t>(ref.x), static_cast<uint16_t>(ref.y), static_cast<uint16_t>(ref.w), static_cast<uint16_t>(ref.h)) {}
Boll::~Boll() {}

void Boll::reset(int32_t max_w, int32_t max_h)
{
	x = static_cast<float>(max_w) / 2.0f - 25.0f / 2.0f;
	y = static_cast<float>(max_h) / 2.0f - 25.0f / 2.0f;
	origin.x = static_cast<float>(RAND_MAX / 2 - rand());
	origin.y = static_cast<float>(RAND_MAX / 2 - rand());
	origin.norm();
	wedge(origin, MIN_WEDGE, MAX_WEDGE);
	speed = BASE_SPEED;
}

void Boll::move(const SDL_Event & event, int32_t max_w, int32_t max_h, float speed)
{
	if (origin.x < 0)
		move_left(origin.x);
	else
		move_right(max_w, origin.x);
	if (origin.y < 0)
		move_up(origin.y);
	else
		move_down(max_h, origin.y);
}
