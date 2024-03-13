#pragma once
#include "IOHandler.h"

namespace Networking {

	class ServerSendRecieve : public SendRecieveInterface {
	public:
		ServerSendRecieve() = default;
		virtual ~ServerSendRecieve() = default;

		virtual int recieveMsg(const acceptedSocket* const a_acceptedSocket, char* a_msg, size_t a_msgLen) const override;
		virtual int sendMsg(const acceptedSocket* const a_acceptedSocket, const char* const a_msg, size_t a_msgLen) const override;
		virtual bool validRecieveMsg(const char* const a_msg, size_t a_msgLen) const override;
	};


}