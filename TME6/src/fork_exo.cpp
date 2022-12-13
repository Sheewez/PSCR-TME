#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

//int main () {
//	const int N = 3;
//	int nbfils = 0;
//	std::cout << "main pid=" << getpid() << std::endl;
//	for (int i=1, j=N; i<=N && j==N ; i++ ) {
//		if (fork()!=0 ){
//			nbfils++;
//			break;
//		}else
//			nbfils = 0;
//		std::cout << " i:j " << i << ":" << j << std::endl;;
//		for (int k=1; k<=i && j==N ; k++) {
//			if ( fork() == 0) {
//				j=0;
//				std::cout << " k:j " << k << ":" << j << std::endl;
//			}
//
//		}
//	}
//
//	for(int i = 1; i <= N; i++){
//		wait(nullptr);
//	}
//
//	return 0;
//
//}
//
