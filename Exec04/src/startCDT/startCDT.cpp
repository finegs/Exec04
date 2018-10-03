/*
 * startCDT.cpp
 *
 *  Created on: 2018. 10. 1.
 *      Author: finegs
 */
#if 0

#include <cstdlib>
#include <windows.h>

using namespace std;

int WINAPI
WinMain (HINSTANCE hIntance, HINSTANCE hPrevInst, LPTSTR lpCmdLine, int nShowCmd)
{
	int option = MessageBoxW(NULL, L"Start CDT", L"Are you sure to startCDT?", MB_OKCANCEL|MB_ICONQUESTION);

//	if(MB_OK != option) {
//		MessageBoxW(NULL, L"Abort Start CDT", L"Abort Start CDT!!!", MB_OK|MB_ICONINFORMATION);
//		return EXIT_SUCCESS;
//	}

	system("echo off");
	string s = "set PATH=C:\\MinGW-w64\\mingw64\\bin"
			 ";C:\\MinGW-w64\\mingw64\\msys64\\bin"
			 ";C:\\MinGW-w64\\mingw64\\x86_64-w64-mingw32\\bin"
			 ";C:\\MinGW-w64\\mingw64\\msys64\\usr\\bin;%PATH%";
	system("cd C:\\");
	system("cd eclipse\\jee-oxygen\\eclipse");
	system("start /B eclipse");

	Sleep(1000);

	return 0;

}

#endif

#if 1

#include <iostream>
#include <string>
#include <cstdlib>
#ifdef _WIN32
#include <direct.h>
#endif

const std::size_t ENV_BUF_SIZE = 1024; // Enough for your PATH?

int executeSystemCommand(const char* cmd) {
	int e;

	e = EXIT_SUCCESS;
    e = std::system(cmd);
    if (e) {
        std::cerr << "`std::system` failed, cmd=" << cmd << ", returned " << e << std::endl;
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
    char* buf;
    if(!(buf = (char*)malloc(ENV_BUF_SIZE))) {
        std::cerr << "Fail to malloc, len=" << ENV_BUF_SIZE << std::endl;
    	return EXIT_FAILURE;
    }
//    std::size_t bufsize = ENV_BUF_SIZE;
//    int e = getenv_s(&bufsize,buf,bufsize,"PATH");
    int e = (NULL != (buf = getenv("PATH"))) ? EXIT_SUCCESS : EXIT_FAILURE;
    if (e) {
        std::cerr << "`getenv_s` failed, return= " << e << ", buf=" << buf << std::endl;
        return EXIT_FAILURE;
    }
    std::string env_path = buf;

    std::string s =
    		"C:\\MinGW-w64\\mingw64\\bin"
    		";C:\\MinGW-w64\\mingw64\\msys64\\bin"
    		";C:\\MinGW-w64\\mingw64\\x86_64-w64-mingw32\\bin"
    		";C:\\MinGW-w64\\mingw64\\msys64\\usr\\bin;";
    env_path.insert(0, s.c_str());

    std::cout << "In main process, `PATH`=" << env_path << std::endl;
    env_path += ";C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\bin\\amd64";
    e = _putenv_s("PATH",env_path.c_str());
    if (e) {
        std::cerr << "`_putenv_s` failed, returned " << e << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << std::endl;

    e = executeSystemCommand("echo off");
    e = executeSystemCommand("echo \"In child process `PATH`=%PATH%\"");

//    chdir("C:\\");
//    chdir("C:\\eclipse\\jee-oxygen\\eclipse");

//    e = executeSystemCommand("C:\\");
//    e = executeSystemCommand("cd C:\\eclipse\\jee-oxygen\\eclipse");
    e = executeSystemCommand("pwd");

    std::string ss = "start "
    		"\/D " "C:\\eclipse\\jee-oxygen\\eclipse"
			"\/B "
			"eclipse";
    e = executeSystemCommand(ss.c_str());

    return EXIT_SUCCESS;
}

#endif

