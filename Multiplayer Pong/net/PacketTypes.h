#pragma once
#include <SFML/Network/Packet.hpp>
#include <cstdint>

enum class CommandToServer : uint8_t {
	Connect = 0,
	Disconnect = 1,

	ClientInput
};

enum class CommandToClient : uint8_t {
	PaddleState = 0,
	BallState = 1,

	ConnectRequestResult
};

enum Input : uint8_t{
	Up = 0x1,
	Down = 0x2
};

sf::Packet& operator<<(sf::Packet& packet, CommandToServer &command);
sf::Packet& operator>>(sf::Packet& packet, CommandToServer &command);
sf::Packet& operator<<(sf::Packet& packet, CommandToClient& command);
sf::Packet& operator>>(sf::Packet& packet, CommandToClient& command);

