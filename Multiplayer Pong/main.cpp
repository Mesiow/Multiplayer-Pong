#include "Game.h"
#include "net/Server.h"

int main() {

	char choice;
	std::cout << "1. Host(Server)\n";
	std::cout << "2. Client(User)\n";
	std::cin >> choice;

	if (choice == '1') {
		Server server;
		server.create(7777);
		server.run();
	}
	else if (choice == '2') {
		Game multi_pong;
		if (multi_pong.Construct(SCREEN_WIDTH, SCREEN_HEIGHT, 4, 4, false, true))
			multi_pong.Start();
	}


	return 0;
}