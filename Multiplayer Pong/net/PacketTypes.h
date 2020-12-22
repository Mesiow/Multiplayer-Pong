#pragma once
#include <SFML/Network/Packet.hpp>
#include <cstdint>

enum CommandToServer : uint8_t {
	Connect = 0,
	Disconnect = 1,
};

enum CommandToClient : uint8_t {

};

sf::Packet& operator<<(sf::Packet& packet, CommandToServer &command);
sf::Packet& operator>>(sf::Packet& packet, CommandToServer &command);
sf::Packet& operator<<(sf::Packet& packet, CommandToClient& command);
sf::Packet& operator>>(sf::Packet& packet, CommandToClient& command);

