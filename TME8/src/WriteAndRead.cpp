#include "Socket.h"
#include <unistd.h>


namespace pr
{
	int readInt (Socket sock) {
		int n=0;
		int fd = sock.getFD();
		if ( read(fd,&n,sizeof(n)) < sizeof(n)) {
			perror("readI");
			fd = -1;
		}
		return n;
	}

	void writeInt (Socket sock, int n) {
		int fd = sock.getFD();
		if ( write(fd,&n,sizeof(n)) < sizeof(n)) {
			perror("writeI");
			fd = -1;
		}
	}

	void writeString (Socket sock, const std::string & s) {
		size_t sz = s.length();
		writeInt(sock, sz);
		int fd = sock.getFD();
		if ( write(fd,s.data(),sz) < sz) {
			perror("writeS");
			fd = -1;
		}
	}

	std::string readString (Socket sock) {
		size_t sz = readInt(sock);
		int fd = sock.getFD();
		if ( read(fd,&sz,sizeof(sz)) < sizeof(sz)) {
			perror("readI");
			fd = -1;
		}
		return sz;
	}

}
