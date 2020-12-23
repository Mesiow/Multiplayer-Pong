#include "Server.h"
#include <iostream>

Server::Server()
{
	socket_.setBlocking(false);
	address_ = sf::IpAddress("127.0.0.1");
	port_ = 0;
	connects_.fill(false);

	//Ball State
	bstate_.posX = SCREEN_WIDTH / 2;
	bstate_.posY = SCREEN_HEIGHT / 2;
	bstate_.velX = 50;
	bstate_.velY = 25;
}

Server::~Server()
{
}

void Server::run()
{
	std::cout << "Server Running\n";
	running_ = true;
	while (running_) {
		receivePackets();
		//updateState();
		//sendState();
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

			case CommandToServer::Disconnect: {
				handleDisconnection(address, port);
			}break;
		}
	}
}

void Server::updateState()
{


	//update ball
	bstate_.posX += bstate_.velX * (1.0f / 60.0f);
	bstate_.posY += bstate_.velY * (1.0f / 60.0f);

	//ball border collision
	if (bstate_.posX + BALL_SIZE > SCREEN_WIDTH || bstate_.posX <= 0) {
		bstate_.velX *= -1;
	}

	if (bstate_.posY + BALL_SIZE > SCREEN_HEIGHT || bstate_.posY <= 0) {
		bstate_.velY *= -1;
	}
}

void Server::sendState()
{

}

void Server::sendPacket(sf::Packet& packet, ClientEndPoint client)
{
	socket_.send(packet, client.address, client.port);
}

void Server::handleConnection(sf::IpAddress& address, uint16_t port)
{
	//Store new client information
	ClientEndPoint newClient;
	newClient.address = address;
	newClient.port = port;

	int slot = emptySlot();
	if (slot == -1) {
		log_error("No Slots left for new Client");
		return;
	}
	std::cout << "slot: " << slot << "\n";
	clients_[slot] = newClient;

	sf::Packet connectionPacket;
	connectionPacket << (uint8_t)CommandToClient::ConnectRequestResult << (uint8_t)1
		>> slot;

	//send connection request success packet to client, plus their slot id
	sendPacket(connectionPacket, clients_[slot]);
	log("New Client connected from", clients_[slot].address, clients_[slot].port);

	//setup paddle position for new client

	connects_[slot] = true;

}

void Server::handleDisconnection(sf::IpAddress& address, uint16_t port)
{
	for (uint8_t i = 0; i < MAX_CONNECTIONS; i++) {
		if (clients_[i].address == address
			&& clients_[i].port == port) {
			clients_.at(i) = ClientEndPoint(); //reset
			connects_[i] = false;
			std::cout << "Client " << address.toString() << ": " << port << " disconnected\n";
		}
	}
}

int Server::emptySlot()
{
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		if (!connects_[i])
			return i;
	}
	return -1;
}
