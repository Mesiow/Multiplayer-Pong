#pragma once
#include "Net.h"
#include "PacketTypes.h"

class Client : Net{
public:
	Client();
	void run();

private:
	void receivePackets()override;
	void sendPacket(sf::Packet& packet)override;

private:

	//Server info
	sf::IpAddress remoteAddress_;
	uint16_t remotePort_;
};