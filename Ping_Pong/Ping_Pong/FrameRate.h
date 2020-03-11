#pragma once

#include <SDL.h>

class FrameRate
{
public:
	FrameRate();
	~FrameRate();

	bool tick(bool db = false);
private:
	uint32_t old_time;
	uint32_t cur_time;
	uint32_t ticks;

	void showFPS(uint32_t t);
};
