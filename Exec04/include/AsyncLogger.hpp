/*
 * AsyncLogger.hpp
 *
 *  Created on: 2017. 12. 8.
 *      Author: finegs
 */

#ifndef ASYNCLOGGER_HPP_
#define ASYNCLOGGER_HPP_

#include <cstdio>
#include <ctime>
#include <cstring>

#include <string>
#include <memory>
#include <iostream>

#include <sstream>
#include <chrono>
#include <vector>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <mutex>


#include <condition_variable>

#include <sys/time.h>

#include <queue>
#include <exception>

namespace asynclogger {

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

static std::string MSG_INF("[I]");
static std::string MSG_DBG("[D]");
static std::string MSG_ERR("[E]");
static std::string MSG_FATAL("[F]");

enum AsyncLogLevel {
	L_DEBUG = 0, L_INFO, L_ERROR, L_FATAL
};

#define AsyncLogLevel_To_Str(Enum) std::string(#Enum).c_str()

template<typename ... Args>
std::string sformat(const std::string& format, Args ... args);

class AsyncLogMsg {
public:
	AsyncLogMsg(const char* msg);
	AsyncLogMsg(const AsyncLogLevel& level, const char* msg);
	~AsyncLogMsg();
	char* getMsg() const {
		return msg;
	}
	TimePoint getTimePoint() const {
		return createTimePoint;
	}

	AsyncLogLevel getLevel() const {
		return level;
	}

private:
	TimePoint createTimePoint;
	char* msg;
	AsyncLogLevel level;

};

class AsyncLogger {
public:
	AsyncLogger();
	~AsyncLogger();

	const char* getTS();
	void getTS(const TimePoint& tp, char* ts);
	AsyncLogLevel getLevel() const {
		return this->level;
	}
	void setLevel(const AsyncLogLevel& level) {
		this->level = level;
	}

	void start();
	void pause();
	void resume();
	void stop();
	void debug(const std::string& msg);
	void info(const std::string& msg);
	void error(const std::string& msg);
	void fatal(const std::string& msg);
	void debug(const char* msg);
	void info(const char* msg);
	void error(const char* msg);
	void fatal(const char* msg);
	int getQueueSize() const {
		return msgQueue.size();
	}
	void join();

private:
	//	std::priority_queue<asynclogger::AsyncLoggerMessage*> que;
	std::queue<asynclogger::AsyncLogMsg*> msgQueue;
	std::mutex mtx;
	std::condition_variable cv;
	std::chrono::milliseconds sleepInterValMills;
	bool isStart;
	bool isPause;
	std::thread* logWriter;
	char* ts;
	std::ostream out;
	std::mutex outMtx;
	AsyncLogLevel level;

	void subTask();
	void log(const asynclogger::AsyncLogLevel& level, const std::string& str);
	void syncDebugLog(const char* msg);
	void handleLog(const asynclogger::AsyncLogMsg* msg);
	void handleLog(const char* msg, const TimePoint& np);
	void handleLog(const asynclogger::AsyncLogLevel& level, const char* msg,
			const TimePoint& tp);
	void handleSyncLog(const asynclogger::AsyncLogLevel& level,
			const char* msg);
};

}

#endif /* ASYNCLOGGER_HPP_ */
