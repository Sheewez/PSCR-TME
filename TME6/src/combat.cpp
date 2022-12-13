
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

#include "combat.h"
#include "rsleep.h"

int pv = 3;
//retourne de kill si == 0 alors adversaire est mort
using namespace std;
void attaquer(pid_t adversaire){
	signal(SIGINT, [](int){
		pv--;
		if (pv <= 0 )
			exit(1);

	});
	if (kill(SIGINT, adversaire) <= 0 ){
		exit(0);
	}
	randsleep();
}

void defense(){
	signal(SIGINT, SIG_IGN);
	randsleep();
}

void combat(pid_t adversaire){
	while(true){
		attaquer(adversaire);
		defense();
	}
}


int main(){
	srand(time(NULL));

	pid_t fils,pere;
	pere = getpid();
	fils = fork();

	if (fils == 0){
		combat(pere);
	}else{
		combat(fils);
	}

	return 0;
}
