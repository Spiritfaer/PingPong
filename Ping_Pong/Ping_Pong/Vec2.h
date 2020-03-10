#include <stdint.h>
#include <math.h>
#include <exception>
#pragma once

class Vec2
{
public:
	Vec2();
	explicit Vec2(float in);
	Vec2(float in_x, float in_y);
	
	~Vec2();

	float	length();
	void	norm();
	Vec2	normalize();

	float	x;
	float	y;
};

Vec2	operator+(const Vec2& a, const Vec2& b);
Vec2	operator-(const Vec2& a, const Vec2& b);
Vec2	operator*(const Vec2& a, const Vec2& b);
Vec2	operator*(const Vec2& a, float f);
Vec2	operator/(const Vec2& a, const Vec2& b);
Vec2	operator/(const Vec2& a, float f);

float	dot(const Vec2 &v1, const Vec2 &v2);
Vec2	reflect(const Vec2& vector, const Vec2& normal);
void	wedge(Vec2 &ref, float min, float max);