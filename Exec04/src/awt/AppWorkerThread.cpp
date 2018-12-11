/*
 * AppWorkerThread.cpp
 *
 *  Created on: 2018. 12. 9.
 *      Author: finegs
 */

#include <iostream>
#include <cstdlib>
#include <unordered_map>
#include <string>
#include <assert.h>

#include "WorkerThread.hpp"

using namespace std;

int init(int argc, char* argv[]);
const WorkerThread* getWorker(const std::string& workerName);

std::unordered_map<std::string, WorkerThread*> workers;
std::unordered_map<std::string, std::string> properties;

int main(int argc, char* argv[])
{
	int workerCnt;
	if(init(argc, argv)) {
		std::cerr << " Illegal Arguments. Check arguments all !!!";
		return EXIT_FAILURE;
	}

	std::cout<< "Main Thread going to wait for ";
	this_thread::sleep_for(std::chrono::minutes(10));

	std::unordered_map<std::string, std::string>::const_iterator it =
			properties.find("workerCnt");
	if(it == properties.end()) {
		std::cerr << " No workerCnt. Check arguments all !!!, ";
		return EXIT_FAILURE;
	}
	else {
		workerCnt = std::stoi(it->second);
	}

	for (int i = 0; i < workerCnt; ++i) {
		std::string key = std::string(WORKER_PREFIX) + std::to_string(i);
		WorkerThread* worker =
				workers[key.c_str()];
		std::cout << "Worker=" << key << ", exiting." << std::endl;
		worker->exitThread();
		std::cout << "Worker=" << key << ", exit complete." << std::endl;
		workers.erase(worker->getName());
		std::cout << "Worker=" << key << ", erase." << std::endl;
		delete worker;
	}


	return EXIT_SUCCESS;
}

int init(int argc, char* argv[])
{
	if(argc < 1) return EXIT_FAILURE;

	int workerCnt = atoi(argv[1]);

	properties.insert(std::pair<std::string, std::string>("workerCnt", std::to_string(workerCnt)));

	for (int i = 0; i < workerCnt; ++i) {
		WorkerThread* worker =
				new WorkerThread((std::string(WORKER_PREFIX) + std::to_string(i)).c_str());
		worker->createThread();
		workers.insert(std::pair<std::string, WorkerThread*>(worker->getName(),
				worker));
	}

	return EXIT_SUCCESS;
}

const WorkerThread* getWorker(const std::string& workerName) {

	assert(workers.size() > 0);

	WorkerThread* worker = nullptr;

	if(workerName.empty())
		worker = workers.at(std::string(WORKER_PREFIX) + std::to_string(0));

	else worker = workers.at(workerName);

	if(nullptr == worker) worker = workers.at(std::string(WORKER_PREFIX) + std::to_string(0));

	return worker;
}
