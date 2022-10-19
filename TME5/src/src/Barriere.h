#pragma once

#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Job.h"

namespace pr {

class Barrier {
	int counter;
	const int N;
	std::mutex m;
	std::condition_variable cv;
public:
	Barrier(int max): N(max), counter(0){}
	void done(){
		std::unique_lock<std::mutex> lg(m);
		counter++;
	}

	void waitFor(){
		std::unique_lock<std::mutex> lg(m);
		while(counter != N){
			cv.wait(lg);
		}
	}
};

class SleepJobBarrier: public Job{
	int calcul (int v) {
		std::cout << "Computing for arg =" << v << std::endl;
		// traiter un gros calcul
		std::this_thread::sleep_for(std::chrono::seconds(1));
		int ret = v % 255;
		std::cout << "Obtained for arg =" << arg <<  " result " << ret << std::endl;
		return ret;
	}
	int arg;
	int * ret;
	Barrier * barrier;
public:
	SleepJobBarrier(int arg, int* ret, Barrier barrier): arg(arg), ret(ret), barrier(barrier){}

	void run(){
		*ret = calcul(arg);
		barrier->done();
	}

};
}
