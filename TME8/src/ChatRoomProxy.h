#ifndef SRC_CHATROOMPROXY_H_
#define SRC_CHATROOMPROXY_H_
#include "IChatRoom.h"
#include "Socket.h"

namespace pr {
	class ChatRoomProxy : public IChatRoom {
		mutable Socket sock;
	public:
		ChatRoomProxy(const std::string & host, int port);
		std::string getSubject() const;
		std::vector<ChatMessage> getHistory() const ;
		bool posterMessage(const ChatMessage & msg) ;
		bool joinChatRoom (IChatter * chatter) ;
		bool leaveChatRoom (IChatter * chatter) ;
		size_t nbParticipants() const;
		~ChatRoomProxy();
	};
}

#endif
