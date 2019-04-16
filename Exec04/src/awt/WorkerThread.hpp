/*
 * AsyncWorkerThread.hpp
 *
 *  Created on: 2018. 12. 9.
 *      Author: finegs
 */

#ifndef WORKERTHREAD_HPP_
#define WORKERTHREAD_HPP_

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>

#include "UserData.hpp"
#include "ThreadMsg.hpp"

const static std::string WORKER_PREFIX = "Worker_";

class WorkerThread
{
public:
	WorkerThread(const char* threadName)
		: m_thread(nullptr),
		  m_timerThread(nullptr),
		  threadName(threadName),
		  m_timerExit(false),
		  m_timerPause(true) {
		m_timerCycle = 250;
	};

	~WorkerThread() {
		exitThread();
	};

	bool createThread();

	void exitThread();

	std::thread::id getThreadId();
	const char* getName() const { return threadName; }

	static std::thread::id getCurrentThreadId();

	std::queue<ThreadMsg*> getQueue() const { return m_queue; }
	std::thread* getTimerThread() const { return m_timerThread; }

	void postMsg(const UserData* data);

	bool createTimerThread();
	void pauseTimer();
	void resumeTimer();

	int setTimerCycle(int cycle);
	int getTimerCycle() const;

private:
	WorkerThread(const WorkerThread&);
	WorkerThread& operator=(const WorkerThread&);

	void process();

	void processTimer();

#if USE_USE_WIN32_TIMER
	unsigned long WorkerThread::processWin32(void* parameter);
#endif

	std::thread* m_thread;
	std::thread* m_timerThread;
	const char* threadName;
	std::queue<ThreadMsg*> m_queue;
	std::mutex	m_mutex;
	std::condition_variable m_cv;
	std::atomic<bool> m_timerExit;
	std::atomic<bool> m_timerPause;
	std::atomic<int>  m_timerCycle;
};

#endif /* WORKERTHREAD_HPP_ */
