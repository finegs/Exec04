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

int main(int argc, char* argv[])
{
	if(init(argc, argv)) {
		std::cerr << " Illegal Arguments. Check arguments all !!!";
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int init(int argc, char* argv[])
{
	if(argc < 1) return EXIT_FAILURE;

	int workerCnt = atoi(argv[0]);

	for (int i = 0; i < workerCnt; ++i) {
		WorkerThread* worker =
				new WorkerThread((std::string(WORKER_PREFIX) + std::to_string(i)).c_str());
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


