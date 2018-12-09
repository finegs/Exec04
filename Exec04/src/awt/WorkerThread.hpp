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
	WorkerThread(const char* threadName) : m_thread(nullptr), threadName(threadName) {};

	~WorkerThread();

	bool createThread();

	void exitThread();

	std::thread::id getThreadId();
	const char* getName() const { return threadName; }

	static std::thread::id getCurrentThreadId();

	void postMsg(const UserData* data);

private:
	WorkerThread(const WorkerThread&);
	WorkerThread& operator=(const WorkerThread&);

	void process();

	void processTimer();

#if USE_USE_WIN32_TIMER
	unsigned long WorkerThread::processWin32(void* parameter);
#endif

	std::thread* m_thread;
	std::thread m_timerThread;
	std::queue<ThreadMsg*> m_queue;
	std::mutex	m_mutex;
	std::condition_variable m_cv;
	std::atomic<bool> m_timerExit;
	const char* threadName;
};

#endif /* WORKERTHREAD_HPP_ */
