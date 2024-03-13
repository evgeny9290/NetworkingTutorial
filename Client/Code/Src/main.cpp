#include "Client.h"
#include "SockUtils.h"
#include "clientUtils.h"


int main()
{
	Networking::ClientSendRecieve clientIOHandler;
	const char* const connectToIP = "127.0.0.1";
	const int connectToPort = 9090;

	Networking::ClientSocket client(connectToIP, connectToPort, &clientIOHandler);

	client.runClient();

	return 0;
}