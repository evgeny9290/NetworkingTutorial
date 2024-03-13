#pragma once
#include "Common.h"
#include "SockUtils.h"


namespace Networking {
	class SendRecieveInterface {
	public:
		SendRecieveInterface() = default;
		virtual ~SendRecieveInterface() = default;

		virtual int recieveMsg(const acceptedSocket* const a_acceptedSocket, char* a_msg, size_t a_msgLen) const = 0;
		virtual int sendMsg(const acceptedSocket* const a_acceptedSocket, const char* const a_msg, size_t a_msgLen) const = 0;
		virtual bool validRecieveMsg(const char* const a_msg, size_t a_msgLen) const = 0;
	};
}