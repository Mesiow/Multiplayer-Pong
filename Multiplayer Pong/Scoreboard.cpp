#include "Scoreboard.h"

Scoreboard::Scoreboard()
{
	scores.fill(0);
}

void Scoreboard::render(olc::PixelGameEngine* pge)
{
	int offset = 50;
	pge->DrawStringDecal(olc::vf2d(SCREEN_WIDTH / 2 - offset, 15), std::to_string(scores[(int)Player::One]));
	pge->DrawStringDecal(olc::vf2d(SCREEN_WIDTH / 2 + offset, 15), std::to_string(scores[(int)Player::Two]));
}

void Scoreboard::addToScore(Player player, int point)
{
	switch (player) {
		case Player::One: scores[(int)Player::One] += point; break;
		case Player::Two: scores[(int)Player::Two] += point; break;
	}
}
