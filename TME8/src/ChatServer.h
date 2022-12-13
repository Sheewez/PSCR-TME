#ifndef SRC_CHATSERVER_H_

#define SRC_CHATSERVER_H_

#include "IChatRoom.h"
#include "TCPServer.h"

namespace pr {

	class ChatServer {
	public:
		TCPServer server;
		ChatServer(IChatRoom * cr, int port);
		~ChatServer();
	};

}

#endif /* SRC_CHATSERVER_H_ */
