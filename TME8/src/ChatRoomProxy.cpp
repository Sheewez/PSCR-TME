#include "ChatRoomProxy.h"
#include "WriteAndRead.cpp"

namespace pr {

	ChatRoomProxy::ChatRoomProxy(const std::string & host, int port) {
		sock.connect(host, port);
	}

	std::string ChatRoomProxy::getSubject() const {
		pr::writeInt(sock, 0);
		auto s = pr::readString(sock);
		return s;
	}

	size_t ChatRoomProxy::nbParticipants() const {
		pr::writeInt(sock, 1);
		auto s = readInt(sock);
		return s;
	}

	ChatRoomProxy::~ChatRoomProxy() {
		writeInt(sock, 2);
		sock.close();
	}

	std::vector<ChatMessage> ChatRoomProxy::getHistory() const { return {} ;}
	bool ChatRoomProxy::posterMessage(const ChatMessage & msg) {return true;}
	bool ChatRoomProxy::joinChatRoom (IChatter * chatter) { return true; }
	bool ChatRoomProxy::leaveChatRoom (IChatter * chatter){ return true; }
}
