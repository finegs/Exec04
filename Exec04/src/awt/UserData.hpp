/*
 * UserData.hpp
 *
 *  Created on: 2018. 12. 9.
 *      Author: finegs
 */

#ifndef USERDATA_HPP_
#define USERDATA_HPP_

#include <cstdlib>
#include <cstring>
#include <string>
#include <chrono>
#include <ctime>

using namespace std;
using namespace std::chrono;

class UserData
{
public:
	UserData(std::string msg)
		: tp(system_clock::now()),
			msg(std::move(msg)) {}
	std::string& getTimeStamp() const
	{
		using namespace std;
		using namespace std::chrono;

		auto ttime_t = system_clock::to_time_t(tp);
		auto tp_sec = system_clock::from_time_t(ttime_t);
		milliseconds ms = duration_cast<milliseconds>(tp-tp_sec);

		std::tm* ttm = localtime(&ttime_t);

		char dtFormat[] = "%Y%m%d %H:%M:%S";
		char timeStr[] = "yyyymmdd HH:MM:SS.fff";

		strftime(timeStr, strlen(timeStr), dtFormat, ttm);

		static std::string rs(timeStr);
		rs.append(".");
		rs.append(std::to_string(ms.count()));
		return rs;
	}

	system_clock::time_point tp;
	std::string msg;
};

#endif /* USERDATA_HPP_ */
