#pragma once

#include "Common.h"
#include <queue>
#include <mutex>
#include <thread>

namespace Networking {

	struct acceptedSocket 
	{
		int socketFD;
		struct sockaddr_in address;

	} typedef acceptedSocket;

	class threadSafeQueue {
	public:
		threadSafeQueue() = default;
		~threadSafeQueue() = default;

		void push(acceptedSocket* a_acceptedSocket);
		acceptedSocket* pop();
		bool empty();


	private:
		std::queue<acceptedSocket*> m_queue;
		std::mutex m_mutex;
		std::condition_variable m_cv;
	};
}