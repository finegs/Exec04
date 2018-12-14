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

void WorkerThread::process()
{
	m_timerExit = false;
	std::thread timerThread(&WorkerThread::processTimer, this);

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

			delete userData;
			delete msg;

			break;
		}

		case MsgTimer:
		{
			std::cout << msg->getTimeStamp() << " Timer expired on " << threadName << std::endl;

			delete msg;
			break;
		}

		case MsgExitThread:
		{
			std::cout << msg->getTimeStamp() << threadName << " is about to exit. TimerThreadId = " << m_timerThread.get_id() << std::endl;

			m_timerExit = true;
			m_timerThread.join();

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
	while(!m_timerExit)
	{
		std::this_thread::sleep_for(250ms);

		ThreadMsg* msg = new ThreadMsg(MsgTimer, 0);
		std::unique_lock<std::mutex> lk(m_mutex);
		m_queue.push(msg);
		m_cv.notify_one();
	}
}

