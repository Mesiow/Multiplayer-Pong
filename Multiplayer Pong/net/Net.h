#pragma once
#include <SFML/Network.hpp>

#define log(msg, address, port) std::cout<<msg<<" "<<address<<":"<<port<<"\n";
#define clog(msg) std::cout<<msg<<"\n";
#define log_error(msg) std::cout<<msg<<"\n";

class Net {
public:
	Net() = default;
	virtual ~Net() {}
	virtual void receivePackets() = 0;
	virtual void sendPacket(sf::Packet& packet){}

protected:
	sf::UdpSocket socket_;
	sf::IpAddress address_;
	uint16_t port_;
};