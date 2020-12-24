#pragma once
#include "Net.h"
#include "PacketTypes.h"
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

	void handlePacketReceive(sf::Packet& packet);
	void connectToServer();
	void disconnectFromServer();

private:
	int clientID = 0;
	bool connected_ = false;

	std::array<Paddle, MAX_CONNECTIONS> peers_;
	std::array<bool, MAX_CONNECTIONS> connects_;
	Ball ball_;


	//Server info
	sf::IpAddress remoteAddress_;
	uint16_t remotePort_;

	olc::PixelGameEngine* pge_; //reference to engine
};