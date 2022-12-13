#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int wait_till_pid(pid_t pid){
	while(true){
		pid_t res = wait(nullptr);
		if(res == -1 || res == pid)
			return res;
	}
	return -1;
}


pid_t wait_till_pid(pid_t pid,int sec) {
	static bool timeout ;
	timeout = false;
	signal(SIGALRM, [](int){
		std::cout << "received SIGALRM" << std::endl; timeout =true;
	});
	signal(SIGCHLD, [](int){
		std::cout << "received SIGCHLD" << std::endl;
	});

	alarm(sec);
	sigset_t set;
	sigfillset(&set);
	sigdelset(&set, SIGALRM);
	sigdelset(&set, SIGCHLD);
	while(true) {
		sigsuspend(&set);
		if(timeout) {
			return-1;
		}else{
			pid_t p = wait(nullptr);
			if(p == pid) {
				alarm(0);
			}
			if(p == -1 || p == pid) {
				return p;
			}
		}
	}
}
int main() {
	pid_t pid = fork();
	if(pid==0) {
		sleep(5);
	}else{
		signal(SIGINT, [](int){});
		pid_t res = wait_till_pid(pid, 3);

	}
}
