#include "ClientIOHandler.h"

namespace Networking {

	int ClientSendRecieve::recieveMsg(const acceptedSocket* const a_acceptedSocket, char* a_msg, size_t a_msgLen) const {
		int bytesRead = recv(a_acceptedSocket->socketFD, a_msg, a_msgLen, 0);

		if (bytesRead == 0) {
			return READ_DONE;
		}

		if (bytesRead > 0 && bytesRead - 1 < MAXLINE) {
			a_msg[bytesRead] = 0;
		}

		std::cout << "bytes Recieved: " << bytesRead << std::endl;
		std::cout << "Response was: " << a_msg << std::endl;

		return bytesRead;
	}

	int ClientSendRecieve::sendMsg(const acceptedSocket* const a_acceptedSocket, const char* const a_msg, size_t a_msgLen) const {
		int bytesSent = send(a_acceptedSocket->socketFD, a_msg, a_msgLen, 0);
		std::cout << "bytes Sent: " << bytesSent << std::endl;
		return bytesSent;
	}

	bool ClientSendRecieve::validRecieveMsg(const char* const a_msg, size_t a_msgLen) const {
		return true;
	}
}