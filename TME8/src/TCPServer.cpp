#include "TCPServer.h"
#include <sys/select.h>
#include <iostream>
#include <unistd.h>
#include <algorithm>



namespace pr
{
	bool TCPServer::startServer(int port) {
			if (waitT == nullptr) {
				ss = new ServerSocket(port);
				if (ss->isOpen()) {
					waitT = new std::thread([](TCPServer * serv){
						while (1) {
							std::cout << "En attente sur accept" << std::endl;
							auto sc = serv->ss->accept();

							std::cout << "Après accept" << std::endl;
							if (sc.isOpen()) {
								auto copy = serv->handler->clone();
								copy->handleConnection(sc);
								delete copy;
							}
						}
					},this);
					return true;
				}
			}
			return false;
		}

		void TCPServer::stopServer() {

			if (waitT != nullptr) {
				int n = 1;
				write(killpipe, &n, sizeof(int));
				waitT->join();
				delete waitT;
				delete ss;
				close(killpipe);
				killpipe = -1;
				waitT = nullptr;
			}
		}
}
