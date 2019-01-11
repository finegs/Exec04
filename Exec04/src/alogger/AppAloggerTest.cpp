/*
 * AppAloggerTest.cpp
 *
 *  Created on: 2019. 1. 5.
 *      Author: fineg
 */

#define LOGGER 1


#if LOGGER

//#define _USE_32BIT_TIME_T


#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <vector>
#include <sys/time.h>
#include <time.h>
#include <thread>
#include <mutex>
//#include <bits/std_mutex.h>
#include <unistd.h>
#include <chrono>
#include <condition_variable>
#include <algorithm>

#include <AsyncLogger.hpp>

using namespace std;
using namespace chrono;
//using namespace chrono_literals;

using namespace asynclogger;

static AsyncLogger logger;

char TS[25];

mutex mainCmdMTX;
condition_variable mainCmdCV;
bool isRun = true;
bool isPaused = false;
//auto  sleepInterval = 3000ms;
milliseconds sleepInterval(10000);

void log(const std::string& msg) {
	logger.info(msg);
}

using namespace std;
vector<int> getPi(string p) {
	int m = (int)p.size(),j=0;
	vector<int> pi(m, 0);
	for(int i= 1;i<m;i++) {
		while(j>0&&p[i] != p[j])
			j = pi[j-1];
		if(p[i] == p[j])
			pi[i] = ++j;
	}
	return pi;
}

vector<int> kmp(string s, string p) {
	vector<int> ans;
	auto pi = getPi(p);
	int n = (int)s.size(), m = (int)p.size(), j = 0;
	for(int i = 0;i<n;i++) {
		while(j>0&&s[i] !=p[j])
			j = pi[j-1];
		if(s[i]==p[j]) {
			if(j==m-1) {
				ans.push_back(i-m+1);
				j = pi[j];
			}
			else {
				j++;
			}
		}
	}

	return ans;
}

void subTask(string s, string p)
{
	char msg[100];

	log("kmp worker is started");

	for (int i = 0; i < 10000 && isRun; ++i) {
		unique_lock<mutex> lck(mainCmdMTX);

		memset(msg, '\0', strlen(msg));
		if(!isPaused) {
			log(sformat("i=%5d : start", i));

			auto matched = kmp(s, p);
			log(sformat("i=%5d : %010d", i, (int)matched.size()));
			for(auto ii: matched) {
				sprintf(msg, "i=%5d : %d", i, ii);
				log(msg);
			}

			log(sformat("i=%5d : end", i));
		}
		else {
			log(sformat("i=%5d : paused", i));
		}

		mainCmdCV.wait_for(lck, sleepInterval);

		lck.unlock();
	}

	log("kmp worker is stop.");
}

void handleUserCmd() {

	string s, p, cmd, buf;
	char lm[200];
	vector<string> cmds;

	log("Command Reader is created and started");

	while(isRun) {
		cmds.clear();

		getline(cin, cmd);
		stringstream ss(cmd);

		while(ss>>buf) cmds.push_back(buf);

		if(!cmd.empty() && (cmds.size()<1|| (cmd = cmds.at(0)).size() < 1)) {
			log("Empty Command : ");
			continue;
		}

		std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

		if("stop" == cmd) {
			log("Notifying stop..");
			isRun = false;
			logger.stop();
		}
		else if ("start" == cmd){
			log("Notifying start..");
			isPaused = false;
			isRun = true;
			logger.start();
		}
		else if("pause" == cmd) {
			log("Notifying pause");
			isRun = true;
			isPaused = true;
			logger.pause();
		}
		else if("resume" == cmd) {
			log("Notifying resume");
			isPaused = false;
			logger.resume();
		}
		else if ("get" == cmd) {
			if(cmds.size() > 1) {
				if(stricmp("qs",  cmds.at(1).c_str())==0) {
					log(sformat("Queue Size= %d", logger.getQueueSize()));
				}
				else if(stricmp("si",  cmds.at(1).c_str())==0) {
					memset(lm, '\0', strlen(lm));
					log(sformat("Sleep Interval(si)= %lld", sleepInterval.count()));
				}
				else if(stricmp("ll",  cmds.at(1).c_str())==0) {
					memset(lm, '\0', strlen(lm));
					log(sformat("Log Level(ll)= %s(%d)", AsyncLogLevel_To_Str(logger.getLevel()), logger.getLevel()));
				}
				else {
					memset(lm, '\0', strlen(lm));
					log(sformat("unsupported parameter: %s", cmds.at(1).c_str()));
				}
			}
			else {
				memset(lm, '\0', strlen(lm));
				log(sformat("unsupported argument count : %lld", cmds.size()));
			}
		}
		else if ("set" == cmd) {
			if(cmds.size() > 2) {
				if(stricmp("si",  cmds.at(1).c_str())==0) {
					int newSi = atoi(cmds.at(2).c_str());

					milliseconds newSleepInt(newSi);

					if(sleepInterval != newSleepInt) {
						memset(lm, '\0', strlen(lm));
						log(sformat("sleepInterval changed : %lld -> %lld", sleepInterval.count(), newSleepInt.count()));

						sleepInterval = newSleepInt;
					}
				}
				else if(stricmp("ll",  cmds.at(1).c_str())==0) {
					int newVal = atoi(cmds.at(2).c_str());
					AsyncLogLevel newLogLevel;
					try {
						newLogLevel = static_cast<AsyncLogLevel>(newVal);
					}
					catch (...) {
						memset(lm, '\0', strlen(lm));
						log(sformat("Unsupported LogLevel : %d", newVal));
						continue;
					}

					if(logger.getLevel() != newLogLevel) {
						memset(lm, '\0', strlen(lm));
						log(sformat("LogLevel changed : %d -> %d", logger.getLevel(), newLogLevel));

						logger.setLevel(AsyncLogLevel(newVal));
					}
				}
				else {
					memset(lm, '\0', strlen(lm));
					log(sformat("unsupported parameter: %s", cmds.at(1).c_str()));
				}
			}
			else {
				memset(lm, '\0', strlen(lm));
				log(sformat("unsupported argument count : %lld", cmds.size()));
			}
		}
		else {
			log("Notifying nothing.. : ");
		}

		mainCmdCV.notify_all();
	}
}

int main() {

	using namespace asynclogger;
	using namespace this_thread;

	string s, p, cmd, buf;
	vector<string> cmds;

	// by SGK 2017. 12. 17. start Logger Writer
	logger.start();

	cout << "Enter string : "; cout.flush();
	// by SGK 2017. 12. 17. read string
	getline(cin, s);

	cout << "Enter pattern : "; cout.flush();

	// by SGK 2017. 12. 17. read pattern
	getline(cin, p);

	thread kmpWorker(subTask, s, p);

	thread usrCmdWorker(handleUserCmd);

	kmpWorker.join();
	usrCmdWorker.join();
	logger.join();

	return 0;
}

#endif




