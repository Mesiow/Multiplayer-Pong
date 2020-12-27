#pragma once
#include "olcPixelGameEngine.h"
#include "Pong.h"
#include <array>

class Scoreboard {
public:
	enum class Player {
		One,
		Two
	};
	Scoreboard();
	void render(olc::PixelGameEngine* pge);

	void addToScore(Player player, int point);


	std::array<int, MAX_CONNECTIONS> scores;
};