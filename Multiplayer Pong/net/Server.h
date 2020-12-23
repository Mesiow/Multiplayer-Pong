#pragma once
#include "Net.h"
#include "PacketTypes.h"
#include "../Pong.h"
#include <array>

struct ClientEndPoint {
	sf::IpAddress address;
	uint16_t port;
};

struct BallState {
	float velX, velY;
	float posX, posY;
};

class Server : Net{
public:
	Server();
	~Server();
	void run();
	void create(uint16_t port);

private:
	void receivePackets()override;
	void updateState();
	void sendState();

	void sendPacket(sf::Packet& packet, ClientEndPoint client);
	void handleConnection(sf::IpAddress &address, uint16_t port);
	void handleDisconnection(sf::IpAddress& address, uint16_t port);

	int emptySlot();


private:
	std::array<ClientEndPoint, MAX_CONNECTIONS> clients_;
	std::array<bool, MAX_CONNECTIONS> connects_;
	bool running_ = false;

	BallState bstate_;
};