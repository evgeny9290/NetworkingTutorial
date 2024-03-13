#include "Client.h"

namespace Networking {
	
	ClientSocket::ClientSocket(const char* const a_ip, const int a_port, SendRecieveInterface* a_sendRecieveInterface)
		: m_sendRecieve(a_sendRecieveInterface),
		  m_clientSocket(new acceptedSocket)
#if defined(_WIN32) || defined(_WIN64)
		 ,
		  m_wsaData(),
		  m_windowsSocketFD()
#endif
	{
		createSocketIPv4();
		setServerAddress(a_ip, a_port);
		connectToServer();
	}

	ClientSocket::~ClientSocket() {
		close();
	}
	
	void ClientSocket::createSocketIPv4() {
	#if defined(_WIN32) || defined(_WIN64)
		m_windowsSocketFD = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
		if (m_windowsSocketFD != NO_ERROR) {
			std::cout << "Error as WSAStartup()" << std::endl;
		}
	#endif
		m_clientSocket->socketFD = socket(AF_INET, SOCK_STREAM, 0);
	}

	void ClientSocket::setServerAddress(const char* a_serverIP, int a_port) {
		memset(&m_clientSocket->address, 0, sizeof(m_clientSocket->address));
		if (!strlen(a_serverIP))
		{
			std::cerr << "Error, server IP is empty" << std::endl;
		}
		m_clientSocket->address.sin_family = AF_INET;
		m_clientSocket->address.sin_port = htons(a_port);

		inet_pton(AF_INET, a_serverIP, &m_clientSocket->address.sin_addr.s_addr);
	}

	void ClientSocket::connectToServer() {
		int connected = connect(m_clientSocket->socketFD,
								(const struct sockaddr*)&m_clientSocket->address,
								sizeof(m_clientSocket->address));
		checkError(connected, "Failed to connect to the server");
	}

	void ClientSocket::sendRequest(const char* const a_msg, size_t a_msgLen) {
		int bytesSent = m_sendRecieve->sendMsg(m_clientSocket, a_msg, a_msgLen);
		checkError(bytesSent, "Failed to send message to the server");
	}

	void ClientSocket::recieveResponse(char* a_msg, size_t a_msgLen) {
		int bytesRead = m_sendRecieve->recieveMsg(m_clientSocket, a_msg, a_msgLen);
		checkError(bytesRead, "Failed to recieve response from server");

		if (m_sendRecieve->validRecieveMsg(a_msg, a_msgLen)) {
			// do some stuff here if needed
			// for example if the recieve msg is valid it got here, but is it a known command?
			// is it anything special?
			// handle it here
		}


	}

	void ClientSocket::checkError(int a_result, const std::string& a_errorMsg) {
		if (a_result < 0) {
			std::cerr << "Error number: " << a_result << " msg: " << a_errorMsg << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	void ClientSocket::close() {
	#if defined(_WIN32) || defined(_WIN64)
			closesocket(m_clientSocket->socketFD);
			shutdown(m_clientSocket->socketFD, SD_BOTH);
			WSACleanup();
			free(m_clientSocket);
	#else
			close(m_clientSocketFD);
			shutdown(m_clientSocketFD, SD_BOTH);
	#endif
	}

	int ClientSocket::getSocketFD() {
		return m_clientSocket->socketFD;
	}


	void ClientSocket::runClient() {
		char sentMsg[MAXLINE];
		char recievedMsg[MAXLINE];

		std::cout << "type what to send" << std::endl;
		std::cout << "type exit to terminate" << std::endl << std::endl;

		while (true) {
			std::string userMsg = getLineFromUser();

			if (!checkAppropriateCommand(userMsg)) {
				continue;
			}

			strncpy(sentMsg, userMsg.c_str(), userMsg.size());
			sentMsg[userMsg.size()] = 0;

			if (sentMsg != NULL) {
				if (strcmp(sentMsg, "exit") == 0) {
					sendRequest(sentMsg, strlen(sentMsg));
					break;
				}

				sendRequest(sentMsg, strlen(sentMsg));
				recieveResponse(recievedMsg, MAXLINE);
			}

		}
	}
}	