#pragma once
#include <SFML/Network.hpp>

class Net {
public:
	Net() = default;
	virtual void receivePackets() = 0;
	virtual void sendPacket(sf::Packet& packet) = 0;

protected:
	sf::UdpSocket socket_;
	sf::IpAddress address_;
	uint16_t port_;
};