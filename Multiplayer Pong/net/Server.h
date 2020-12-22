#pragma once
#include "Net.h"
#include "PacketTypes.h"
#include "../Pong.h"
#include <array>

struct ClientEndPoint {
	sf::IpAddress address;
	uint16_t port;
};

class Server : Net{
public:
	Server();
	~Server();
	void run();
	void create(uint16_t port);

private:
	void receivePackets()override;
	void sendPacket(sf::Packet& packet)override;
	void handleConnection(sf::IpAddress &address, uint16_t port);

	int8_t emptySlot();


private:
	std::array<ClientEndPoint, MAX_CONNECTIONS> clients_;
	std::array<bool, MAX_CONNECTIONS> connects_;
	bool running_ = false;
};