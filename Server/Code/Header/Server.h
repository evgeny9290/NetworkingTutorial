#pragma once

#include "ServerIOHandler.h"

#include <limits.h>
#include <thread>
#include <mutex>

namespace Networking {

	class ServerSocket
	{
	public:
		ServerSocket(int a_listenOnPort, int a_listenAmount, SendRecieveInterface* a_sendRecieveInterface);
		~ServerSocket();

		void startListening();
		int getServerSocketFD();
		struct sockaddr_in getServerSocketAddress();
		void startAcceptionRequests();

	private:
		void recieveAndSendOnThread();
		void recieveAndSend(acceptedSocket* a_acceptedSocket);

		acceptedSocket* acceptConnection();

		void close();
		void closeSocketAndFree(acceptedSocket* a_acceptedSocket);

		void initThreadPool();
		void setUpServerSocket(const char* const a_serverIP, int a_port);
		void createSocketIPv4();
		void checkError(int a_result, const std::string& a_errorMsg);

	private:
		int m_listenAmount;
		int m_serverSocketFD;
		int m_listenOnPort;
		struct sockaddr_in m_serverSocketAddress;
		int m_poisonedApples;
		int m_numConnectedClients;
		std::vector<std::thread> m_threadPool;
		threadSafeQueue m_clientSocketsQueue;
		std::mutex m_mutex;
		bool m_stopServer;
		std::condition_variable m_cv;
		SendRecieveInterface* m_sendRecieve;

		#if defined(_WIN32) || defined(_WIN64)
				WSADATA m_wsaData;
				int m_windowsSocketFD;
		#endif
	};
}