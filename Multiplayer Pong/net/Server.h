#pragma once
#include "Net.h"
#include "PacketTypes.h"
#include "../Pong.h"
#include <array>
#include <thread>
#include <chrono>

enum class Player;

struct ClientInput {
	int up;
	int down;
};

struct ClientEndPoint {
	sf::IpAddress address;
	uint16_t port;
};

struct ObjectState {
	float velX, velY;
	float posX, posY;

	void reset(float x, float y) { //reset object to a specific position
		posX = x; posY = y;
	}
};

struct ScoreState {
	Player player;
	bool playerScored;
};

class Server : Net{
public:
	Server();
	~Server();
	void run();
	void create(uint16_t port);

	static uint16_t getPort();

private:
	void receivePackets()override;
	void updateState();
	void sendState();

	void sendPacket(sf::Packet& packet, ClientEndPoint &client);
	void handleConnection(sf::IpAddress &address, uint16_t port);
	void handleDisconnection(sf::IpAddress& address, uint16_t port);
	void handleBroadcast(sf::IpAddress &address, uint16_t port);
	void handleClientInput(sf::Packet& packet);

	int connectedClients(); //returns current number of connected clients
	int emptySlot();


private:
	std::array<ClientEndPoint, MAX_CONNECTIONS> clients_;
	std::array<ClientInput, MAX_CONNECTIONS> clientInputs_;
	std::array<bool, MAX_CONNECTIONS> connects_;
	std::array<ObjectState, MAX_CONNECTIONS> paddleStates_;
	bool running_ = false;

	ObjectState bstate_;
	ScoreState sstate_;

	static uint16_t port;
};