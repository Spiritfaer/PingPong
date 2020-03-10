#include <algorithm>
#include "boll.h"

Boll::Boll(float x, float y, float w, float h, int32_t color) : Obj(x, y, w, h, color)
{
	origin.x = static_cast<float>(RAND_MAX / 2 - rand());
	origin.y = static_cast<float>(RAND_MAX / 2 - rand());
	origin.norm();
	wedge(origin, MIN_WEDGE, MAX_WEDGE);
}

Boll::Boll(const SDL_Rect & ref) : Boll(ref.x, ref.y, ref.w, ref.h) {}
Boll::~Boll() {}

void Boll::reset(int32_t max_w, int32_t max_h)
{
	x = max_w / 2 - 25 / 2;
	y = max_h / 2 - 25 / 2;
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
