#pragma once
#include "olcPixelGameEngine.h"


class Paddle {
public:
	Paddle();
	void render(olc::PixelGameEngine* pge);
	void setPosition(float x, float y);

	int width, height;
	float posx, posy;
	olc::Pixel color;
};