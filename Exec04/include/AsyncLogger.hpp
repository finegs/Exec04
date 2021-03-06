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
#include <unordered_map>

#include <condition_variable>

#include <sys/time.h>

#include <queue>
#include <exception>

#ifdef _WIN32
#ifdef BUILDING_DLL
#define ALOGGER_DLL __declspec(dllexport)
#elif USING_DLL
#define ALOGGER_DLL __declspec(dllimport)
#else
#define ALOGGER_DLL
#endif
#else
#define ALOGGER_DLL
#endif


namespace asynclogger {

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

static std::string MSG_INF("[I]");
static std::string MSG_DBG("[D]");
static std::string MSG_ERR("[E]");
static std::string MSG_FATAL("[F]");

static std::string _ASYNC_ROOT_LOGGER("Root");

enum AsyncLogLevel {
	L_DEBUG = 0, L_INFO, L_ERROR, L_FATAL
};

#define AsyncLogLevel_To_Str(Enum) std::string(#Enum).c_str()

template<typename ... Args>
std::string ALOGGER_DLL sformat(const std::string& format, Args ... args);

void ALOGGER_DLL mformat(const char* format);

template<typename T, typename... Targs>
void ALOGGER_DLL mprintf(const char* format, T value, Targs... Fargs);

class ALOGGER_DLL AsyncLogMsg {
public:
	AsyncLogMsg( const char* _msg, const AsyncLogLevel& level = AsyncLogLevel::L_INFO);
	~AsyncLogMsg();
	char* getMsg() const { return msg; }
	TimePoint getTimePoint() const { return createTimePoint; }
	AsyncLogLevel getLevel() const { return level; }

private:
	TimePoint createTimePoint;
	char* msg;
	AsyncLogLevel level;

};

class ALOGGER_DLL AsyncLogger {
public:
	static int init(int argc, char* argv[]);
	static AsyncLogger& getLogger(const std::string& loggerName);
	static bool registerLogger(const std::string& loggerName, AsyncLogger& logger);
	static AsyncLogger& unregisterLogger(const std::string& loggerName);
public:
	AsyncLogger();

	explicit
	AsyncLogger(const std::string& name)
				: name(name),
				  isStart(true),
				  isPause(true),
				  logWriter(nullptr),
				  ts(nullptr),
				  out(nullptr),
				  level(asynclogger::AsyncLogLevel::L_INFO) {}
	AsyncLogger(const AsyncLogger& logger)
				: name(logger.name),
				  isStart(logger.isStart),
				  isPause(logger.isPause),
				  logWriter(logger.logWriter),
				  ts(nullptr),
				  out(nullptr),
				  level(logger.level) {}

	AsyncLogger(AsyncLogger&& logger)
				: name(std::move(logger.name)),
				  msgQueue(std::move(logger.msgQueue)),
				  isStart(logger.isStart),
				  isPause(logger.isPause),
				  logWriter(std::move(logWriter)),
				  ts(std::move(logger.ts)),
				  out(std::move(logger.out)),
				  level(logger.level)
	{
		logger.logWriter = nullptr;
		logger.out = nullptr;
	}

	~AsyncLogger();


	const AsyncLogLevel& getLevel() const {
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

	const std::string& getName() const { return name; }
	const char* getTS();
	void getTS(const TimePoint& tp, char* ts);

private:
	//	std::priority_queue<asynclogger::AsyncLoggerMessage*> que;
	const std::string name;
	std::queue<asynclogger::AsyncLogMsg*> msgQueue;
	std::mutex mtx;
	std::condition_variable cv;
	std::chrono::milliseconds sleepInterValMills;
	bool isStart;
	bool isPause;
	std::thread* logWriter;
	char* ts;
	std::ostream* out;
	std::mutex outMtx;
	AsyncLogLevel level;

	static std::unordered_map<std::string, asynclogger::AsyncLogger> loggers;


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
