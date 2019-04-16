/*
 * AppCLI.hpp
 *
 *  Created on: 2018. 12. 16.
 *      Author: finegs
 */

#ifndef APPCLI_HPP_
#define APPCLI_HPP_

#include <string>

struct AppCLICmd {

	enum AppCLICmdType
	{
		cHelp,
		cStatus,
		cExit,
		cPause,
		cResume,
		cPauseTimer,
		cResumeTimer,
		cResetTimeCycle,
		cStringTest
	};

	static AppCLICmd::AppCLICmdType toAppCLICmd(const std::string& str)
	{
		if(str == "-help") return cHelp;
		else if(str == "-status") return cStatus;
		else if(str == "-exit") return cExit;
		else if(str == "-pause") return cPause;
		else if(str == "-resume") return cResume;
		else if(str == "-pauseTimer") return cPauseTimer;
		else if(str == "-resumeTimer") return cResumeTimer;
		else if(str == "-rtc") return cResetTimeCycle;
		else if(str == "-stest") return cStringTest;
		else return cHelp;
	}
};

#endif /* APPCLI_HPP_ */
