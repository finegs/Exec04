/*
 * ThreadMsg.hpp
 *
 *  Created on: 2018. 12. 9.
 *      Author: finegs
 */

#ifndef THREADMSG_HPP_
#define THREADMSG_HPP_

enum ThreadMsgType{
	MsgPostUserData = 0,
	MsgTimer  = 1,
	MsgExitThread = 2
};

using namespace std::chrono;


class ThreadMsg
{
	friend class WorkerThread;

public:
	ThreadMsg(ThreadMsgType msgType, const UserData* msg)
		: msgType(msgType),
		  msg(msg),
		  tp(system_clock::now()) {
	}

	~ThreadMsg() {
		delete msg;
		msg = nullptr;
	}

	std::string getTimeStamp() {
		using namespace std;
		using namespace std::chrono;

		auto ttime_t = system_clock::to_time_t(tp);
		auto tp_sec = system_clock::from_time_t(ttime_t);
		milliseconds ms = duration_cast<milliseconds>(tp-tp_sec);

		std::tm* ttm = localtime(&ttime_t);

		char dtFormat[] = "%Y%m%d %H:%M:%S";
		char timeStr[] = "yyyymmdd HH:MM:SS.fff";

		strftime(timeStr, strlen(timeStr), dtFormat, ttm);

		std::string rs(timeStr);
		rs.append(".");
		rs.append(std::to_string(ms.count()));
		return rs;
	}

	const UserData* getMsg() const { return msg; }

	private:
		ThreadMsgType msgType;
		const UserData*	  msg;
		std::chrono::system_clock::time_point tp;
};

#endif /* THREADMSG_HPP_ */
