#include "Client.h"
#include <iostream>

Client::Client()
{
	address_ = sf::IpAddress("127.0.0.1");
	port_ = 5400;

	remoteAddress_ = sf::IpAddress::LocalHost;
	remotePort_ = 7777;

	if (socket_.bind(port_, address_) != sf::Socket::Done) {
		std::cerr << "Client socket failed to create\n";
		return;
	}
	else {
		std::cout << "Client setup successfully\n";

		//send test packet to server
		sf::Packet p;
		p << CommandToServer::Connect;
		socket_.send(p, remoteAddress_, remotePort_);
	}
}

void Client::run()
{
}

void Client::receivePackets()
{
	sf::Packet packet;
	sf::IpAddress sender;
	uint16_t port;
	if(socket_.receive(packet, sender, port) == sf::Socket::Done){
		std::cout << "Packet of size " << packet.getDataSize() << " received from "
			<< sender << " on port " << port << std::endl;
	}
}

void Client::sendPacket(sf::Packet& packet)
{

}
