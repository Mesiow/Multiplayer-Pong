#include "Ball.h"

Ball::Ball()
{
	size = BALL_SIZE;
	posX = 0; posY = 0;
}

void Ball::render(olc::PixelGameEngine* pge)
{
	pge->FillRect(posX, posY, size, size);
}

void Ball::setPosition(float x, float y)
{
	posX = x; posY = y;
}
