#include "Server.h"
#include "SockUtils.h"
#include "ServerIOHandler.h"

int main()
{	
	int portNumber = 9090;
	int listenAmount = 3;
	Networking::ServerSendRecieve serverIOHandler;

	Networking::ServerSocket server(portNumber, listenAmount, &serverIOHandler);

	server.startListening();

	server.startAcceptionRequests();

	return 0;
}