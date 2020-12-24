#include "Paddle.h"
#include "Pong.h"

Paddle::Paddle()
{
	width = PADDLE_WIDTH;
	height = PADDLE_HEIGHT;
	posx = 0; posy = 0;
}

void Paddle::render(olc::PixelGameEngine* pge)
{
	pge->FillRect(posx, posy, width, height, olc::GREEN);
}

void Paddle::setPosition(float x, float y)
{
	posx = x; posy = y;
}
