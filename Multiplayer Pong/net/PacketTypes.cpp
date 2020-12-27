#include "PacketTypes.h"
#include "../Scoreboard.h"

sf::Packet& operator<<(sf::Packet& packet, CommandToServer &command)
{
    packet << (uint8_t)command;
    return packet;
}

sf::Packet& operator>>(sf::Packet& packet, CommandToServer &command)
{
    uint8_t commandId;
    packet >> commandId;
    command = (CommandToServer)commandId;
    return packet;
}

sf::Packet& operator<<(sf::Packet& packet, CommandToClient& command)
{
    packet << (uint8_t)command;
    return packet;
}

sf::Packet& operator>>(sf::Packet& packet, CommandToClient& command)
{
    uint8_t commandId;
    packet >> commandId;
    command = (CommandToClient)commandId;
    return packet;
}

sf::Packet& operator>>(sf::Packet& packet, Player& player)
{
    int playerId;
    packet >> playerId;
    player = (Player)playerId;
    return packet;
}
