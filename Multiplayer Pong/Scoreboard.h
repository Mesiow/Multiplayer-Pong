#pragma once
#include "olcPixelGameEngine.h"
#include "Pong.h"
#include <array>

enum class Player;

class Scoreboard {
public:
	Scoreboard();
	void render(olc::PixelGameEngine* pge);
	void addToScore(Player player, int point);


	std::array<int, MAX_CONNECTIONS> scores;
};