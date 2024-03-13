#pragma once

#include "Common.h"
#include "clientUtils.h"
#include "ClientIOHandler.h"

#include <map>
#include <sstream>

namespace Networking {

	class ClientSocket
	{
	public:
		ClientSocket(const char* const a_ip, const int a_port, SendRecieveInterface* a_sendRecieveInterface);
		~ClientSocket();

		void setServerAddress(const char* a_serverIP, int a_port);
		void connectToServer();
		void sendRequest(const char* const a_msg, size_t a_msgLen);
		void recieveResponse(char* a_msg, size_t a_msgLen);
		int getSocketFD();
		void runClient();

	private:
		void createSocketIPv4();
		void checkError(int a_result, const std::string& a_errorMsg);
		void close();

	private:
		acceptedSocket* m_clientSocket;
		SendRecieveInterface* m_sendRecieve;
		#if defined(_WIN32) || defined(_WIN64)
				WSADATA m_wsaData;
				int m_windowsSocketFD;
		#endif
	};

}