#include "Client.h"
#include "Server.h"
#include <iostream>

Client::Client(olc::PixelGameEngine *pge)
{
	this->pge_ = pge;

	connects_.fill(false);
	address_ = sf::IpAddress::getLocalAddress();

	remotePort_ = Server::getPort();
	//Bind udp socket
	if (socket_.bind(sf::Socket::AnyPort, address_) != sf::Socket::Done) {
		log_error("Client socket failed to create");
		return;
	}
	else {
		sendBroadcast();
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
	uint8_t input = 0;
	if (pge_->GetKey(olc::Key::W).bHeld) {
		input |= Input::Up;
	}
	if (pge_->GetKey(olc::Key::S).bHeld) {
		input |= Input::Down;
	}

	//send input
	sf::Packet inputPacket;
	inputPacket << (uint8_t)CommandToServer::ClientInput << clientID << input; //send input, with client id
	sendPacket(inputPacket);

	//receive packets
	receivePackets();

	//render
	for (size_t i = 0; i < MAX_CONNECTIONS; i++) {
		if (connects_[i]) {
			peers_[i].render(pge_);
		}
	}
	ball_.render(pge_);
}

void Client::receivePackets()
{
	sf::Packet packet;
	sf::IpAddress sender;
	uint16_t port;
	while(socket_.receive(packet, sender, port) == sf::Socket::Done){
		handlePacketReceive(packet);
	}
}

void Client::sendPacket(sf::Packet& packet)
{
	socket_.send(packet, remoteAddress_, remotePort_);
}

void Client::sendBroadcast()
{
	//Broadcast data to get the server ip
	sf::Packet broadcastPacket;
	broadcastPacket << (uint8_t)CommandToServer::Broadcast;

	if (socket_.send(broadcastPacket, sf::IpAddress::Broadcast, remotePort_) == sf::Socket::Done) {
		sf::Packet recv_packet;
		sf::IpAddress serverAddress;
		uint16_t port;
		if (socket_.receive(recv_packet, serverAddress, port) == sf::Socket::Done) {
			CommandToClient command;
			uint8_t result;
			recv_packet >> command >> result;

			if (command == CommandToClient::BroadcastRequestResult) {
				if (result) {
					//Save server information
					remoteAddress_ = serverAddress;
					remotePort_ = port;
				}
			}
		}
	}
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
			uint8_t id;
			float posx, posy;
			
			while (packet >> id) {
				packet >> posx >> posy;
				connects_[id] = true;
				peers_[id].setPosition(posx, posy);
			}
			
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

	
	if(socket_.receive(recv_packet, address, port) == sf::Socket::Done) {
		CommandToClient command;
		uint8_t result;

		if (recv_packet >> command >> result >> clientID) {
			if (result) {
				connected_ = true;

				std::cout << "Client ID: " << (int)clientID << std::endl;

				//setup peer objects/sprites
				std::cout << "Connection request accepted\n";
				connects_[clientID] = true;

				socket_.setBlocking(false);
			}
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
