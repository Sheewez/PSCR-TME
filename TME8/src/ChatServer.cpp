#include "ChatServer.h"
#include <unistd.h>
#include <string>
#include <iostream>
#include "WriteAndRead.cpp"

namespace pr {

	class ChatRoomCH : public ConnectionHandler {
		IChatRoom * cr;
	public :
		ChatRoomCH (IChatRoom * cr):cr(cr){}

		void handleConnection(Socket s) {
			while (1) {
				int req = pr::readInt(s);
				switch (req) {
					case 0:
					{
						std::string subject = cr->getSubject();
						pr::writeString(s, subject);
						break;
					}
					case 1:
					{
						size_t sz = cr->nbParticipants();
						pr::writeInt(s, sz);
						break;
					}
					case 2:
					{
						std::cout << "fermeture" << std::endl;
						return;
					}
						default :
						std::cerr << "error message " << req << std::endl;
				}
			}
		}

		ConnectionHandler * clone() const {
			return new ChatRoomCH(* this);
		}
	};

	ChatServer::ChatServer(IChatRoom * cr, int port) :server(new ChatRoomCH(cr)) {
		server.startServer(port);
	}

	ChatServer::~ChatServer() {
		server.stopServer();
	}
}
