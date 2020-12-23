#pragma once
#include <SFML/Network/Packet.hpp>
#include <cstdint>

enum class CommandToServer : uint8_t {
	Connect = 0,
	Disconnect = 1,
};

enum class CommandToClient : uint8_t {
	PaddleState = 0,
	BallState = 1,

	ConnectRequestResult
};

sf::Packet& operator<<(sf::Packet& packet, CommandToServer &command);
sf::Packet& operator>>(sf::Packet& packet, CommandToServer &command);
sf::Packet& operator<<(sf::Packet& packet, CommandToClient& command);
sf::Packet& operator>>(sf::Packet& packet, CommandToClient& command);

