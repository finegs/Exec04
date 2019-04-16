/*
 * WorkerThread.cpp
 *
 *  Created on: 2018. 12. 9.
 *      Author: finegs
 */
#include <cassert>
#include <iostream>
#include <string>
#include <thread>
#include <sstream>
#include "WorkerThread.hpp"

using std::thread;

#if USE_WIN32_TIMER
#include <windows.h>
#include <mmsystem.h>
#endif

bool WorkerThread::createThread()
{
	if(m_thread) return true;
	m_thread = new std::thread(&WorkerThread::process, this);
	return true;
}

bool WorkerThread::createTimerThread()
{
	if(m_timerThread) return true;
	m_timerExit = false;
	m_timerThread = new std::thread(&WorkerThread::processTimer, this);
	return true;
}

void WorkerThread::process()
{
	createTimerThread();
	while(1)
	{
		ThreadMsg* msg = 0;
		{
			std::unique_lock<std::mutex> lk(m_mutex);
			while(m_queue.empty())
				m_cv.wait(lk);
			if(m_queue.empty())
				continue;

			msg = m_queue.front();
			m_queue.pop();
		}

		switch(msg->msgType)
		{
		case MsgPostUserData:
		{
			assert(msg->msg != NULL);

			const UserData* userData = static_cast<const UserData*>(msg->msg);
			std::cout << userData->getTimeStamp() << " " << userData->msg << " on " << threadName << std::endl;

			delete msg;
			msg = nullptr;
			break;
		}

		case MsgTimer:
		{
			std::cout << msg->getTimeStamp() << " Timer expired on " << threadName << ", User Message=" << msg->getMsg()->msg << std::endl;

			delete msg;
			msg = nullptr;
			break;
		}

		case MsgExitThread:
		{
			std::cout << msg->getTimeStamp() << threadName << " is about to exit. TimerThreadId = " << m_timerThread->get_id() << std::endl;

			m_timerExit = true;
			if(m_timerThread) {
				m_timerThread->join();
				m_timerThread = nullptr;
			}

			delete msg;
			std::unique_lock<std::mutex> lk(m_mutex);
			int cnt = 0;
			while(!m_queue.empty())
			{
				msg = m_queue.front();
				m_queue.pop();
				delete msg;
				cnt++;
			}

			std::cout << msg->getTimeStamp() << threadName << " is popping and delete all queued msg [" << cnt << "]" << std::endl;
			break;
		}

		default:
			assert(false);
		}
	}
}

void WorkerThread::postMsg(const UserData* data)
{
	assert(m_thread);

	ThreadMsg* msg = new ThreadMsg(MsgPostUserData, data);
	std::unique_lock<std::mutex> lk(m_mutex);
	m_queue.push(msg);
	m_cv.notify_all();
}

void WorkerThread::pauseTimer()
{
	assert(m_timerThread);
	m_timerPause = true;
}

void WorkerThread::resumeTimer()
{
	assert(m_timerThread);
	m_timerPause = false;
}

void WorkerThread::exitThread()
{
	if(!m_thread) return;

	ThreadMsg* msg = new ThreadMsg(MsgExitThread, nullptr);
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_queue.push(msg);
		m_cv.notify_one();
	}

	m_thread->join();
	delete m_thread;
	m_thread = nullptr;
}

#if USE_WIN32_TIMER
unsigned long WorkerThread::processWin32(void* parameter)
{
	MSG msg;
	bool bRet;

	MMRESULT timerId = timeSetEvent(250, 10, &WorkerThread::processTimer,
			reinterpret_cast<DWORD>(this), TIME_PERIODIC);

	while((bRet = GetMessage(&msg, NULL, WM_USER_BEGIN, WM_USER_END)) != 0)
	{
		switch(msg.message)
		{
		case WM_DISPLAYCHANGE:
			assert(msg.wParam != NULL);

			const UserData* userData = static_cast<const UserData*>(msg.wParam);

			std::cout << userData->getTimeStamp() << " " << userData->year << " on " << threadName << std::endl;

			delete userData;
			break;
		}

		case WM_USER_TIMER:
		{
			std::cout << "Timer expired on " << threadName << std::endl;
			break;
		}

		case WM_EXIT_THREAD:
		{
			timeKillEvent(timerId);
			break;
		}
		default:
			assert(false);
	}
}

#endif


void WorkerThread::processTimer()
{
	stringstream ss;
	ss << std::this_thread::get_id();
	while(!m_timerExit)
	{
//		std::this_thread::sleep_for(std::chrono::duration<std::chrono::milliseconds>(250));
//		int tc = m_timerCycle.load(std::memory_order_relaxed);
		std::this_thread::sleep_for(std::chrono::milliseconds(m_timerCycle));
		// by SGK 2018. 12. 15. add timer pause
		if(m_timerPause) continue;

		UserData* um = new UserData(ss.str());

		ThreadMsg* msg = new ThreadMsg(MsgTimer, um);
		std::unique_lock<std::mutex> lk(m_mutex);
		m_queue.push(msg);
		m_cv.notify_one();
	}
}

int WorkerThread::setTimerCycle(int newCycle)
{
	int oldCycle = m_timerCycle.load(std::memory_order_relaxed);
	if(newCycle == oldCycle) return oldCycle;
	m_timerCycle.store(newCycle, std::memory_order_relaxed);
	return oldCycle;
}

int WorkerThread::getTimerCycle() const
{
	return m_timerCycle.load(std::memory_order_relaxed);
}


