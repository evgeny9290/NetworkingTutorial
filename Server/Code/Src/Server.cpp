#include "Server.h"
#include <chrono>
#include <sstream>


namespace Networking {

	ServerSocket::ServerSocket(int a_listenOnPort, int a_listenAmount, SendRecieveInterface* a_sendRecieveInterface)
		: m_listenAmount(a_listenAmount),
		  m_listenOnPort(a_listenOnPort),
		  m_serverSocketFD(-1),
		  m_serverSocketAddress(),
		  m_poisonedApples(0),
		  m_numConnectedClients(0),
		  m_threadPool(),
		  m_cv(),
		  m_clientSocketsQueue(),
		  m_stopServer(false),
		  m_mutex(),
		  m_sendRecieve(a_sendRecieveInterface)
#if defined(_WIN32) || defined(_WIN64)
		,
		m_wsaData(),
		m_windowsSocketFD()
#endif
	{
		setUpServerSocket("", a_listenOnPort);
		initThreadPool();
	}

	ServerSocket::~ServerSocket() {
		close();
	}

	void ServerSocket::initThreadPool() {
		for (int i = 0; i < m_listenAmount; i++) {
			m_threadPool.emplace_back(&ServerSocket::recieveAndSendOnThread, this);
		}
	}

	void ServerSocket::createSocketIPv4() {
	#if defined(_WIN32) || defined(_WIN64)
			m_windowsSocketFD = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
			if (m_windowsSocketFD != NO_ERROR) {
				std::cout << "Error as WSAStartup()" << std::endl;
			}
	#endif
			m_serverSocketFD = socket(AF_INET, SOCK_STREAM, 0);
			checkError(m_serverSocketFD, "Failet to create socket");
	}

	void ServerSocket::setUpServerSocket(const char* const a_serverIP, int a_port) {
		createSocketIPv4();
		memset(&m_serverSocketAddress, 0, sizeof(m_serverSocketAddress));
		m_serverSocketAddress.sin_family = AF_INET;
		m_serverSocketAddress.sin_port = htons(a_port);

		if (!strlen(a_serverIP))
		{
			m_serverSocketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
		}
		else
		{
			inet_pton(AF_INET, a_serverIP, &m_serverSocketAddress.sin_addr.s_addr);
		}

		checkError(bind(m_serverSocketFD, (const struct sockaddr*)&m_serverSocketAddress, sizeof(m_serverSocketAddress)),
			"Error binding socket to port");
	}

	acceptedSocket* ServerSocket::acceptConnection() {
		struct sockaddr_in clientAddress;
		socklen_t clientAddressLen = sizeof(clientAddress);
		int clientSocketFD = accept(m_serverSocketFD, (struct sockaddr*)&clientAddress, &clientAddressLen);
		checkError(clientSocketFD, "Error accepting connections");

		acceptedSocket* acceptedSock = (acceptedSocket*)malloc(sizeof(acceptedSocket));
		memset(acceptedSock, 0, sizeof(acceptedSocket));
		acceptedSock->socketFD = clientSocketFD;
		acceptedSock->address = clientAddress;
		m_numConnectedClients++;

		return acceptedSock;
	}

	void ServerSocket::startListening() {
		checkError(listen(m_serverSocketFD, m_listenAmount), "Failed to listen to sockets");
	}

	void ServerSocket::startAcceptionRequests() {
		while (!m_stopServer)
		{
			// SERVER WILL NOT SHUT DOWN BECAUSE IT ACCEPTS CONNECTIONS ON MAIN THREAD
			// THIS ACCEPT FUNCTION IS A BLOCKING FUNCTION.
			// SO ANOTHER "ACCEPTENCE" IS PENDING RIGHT AFTER ONE IS ALREADY MADE
			// SO MAINTHREAD IS STUCK HERE!!!
			// EVEN AFTER ALL CLIENTS WENT OFF
			acceptedSocket* clientSocket = acceptConnection();
			m_clientSocketsQueue.push(clientSocket);

			std::cout << "@@@@@@@@@@@" << std::endl;
			std::cout << m_numConnectedClients << std::endl;
			std::cout << m_poisonedApples << std::endl;
			std::cout << "@@@@@@@@@@@" << std::endl;

		}
		std::cout << "main thread after while" << std::endl;
	}

