#pragma once

#include "Queue.h"
#include "Job.h"
#include <vector>
#include <thread>

namespace pr {

class Pool {
	Queue<Job> queue;
	std::vector<std::thread> threads;
public:
	Pool(int qsize) ;
	void start (int nbthread){
		threads.reserve(nbthread);
		queue.setBlocking(true);
		for( auto & t : threads){
			threads.emplace_back(poolWorker, queue);
		}
	}
	void submit (Job * job){
			//tout les point de la scene faire pool

	}

	void stop(){
		queue.setBlocking(false);
		for( auto & t : threads){
			t.join();
		}
		threads.clear();
	}

	void poolWorker(Queue<Job> &queue){
		while(true){
			Job * job = queue.pop();
			if (job == nullptr)
				return;
			job->run();
			delete job;
		}
	}

	~Pool() ;
};

}
