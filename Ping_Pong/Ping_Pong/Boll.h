#pragma once
#include "Obj.h"
#include "Vec2.h"


const float MIN_WEDGE = -0.65f;
const float MAX_WEDGE = std::abs(-0.65f);


class Boll : public Obj
{
public:
	Boll(float x, float y, float w, float h, int32_t color = 0xFF0000);
	Boll(const SDL_Rect &ref);
	~Boll();

	void reset(int32_t max_w, int32_t max_h);
	void move(const SDL_Event & event, int32_t max_w, int32_t max_h, float speed) override;
	
	Vec2 origin;
};
