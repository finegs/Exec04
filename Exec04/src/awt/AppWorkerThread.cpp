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

enum AppCLICmd {
	cHelp,
	cStatus,
	cExit,
	cPause,
	cResume
};


AppCLICmd toAppCLICmd(const std::string& str)
{
	if(str == "-help") return cHelp;
	else if(str == "-status") return cStatus;
	else if(str == "-exit") return cExit;
	else if(str == "-pause") return cPause;
	else if(str == "-resume") return cResume;
	else return cHelp;
}

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

	bool isRun =- true;
	std::string cmd;
	while(isRun) {
		std::cout << "cmd >> " << std::endl;
		std::cin >> cmd;

		AppCLICmd c = toAppCLICmd(cmd);

		switch(c) {

		case cStatus:
		{
			size_t workerCnt = workers.size();
			std::cout << "Current Workers.size=" << workerCnt;
			for (size_t i = 0; i < workerCnt; ++i) {
				std::string key = std::string(WORKER_PREFIX) + std::to_string(i);
				WorkerThread* worker =
						workers[key.c_str()];
				std::cout << "[" << i << "/" << workerCnt << "] - Workers[" << key << "]=" << worker;
				std::cout << ", queue=" << worker->getQueue().size() << std::endl;
			}
			std::cout << std::endl;
		}
			break;

		case cExit:
		{
			isRun = false;
		}
		break;

		case cPause:
		{
			std::cout << "Not yet implemented" << std::endl;
		}
		break;

		case cResume:
		{
			std::cout << "Not yet implemented" << std::endl;
		}
		break;

		case cHelp:
		default:
		{
			std::cout << "Command : -help 			(This Message)" << std::endl;
			std::cout << "          -status 		(Print Current Threads Status)" << std::endl;
			std::cout << "          -stopall 		(stop all worker threads)" << std::endl;
			std::cout << "          -pausetimer 	(pause timer)" << std::endl;
			std::cout << "          -exit 			(Exit current process)" << std::endl;
		}
		break;
		}

		std::cin.clear();
	}
//	std::cout<< "Main Thread going to wait for ";
//	this_thread::sleep_for(std::chrono::minutes(10));

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
