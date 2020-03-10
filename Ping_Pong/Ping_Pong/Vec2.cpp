#include "vec2.h"

Vec2::Vec2(): x(0.f), y(0.f) {}
Vec2::Vec2(float in) : x(in), y(in) {}
Vec2::Vec2(float in_x, float in_y) : x(in_x), y(in_y) {}
Vec2::~Vec2() {}

float Vec2::length()
{
	return static_cast<float>(sqrt((x * x) + (y * y)));
}

void Vec2::norm()
{
	float inv_length = (1 / length());
	x *= inv_length;
	y *= inv_length;
}

Vec2 Vec2::normalize()
{
	float inv_length = (1 / length());
	return Vec2(x * inv_length, y * inv_length);
}

Vec2 operator+(const Vec2 & a, const Vec2 & b)
{
	return Vec2(a.x + b.x, a.y + b.y);
}

Vec2 operator-(const Vec2 & a, const Vec2 & b)
{
	return Vec2(a.x - b.x, a.y - b.y);
}

Vec2 operator*(const Vec2 & a, const Vec2 & b)
{
	return Vec2(a.x * b.x, a.y * b.y);
}

Vec2 operator*(const Vec2 & a, float f)
{
	return Vec2(a.x * f, a.y * f);
}

Vec2 operator/(const Vec2 & a, const Vec2 & b)
{
	if (!b.x, !b.y)
		throw std::exception("Division by zero");
	return Vec2(a.x / b.x, a.y / b.y);
}

Vec2 operator/(const Vec2 & a, float f)
{
	if (f == 0)
		throw std::exception("Division by zero");
	return Vec2(a.x / f, a.y / f);
}

float dot(const Vec2 &v1, const Vec2 &v2)
{
	return static_cast<float>(v1.x * v2.x + v1.y * v2.y);
}

Vec2 reflect(const Vec2 & vector, const Vec2 & normal)
{
	return (vector + (normal * -(dot(vector, normal) * 2)));
}

void wedge(Vec2 & ref, float min, float max)
{
	if (ref.x < 0)
		ref.x = ref.x > min ? min : ref.x;
	else if (ref.x > 0)
		ref.x = ref.x < max ? max : ref.x;
	if (ref.y < 0)
		ref.y = ref.y > min ? min : ref.y;
	else if (ref.y > 0)
		ref.y = ref.y < max ? max : ref.y;
}
