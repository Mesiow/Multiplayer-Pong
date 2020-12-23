#pragma once
#include "olcPixelGameEngine.h"
#include "Pong.h"

class Ball {
public:
	Ball();
	void render(olc::PixelGameEngine* pge);

	void setPosition(float x, float y);

	int size;
	float posX, posY;
};