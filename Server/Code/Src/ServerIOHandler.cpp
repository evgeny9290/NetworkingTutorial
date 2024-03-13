#include "ServerIOHandler.h"

namespace Networking {

	int ServerSendRecieve::recieveMsg(const acceptedSocket* const a_acceptedSocket, char* a_msg, size_t a_msgLen) const{
		int bytesRead = recv(a_acceptedSocket->socketFD, a_msg, a_msgLen, 0);

		if (bytesRead == 0) {
			return READ_DONE;
		}

		if (bytesRead > 0 && bytesRead - 1 < MAXLINE) {
			a_msg[bytesRead] = 0;
		}

		std::cout << "###########" << std::endl;
		std::cout << "Recieved request from: " << a_acceptedSocket->socketFD << std::endl;
		std::cout << "Request was: " << a_msg << std::endl;
		std::cout << "###########" << std::endl;

		if (strcmp(a_msg, "exit") == 0) {
			std::cout << "@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
			std::cout << "Client Sent Termination Sygnal" << std::endl;
			std::cout << "@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
			return EXIT_CODE;
		}

		return bytesRead;
	}

	int ServerSendRecieve::sendMsg(const acceptedSocket* const a_acceptedSocket, const char* const a_msg, size_t a_msgLen) const {
		char buffer[MAXLINE];

		std::string msgToClient = "hello client number: " + std::to_string(a_acceptedSocket->socketFD);
		const char* msgToClientC = msgToClient.c_str();

		strncpy(buffer, msgToClientC, strlen(msgToClientC));
		int bytesSent = send(a_acceptedSocket->socketFD, buffer, strlen(msgToClientC), 0);
		return bytesSent;
	}

	bool ServerSendRecieve::validRecieveMsg(const char* const a_msg, size_t a_msgLen) const {
		return true;
	}

}