#include "Server.h"
#include <iostream>

Server::Server()
{
	address_ = sf::IpAddress("127.0.0.1");
	port_ = 0;
	connects_.fill(false);
}

Server::~Server()
{
}

void Server::run()
{
	std::cout << "Server Running\n";
	running_ = true;
	while (running_) {
		//receive packets
		receivePackets();
		//update state
		//send state
	}
}

void Server::create(uint16_t port)
{
	this->port_ = port;
	//bind socket to local host
	if (socket_.bind(port, address_) != sf::Socket::Done) {
		std::cerr << "Error occured binding server UDP Socket\n";
		return;
	}
	else {
		std::cout << "Server setup successfully\n";

	}
}

void Server::receivePackets()
{
	sf::Packet packet;
	sf::IpAddress address;
	uint16_t port;

	if (socket_.receive(packet, address, port) == sf::Socket::Done) {
		CommandToServer command;
		packet >> command;

		switch (command) {
			case CommandToServer::Connect: {
				handleConnection(address, port);
			}break;
		}
	}
}

void Server::sendPacket(sf::Packet& packet)
{

}

void Server::handleConnection(sf::IpAddress& address, uint16_t port)
{
	//Store new client information
	ClientEndPoint newClient;
	newClient.address = address;
	newClient.port = port;

	int8_t slot = emptySlot();
	if (slot == -1) {
		std::cerr << "No Slots left for new Client\n";
		return;
	}
	clients_[slot] = newClient;


	std::cout << "New Client from " << clients_[slot].address.toString() 
		<< ": " << clients_[slot].port << std::endl;

	//setup paddle position for new client

	connects_[slot] = true;

}

int8_t Server::emptySlot()
{
	for (uint8_t i = 0; i < MAX_CONNECTIONS; i++) {
		if (!connects_[i])
			return i;
	}
	return -1;
}
