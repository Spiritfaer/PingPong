#include <iostream>
#include "FrameRate.h"

FrameRate::FrameRate()
{
	old_time = SDL_GetTicks();
	ticks = cur_time = 0;
}

FrameRate::~FrameRate() {}

bool FrameRate::tick(bool db)
{
	cur_time = SDL_GetTicks();
	++ticks;
	uint32_t tmp = cur_time - old_time;
	if (tmp > 1000) {
		old_time = cur_time;
		if (db)
			showFPS(ticks);
		ticks = 0;
	}
	return true;
}

void FrameRate::showFPS(uint32_t t)
{
	std::cout << "FPS = " << t << std::endl;
}
