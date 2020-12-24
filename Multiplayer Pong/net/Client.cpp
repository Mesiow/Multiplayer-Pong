#include "Client.h"
#include <iostream>

Client::Client(olc::PixelGameEngine *pge)
{
	this->pge_ = pge;

	address_ = sf::IpAddress::LocalHost;
	port_ = 5400;

	remoteAddress_ = sf::IpAddress::LocalHost;
	remotePort_ = 7777;
	if (socket_.bind(sf::Socket::AnyPort, address_) != sf::Socket::Done) {
		log_error("Client socket failed to create");
		return;
	}
	else {
		clog("Client setup successfully");
		connectToServer();
	}
}

Client::~Client()
{
	disconnectFromServer();
}

void Client::run()
{
	if (!connected_)
		return;

	//handle input

	//receive packets
	receivePackets();

	//render
	ball_.render(pge_);
}

void Client::receivePackets()
{
	sf::Packet packet;
	sf::IpAddress sender;
	uint16_t port;
	if(socket_.receive(packet, sender, port) == sf::Socket::Done){
		std::cout << "Packet of size " << packet.getDataSize() << " received from "
			<< sender << " on port " << port << std::endl;

		handlePacketReceive(packet);
	}
}

void Client::sendPacket(sf::Packet& packet)
{
	socket_.send(packet, remoteAddress_, remotePort_);
}

void Client::handlePacketReceive(sf::Packet& packet)
{
	CommandToClient command;
	packet >> command;
	switch (command) {
		case CommandToClient::BallState: {
			float posx, posy;
			packet >> posx >> posy;
			ball_.setPosition(posx, posy);
		}break;

		case CommandToClient::PaddleState: {

		}break;
	}
}

void Client::connectToServer()
{
	//Send Connection packet to server
	sf::Packet packet;
	packet << (uint8_t)CommandToServer::Connect;
	sendPacket(packet);


	//Receive connection result from the server
	sf::Packet recv_packet;
	sf::IpAddress address;
	uint16_t port;

	if (socket_.receive(recv_packet, address, port) == sf::Socket::Done) {
		CommandToClient command;
		uint8_t result;
		recv_packet >> command >> result;

		//if result is 1(true), connection allowed
		if (result) {
			connected_ = true;

			//setup peer objects/sprites
			std::cout << "Connection request accepted\n";

			socket_.setBlocking(false);
		}
	}
}

void Client::disconnectFromServer()
{
	sf::Packet disconnect;
	disconnect << (uint8_t)CommandToServer::Disconnect;
	sendPacket(disconnect);

	socket_.unbind();
}
