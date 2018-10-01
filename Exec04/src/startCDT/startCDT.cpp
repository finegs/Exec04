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

const std::size_t ENV_BUF_SIZE = 1024; // Enough for your PATH?

int main()
{
    char buf[ENV_BUF_SIZE];
    std::size_t bufsize = ENV_BUF_SIZE;
    int e = getenv_s(&bufsize,buf,bufsize,"PATH");
    if (e) {
        std::cerr << "`getenv_s` failed, returned " << e << '\n';
        exit(EXIT_FAILURE);
    }
    std::string env_path = buf;
    std::cout << "In main process, `PATH`=" << env_path << std::endl;
    env_path += ";C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\bin\\amd64";
    e = _putenv_s("PATH",env_path.c_str());
    if (e) {
        std::cerr << "`_putenv_s` failed, returned " << e << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << std::endl;
    e = std::system("echo \"In child process `PATH`=%PATH%\"");
    if (e) {
        std::cerr << "`std::system` failed, returned " << e << std::endl;
        exit(EXIT_FAILURE);
    }
    return 0;
}

#endif

