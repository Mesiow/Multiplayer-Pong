#pragma once
#include "olcPixelGameEngine.h"
#include "net/Client.h"

// Override base class with your custom functionality
class Game : public olc::PixelGameEngine
{
public:
	Game()
	{
		sAppName = "Multiplayer Pong";
	}

public:
	bool OnUserCreate() override
	{
		client_ = new Client();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		
		return true;
	}

private:
	Client* client_;
};
