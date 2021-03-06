#pragma once
#include "Net.h"
#include "PacketTypes.h"
#include "../Scoreboard.h"
#include "../Paddle.h"
#include "../Ball.h"

class Client : Net{
public:
	Client(olc::PixelGameEngine *pge);
	~Client();
	void run();

private:
	void receivePackets()override;
	void sendPacket(sf::Packet& packet)override;
	void sendBroadcast(); //sends broadcast to all lan computers to obtain the server's ip address

	void handlePacketReceive(sf::Packet& packet);
	void connectToServer();
	void disconnectFromServer();

private:
	sf::Uint8 clientID;
	bool connected_ = false;

	std::array<Paddle, MAX_CONNECTIONS> peers_;
	std::array<bool, MAX_CONNECTIONS> connects_;

	Scoreboard sboard_;
	Ball ball_;


	//Server info
	sf::IpAddress remoteAddress_;
	uint16_t remotePort_;

	olc::PixelGameEngine* pge_; //reference to engine
};