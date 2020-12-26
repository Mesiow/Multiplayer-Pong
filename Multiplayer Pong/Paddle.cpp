#include "Paddle.h"
#include "Pong.h"

Paddle::Paddle()
{
	width = PADDLE_WIDTH;
	height = PADDLE_HEIGHT;
	posx = 0; posy = 0;
	color = olc::Pixel(59, 227, 7, 255); //light green
}

void Paddle::render(olc::PixelGameEngine* pge)
{
	//pge->FillRect(posx, posy, width, height, color);
	pge->GradientFillRectDecal(olc::vi2d(posx, posy), olc::vi2d(width, height),
		olc::GREEN, olc::WHITE, olc::RED, olc::BLUE);
}

void Paddle::setPosition(float x, float y)
{
	posx = x; posy = y;
}
