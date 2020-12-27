#include "Server.h"
#include <iostream>

uint16_t Server::port = 7777;

Server::Server()
{
	socket_.setBlocking(false);
	connects_.fill(false);

	//Ball State
	bstate_.posX = SCREEN_WIDTH / 2;
	bstate_.posY = SCREEN_HEIGHT / 2;
	bstate_.velX = 60;
	bstate_.velY = 25;

	sstate_.playerScored = false;
}

Server::~Server()
{
}

void Server::run()
{
	std::cout << "Server Running\n";
	running_ = true;
	while (running_) {
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
		receivePackets();
		updateState();
		sendState();
	}
}

void Server::create(uint16_t port)
{
	address_ = sf::IpAddress::getLocalAddress();
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

uint16_t Server::getPort()
{
	return port;
}

void Server::receivePackets()
{
	sf::Packet packet;
	sf::IpAddress address;
	uint16_t port;

	while(socket_.receive(packet, address, port) == sf::Socket::Done) {
		CommandToServer command;
		packet >> command;

		switch (command) {
			case CommandToServer::Connect: {
				handleConnection(address, port);
			}break;

			case CommandToServer::Disconnect: {
				handleDisconnection(address, port);
			}break;

			case CommandToServer::Broadcast: {
				handleBroadcast(address, port);
			}break;

			case CommandToServer::ClientInput: {
				handleClientInput(packet);
			}break;
		}
	}
}

void Server::updateState()
{
	//update paddles
	for (size_t i = 0; i < MAX_CONNECTIONS; i++) {
		if (connects_[i]) {
			auto& input = clientInputs_[i];
			auto& paddle = paddleStates_[i];

			const float delta = 20.0f;
			if (input.up) {
				paddle.velY -= delta;
			}
			if (input.down) {
				paddle.velY += delta;
			}
			
			paddle.velY *= 0.9f;
			if (std::abs(paddle.velY) < 0.05f) {
				paddle.velY = 0.0f;
			}
			if (std::abs(paddle.velY) < 0.05f) {
				paddle.velY = 0.0f;
			}

			paddle.posY += paddle.velY * (1.0f / 60.0f);

			if (paddle.posY > SCREEN_HEIGHT) {
				paddle.posY = SCREEN_HEIGHT - 1;
				paddle.velY = 0;
			}
			else if (paddle.posY < 0) {
				paddle.posY = 1;
				paddle.velY = 0;
			}

		}
	}

	//update ball only when both clients are in the game
	if (connectedClients() >= 2) {
		bstate_.posX += bstate_.velX * (1.0f / 60.0f);
		bstate_.posY += bstate_.velY * (1.0f / 60.0f);
	}

	//Paddle collision against ball
	const auto& leftPlayer = paddleStates_[0];
	const auto& rightPlayer = paddleStates_[1];

	//left paddle collision
	if (bstate_.posX <= leftPlayer.posX + PADDLE_WIDTH &&
		(bstate_.posY >= leftPlayer.posY && bstate_.posY <= leftPlayer.posY + PADDLE_HEIGHT)) {
		bstate_.velX *= -1;
	}

	//right paddle collision
	if (bstate_.posX + BALL_SIZE >= rightPlayer.posX &&
		bstate_.posY >= rightPlayer.posY && 
		bstate_.posY + BALL_SIZE <= rightPlayer.posY + PADDLE_HEIGHT) {
		bstate_.velX *= -1;
	}

	//ball border collision
	if (bstate_.posX + BALL_SIZE > SCREEN_WIDTH) { //ball hit right border, player 1 scores
		sstate_.player = Player::One;
		sstate_.playerScored = true;
	}

	if (bstate_.posX <= 0) { //ball hit left border, player 2 scores
		sstate_.player = Player::Two;
		sstate_.playerScored = true;
	}

	//X axis bounce
	if (bstate_.posX + BALL_SIZE > SCREEN_WIDTH || bstate_.posX <= 0) {
		bstate_.velX *= -1;
	}


	//Y axis bounce
	if (bstate_.posY + BALL_SIZE > SCREEN_HEIGHT || bstate_.posY <= 0) {
		bstate_.velY *= -1;
	}
}

void Server::sendState()
{
	//Send paddle data
	sf::Packet paddlePacket;
	paddlePacket << (uint8_t)CommandToClient::PaddleState;
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		if (connects_[i]) {
			auto& clientState = paddleStates_[i];
			//send client slot id and client position
			paddlePacket << (uint8_t)i << clientState.posX << clientState.posY;
		}
	}



	//Send ball data to clients
	sf::Packet ballPacket;
	ballPacket << (uint8_t)CommandToClient::BallState << bstate_.posX << bstate_.posY;

	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		if (connects_[i]) {
			auto &clientEndPoint = clients_[i];
			sendPacket(paddlePacket, clientEndPoint);
			sendPacket(ballPacket, clientEndPoint);
		}
	}

	//Score data
	if (sstate_.playerScored) {
		sf::Packet scorePacket;
		//send command, and which player scored and number of points to reward player with
		scorePacket << (uint8_t)CommandToClient::PlayerScored << (int)sstate_.player << (uint8_t)1;
		for (int i = 0; i < MAX_CONNECTIONS; i++) {
			auto& clientEndPoint = clients_[i];	
			if (scorePacket.getDataSize() > 0) {
				sendPacket(scorePacket, clientEndPoint);
			}
		}
		sstate_.playerScored = false;
	}
}

void Server::sendPacket(sf::Packet& packet, ClientEndPoint &client)
{
	socket_.send(packet, client.address, client.port);
}

void Server::handleConnection(sf::IpAddress& address, uint16_t port)
{
	//Store new client information
	ClientEndPoint newClient;
	newClient.address = address;
	newClient.port = port;

	sf::Uint8 slot = emptySlot();
	
	std::cout << "slot: " << (int)slot << "\n";
	clients_[slot] = newClient;

	sf::Packet connectionPacket;
	connectionPacket << (uint8_t)CommandToClient::ConnectRequestResult << (uint8_t)1
		<< slot;

	//send connection request success packet to client, plus their slot id
	sendPacket(connectionPacket, clients_[slot]);
	log("New Client connected from", clients_[slot].address, clients_[slot].port);

	////setup paddle position for new client
	ObjectState pstate;
	pstate.velX = 0;
	pstate.velY = 10;

	if (slot == 0) {
		//left paddle
		pstate.posX = 25;
		pstate.posY = SCREEN_HEIGHT / 2;
	}
	else{
		pstate.posX = SCREEN_WIDTH - 25;
		pstate.posY = SCREEN_HEIGHT / 2;
	}

	paddleStates_[slot] = pstate;
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

void Server::handleBroadcast(sf::IpAddress &address, uint16_t port)
{
	log("Broadcast from client ", address, port);
	sf::Packet serverip;
	serverip << (uint8_t)CommandToClient::BroadcastRequestResult << (uint8_t)1;
	socket_.send(serverip, address, port);
}

void Server::handleClientInput(sf::Packet& packet)
{
	uint8_t input;
	uint8_t clientID;
	packet >> clientID >> input;
	
	auto& clientInput = clientInputs_[clientID];
	clientInput.up = input & Input::Up;
	clientInput.down = input & Input::Down;
}

int Server::connectedClients()
{
	int connected = 0;
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		if (connects_[i])
			connected++;
	}
	return connected;
}

int Server::emptySlot()
{
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		if (!connects_[i]) {
			return i;
		}
	}
	return -1;
}
