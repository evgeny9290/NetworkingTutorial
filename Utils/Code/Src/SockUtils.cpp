#include "SockUtils.h"

namespace Networking {
	acceptedSocket* threadSafeQueue::pop() {
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait(lock, [this] {return !m_queue.empty(); });

		if (!m_queue.empty()) {
			acceptedSocket* acceptedSocket = m_queue.front();
			m_queue.pop();
			return acceptedSocket;
		}
		return nullptr;
	}

	void threadSafeQueue::push(acceptedSocket* a_acceptedSocket) {
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_queue.push(a_acceptedSocket);
		}
		m_cv.notify_one();

	}

	bool threadSafeQueue::empty() {
		return m_queue.empty();
	}
};