	void ServerSocket::recieveAndSendOnThread() {
		while (!m_stopServer || (m_stopServer && !m_clientSocketsQueue.empty())) {
			acceptedSocket* acceptedSocket = m_clientSocketsQueue.pop();
			if (acceptedSocket != NULL) {
				recieveAndSend(acceptedSocket);
			}
			// IF YOU WANT TO SHUT DOWN THE THREAD POOL UNCOMMECT THE FOLLOWING CODE
			//std::unique_lock<std::mutex> lock(m_mutex);
			//m_cv.wait(lock, [this] {return m_stopServer; });
		}
		std::cout << "***** THREAD POOL EXIT ******" << std::endl;
	}

	void ServerSocket::recieveAndSend(acceptedSocket* a_acceptedSocket) {
		char buffer[MAXLINE];
		int bytesRecieved = 0;
		bool run = true;

		while (run) {
			bytesRecieved = m_sendRecieve->recieveMsg(a_acceptedSocket, buffer, sizeof(buffer));
			checkError(bytesRecieved, "Failed to recieve request from client");

			if (m_sendRecieve->validRecieveMsg(buffer, sizeof(buffer))) {
				// do some stuff here if needed
				// for example if the recieve msg is valid it got here, but is it a known command?
				// is it anything special?
				// handle it here

				// This is just an example when the task execution is implemented as a "task factory"
				//// execute task recieved from client.
				//std::lock_guard<std::mutex> lock(m_mutex);
				//m_taskFactory->taskExecute("SomeKey");
				std::cout << "task executed" << std::endl;
			}

			if (bytesRecieved == EXIT_CODE) {
				closeSocketAndFree(a_acceptedSocket);
				run = false;
			}

			// IF YOU WANT TO SHUT DOWN THE THREAD POOL UNCOMMECT THE FOLLOWING CODE
			//{
			//	std::lock_guard<std::mutex> lock(m_mutex);
			//	if (m_numConnectedClients == m_poisonedApples) {
			//		std::cout << "SERVER SHOULD SHUTDOWN GOD DAMNIT" << std::endl;
			//		m_stopServer = true;
			//		m_cv.notify_all();
			//	}
			//}

			if (run) {
				std::lock_guard<std::mutex> lock(m_mutex);
				int bytesSent = m_sendRecieve->sendMsg(a_acceptedSocket, buffer, sizeof(buffer));
				checkError(bytesSent, "Failed to send message to the client");
			}

		}

		std::cout << "%%%%%%%%Finished Send and Recieve%%%%%%%" << std::endl;
		return;
	}

	void ServerSocket::checkError(int a_result, const std::string& a_errorMsg) {
		if (a_result < 0) {
			std::cerr << "Error number: " << a_result << " msg: " << a_errorMsg << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	void ServerSocket::closeSocketAndFree(acceptedSocket* a_acceptedSocket) {
#if defined(_WIN32) || defined(_WIN64)
		closesocket(a_acceptedSocket->socketFD);
#else
		close(a_acceptedSocket->socketFD);
#endif
		free(a_acceptedSocket);
	}

	void ServerSocket::close() {
#if defined(_WIN32) || defined(_WIN64)
		for (auto& t : m_threadPool) {
			if (t.joinable()) {
				t.join();
			}
		}

		free(m_sendRecieve);

		//for (auto& c : m_clientSocketsQueue) {
		//	free(c);
		//}

		closesocket(m_serverSocketFD);
		WSACleanup();
		shutdown(m_serverSocketFD, SD_BOTH);
#else
		close(m_serverSocketFD);
		shutdown(m_serverSocketFD, SD_BOTH);

#endif
	}

	int ServerSocket::getServerSocketFD() {
		return m_serverSocketFD;
	}

	struct sockaddr_in ServerSocket::getServerSocketAddress() {
		return m_serverSocketAddress;
	}
}