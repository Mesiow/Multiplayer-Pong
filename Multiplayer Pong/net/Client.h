#pragma once
#include "Net.h"
#include "PacketTypes.h"
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
	bool connected_ = false;
	Ball ball_;


	//Server info
	sf::IpAddress remoteAddress_;
	uint16_t remotePort_;

	olc::PixelGameEngine* pge_; //reference to engine
};