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

#include <AsyncLogger.hpp>
#include <AppCLI.hpp>

#include "WorkerThread.hpp"

using namespace std;


int init(int argc, char* argv[]);
const WorkerThread* getWorker(const std::string& workerName);

std::unordered_map<std::string, WorkerThread*> workers;
std::unordered_map<std::string, std::string> properties;

int main(int argc, char* argv[])
{
	int workerCnt;

	// by SGK 2018. 12. 16. add AsyncLogger
	if(!asynclogger::AsyncLogger::init(argc, argv)) {
		std::cerr<< "illegal argument. Check all arguments" << std::endl;
		return EXIT_FAILURE;
	}

	if(init(argc, argv))
	{
		std::cerr << " Illegal Arguments. Check arguments all !!!";
		return EXIT_FAILURE;
	}

	bool isRun =- true;
	std::string cmd;
	while(isRun) {
		std::cin.clear();
		std::cout << ">> cmd : "; std::cout.flush();
		std::cin >> cmd;

		AppCLICmd::AppCLICmdType c = AppCLICmd::toAppCLICmd(cmd);
		switch(c) {

		case AppCLICmd::AppCLICmdType::cStatus:
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

		case AppCLICmd::AppCLICmdType::cExit:
		{
			isRun = false;
		}
		break;

		case AppCLICmd::AppCLICmdType::cPause:
		{
			std::cout << "Not yet implemented" << std::endl;
		}
		break;

		case AppCLICmd::AppCLICmdType::cResume:
		{
			std::cout << "Not yet implemented" << std::endl;
		}
		break;

		case AppCLICmd::AppCLICmdType::cPauseTimer:
		{
			size_t workerCnt = workers.size();
			for (size_t i = 0; i < workerCnt; ++i) {
				std::string key = std::string(WORKER_PREFIX) + std::to_string(i);
				WorkerThread* worker = workers[key.c_str()];
				std::cout << "Worker=" << key << ", pauseTimer." << std::endl;
				worker->pauseTimer();
			}
		}
		break;

		case AppCLICmd::AppCLICmdType::cResumeTimer:
		{
			size_t workerCnt = workers.size();
			for (size_t i = 0; i < workerCnt; ++i) {
				std::string key = std::string(WORKER_PREFIX) + std::to_string(i);
				WorkerThread* worker = workers[key.c_str()];
				std::cout << "Worker=" << key << ",  resumeTimer." << std::endl;
				worker->resumeTimer();
			}
		}
		break;

		case AppCLICmd::AppCLICmdType::cResetTimeCycle:
		{
			int newTimerCycle;
			std::cin >> newTimerCycle;
			if(newTimerCycle < 0)
			{
				std::cout << "Illegal New TimerCycle. [" << newTimerCycle << "]" << std::endl;
				break;
			}

			size_t workerCnt = workers.size();
			for (size_t i = 0; i < workerCnt; ++i) {
				std::string key = std::string(WORKER_PREFIX) + std::to_string(i);
				WorkerThread* worker = workers[key.c_str()];
				std::cout << "Worker=" << key << ",  resetTimerCycle. TimeCycle : [" << worker->getTimerCycle() << "] -> [" << newTimerCycle << "]" << std::endl;
				worker->setTimerCycle(newTimerCycle);
			}
		}
		break;

		case AppCLICmd::AppCLICmdType::cStringTest:
		{
			std::string s;
			std::cout << "Enter string to test : "; std::cout.flush();
			std::cin >> s;

			std::string s2(std::move(s));
			if(s == s2) std::cout << "s and s2 is same" << std::endl;
			else std::cout << "s and s2 is different" << std::endl;

			if(s.c_str() == s2.c_str()) std::cout << "s.c_str() and s2.c_str() is same" << std::endl;
			else std::cout << "s.c_str() and s2.c_str() is different" << std::endl;

			break;
		}

		case AppCLICmd::AppCLICmdType::cHelp:
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
