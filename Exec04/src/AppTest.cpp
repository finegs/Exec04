//============================================================================
// Name        : exec03.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#define LOGGER 0
#define CV 0
#define LIB_TEST 0
#define MAP_TEST 0
#define FUNC_PT_MAP 1

#ifndef LOGGER

#include <bitset>
#include <iostream>
#include <algorithm>
#include <map>

using namespace std;

template <class K,class V>
class value_equals {
private:
	V value;
public:
	value_equals(const V& v) : value(v) {
	}

	bool operator() (pair<const K, V> elem) {
		return elem.second == value;
	}
};

int main() {
//	string str;
//
////	setvbuf(stdout, NULL, _IONBF, 0);
////	setvbuf(stderr, NULL, _IONBF, 0);
//
//	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
//	cout << "Enter string" << endl;
//	fflush(stdout);
////	cin >> str;
//	getline(cin, str);
//	cout << "Entered string is "<< str << endl;



#if 0
	bitset<10> bitsets(2);

	bitsets.set(3);
	bitsets.set(6);

	bitsets[8] = true;
	bitsets[7] = true;
	bitsets[9] = bitsets[3];

	if(bitsets.test(3))
		cout << "bit 3 is set!\n" << endl;
	cout <<bitsets << endl;
#endif

	typedef map<float, float> FloatFloatMap;
	FloatFloatMap coll;
	FloatFloatMap::iterator pos;

	coll[1] = 7;
	coll[2] = 4;
	coll[3] = 2;
	coll[4] = 3;
	coll[5] = 6;
	coll[6] = 1;
	coll[7] = 3;

	pos= find_if(coll.begin(), coll.end(), value_equals<float, float>(3.0));
	if(pos!= coll.end()) {
		cout << pos->first << ": "
				<< pos->second << endl;
	}

	return 0;
}

#endif

#if LOGGER

//#define _USE_32BIT_TIME_T


#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <vector>
#include <sys/time.h>
#include <time.h>
#include <thread>
#include <mutex>
//#include <bits/std_mutex.h>
#include <unistd.h>
#include <chrono>
#include <condition_variable>
#include <algorithm>

#include <AsyncLogger.hpp>

using namespace std;
using namespace chrono;
//using namespace chrono_literals;

using namespace asynclogger;

static AsyncLogger logger;

char TS[25];

mutex mainCmdMTX;
condition_variable mainCmdCV;
bool isRun = true;
bool isPaused = false;
//auto  sleepInterval = 3000ms;
milliseconds sleepInterval(10000);

void log(const std::string& msg) {
	logger.info(msg);
}

using namespace std;
vector<int> getPi(string p) {
	int m = (int)p.size(),j=0;
	vector<int> pi(m, 0);
	for(int i= 1;i<m;i++) {
		while(j>0&&p[i] != p[j])
			j = pi[j-1];
		if(p[i] == p[j])
			pi[i] = ++j;
	}
	return pi;
}

vector<int> kmp(string s, string p) {
	vector<int> ans;
	auto pi = getPi(p);
	int n = (int)s.size(), m = (int)p.size(), j = 0;
	for(int i = 0;i<n;i++) {
		while(j>0&&s[i] !=p[j])
			j = pi[j-1];
		if(s[i]==p[j]) {
			if(j==m-1) {
				ans.push_back(i-m+1);
				j = pi[j];
			}
			else {
				j++;
			}
		}
	}

	return ans;
}

void subTask(string s, string p)
{
	char msg[100];

	log("kmp worker is started");

	for (int i = 0; i < 10000 && isRun; ++i) {
		unique_lock<mutex> lck(mainCmdMTX);

		memset(msg, '\0', strlen(msg));
		if(!isPaused) {
			log(sformat("i=%5d : start", i));

			auto matched = kmp(s, p);
			log(sformat("i=%5d : %010d", i, (int)matched.size()));
			for(auto ii: matched) {
				sprintf(msg, "i=%5d : %d", i, ii);
				log(msg);
			}

			log(sformat("i=%5d : end", i));
		}
		else {
			log(sformat("i=%5d : paused", i));
		}

		mainCmdCV.wait_for(lck, sleepInterval);

		lck.unlock();
	}

	log("kmp worker is stop.");
}

void handleUserCmd() {

	string s, p, cmd, buf;
	char lm[200];
	vector<string> cmds;

	log("Command Reader is created and started");

	while(isRun) {
		cmds.clear();

		getline(cin, cmd);
		stringstream ss(cmd);

		while(ss>>buf) cmds.push_back(buf);

		if(!cmd.empty() && (cmds.size()<1|| (cmd = cmds.at(0)).size() < 1)) {
			log("Empty Command : ");
			continue;
		}

		std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

		if("stop" == cmd) {
			log("Notifying stop..");
			isRun = false;
			logger.stop();
		}
		else if ("start" == cmd){
			log("Notifying start..");
			isPaused = false;
			isRun = true;
			logger.start();
		}
		else if("pause" == cmd) {
			log("Notifying pause");
			isRun = true;
			isPaused = true;
			logger.pause();
		}
		else if("resume" == cmd) {
			log("Notifying resume");
			isPaused = false;
			logger.resume();
		}
		else if ("get" == cmd) {
			if(cmds.size() > 1) {
				if(stricmp("qs",  cmds.at(1).c_str())==0) {
					log(sformat("Queue Size= %d", logger.getQueueSize()));
				}
				else if(stricmp("si",  cmds.at(1).c_str())==0) {
					memset(lm, '\0', strlen(lm));
					log(sformat("Sleep Interval(si)= %lld", sleepInterval.count()));
				}
				else if(stricmp("ll",  cmds.at(1).c_str())==0) {
					memset(lm, '\0', strlen(lm));
					log(sformat("Log Level(ll)= %s(%d)", AsyncLogLevel_To_Str(logger.getLevel()), logger.getLevel()));
				}
				else {
					memset(lm, '\0', strlen(lm));
					log(sformat("unsupported parameter: %s", cmds.at(1).c_str()));
				}
			}
			else {
				memset(lm, '\0', strlen(lm));
				log(sformat("unsupported argument count : %lld", cmds.size()));
			}
		}
		else if ("set" == cmd) {
			if(cmds.size() > 2) {
				if(stricmp("si",  cmds.at(1).c_str())==0) {
					int newSi = atoi(cmds.at(2).c_str());

					milliseconds newSleepInt(newSi);

					if(sleepInterval != newSleepInt) {
						memset(lm, '\0', strlen(lm));
						log(sformat("sleepInterval changed : %lld -> %lld", sleepInterval.count(), newSleepInt.count()));

						sleepInterval = newSleepInt;
					}
				}
				else if(stricmp("ll",  cmds.at(1).c_str())==0) {
					int newVal = atoi(cmds.at(2).c_str());
					AsyncLogLevel newLogLevel;
					try {
						newLogLevel = static_cast<AsyncLogLevel>(newVal);
					}
					catch (...) {
						memset(lm, '\0', strlen(lm));
						log(sformat("Unsupported LogLevel : %d", newVal));
						continue;
					}

					if(logger.getLevel() != newLogLevel) {
						memset(lm, '\0', strlen(lm));
						log(sformat("LogLevel changed : %d -> %d", logger.getLevel(), newLogLevel));

						logger.setLevel(AsyncLogLevel(newVal));
					}
				}
				else {
					memset(lm, '\0', strlen(lm));
					log(sformat("unsupported parameter: %s", cmds.at(1).c_str()));
				}
			}
			else {
				memset(lm, '\0', strlen(lm));
				log(sformat("unsupported argument count : %lld", cmds.size()));
			}
		}
		else {
			log("Notifying nothing.. : ");
		}

		mainCmdCV.notify_all();
	}
}

int main() {

	using namespace asynclogger;
	using namespace this_thread;

	string s, p, cmd, buf;
	vector<string> cmds;

	// by SGK 2017. 12. 17. start Logger Writer
	logger.start();

	cout << "Enter string : "; cout.flush();
	// by SGK 2017. 12. 17. read string
	getline(cin, s);

	cout << "Enter pattern : "; cout.flush();

	// by SGK 2017. 12. 17. read pattern
	getline(cin, p);

	thread kmpWorker(subTask, s, p);

	thread usrCmdWorker(handleUserCmd);

	kmpWorker.join();
	usrCmdWorker.join();
	logger.join();

	return 0;
}

#endif

#if 0

std::mutex m;
std::condition_variable cv;
std::string data;
bool ready = false;
bool processed = false;

void worker_thread()
{
    std::cout << "Worker thread is ready for start #\n";
    // Wait until main() sends data
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, []{
    		std::cout << "Worker after waiting .... #\n";
    		return ready;
    });

    // after the wait, we own the lock.
    std::cout << "Worker thread is processing data\n";
    data += " after processing";

    // Send data back to main()
    processed = true;
    std::cout << "Worker thread signals data processing completed\n";

    // Manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    lk.unlock();
    cv.notify_one();
}

int main()
{
    std::thread worker(worker_thread);

    data = "Example data";
    // send data to the worker thread
    {
        std::lock_guard<std::mutex> lk(m);
        ready = true;
        std::cout << "main() signals data ready for processing\n";
    }
    cv.notify_one();

    // wait for the worker
    {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, []{
        		std::cout << "main() after waiting @\n";
        		return processed;
        });
    }
    std::cout << "Back in main(), data = " << data << '\n';

    worker.join();
}

#endif


#if CV

#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <vector>
#include <sys/time.h>
#include <time.h>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <chrono>
#include <deque>
#include <condition_variable>

using namespace std;
std::deque<int> q;
std::mutex mu;
std::condition_variable cond;

void function_1() {
	int count = 10;
	while(count > 0) {
		std::unique_lock<std::mutex> locker(mu);
		q.push_front(count);
		locker.unlock();
		cond.notify_all();
		this_thread::sleep_for(chrono::seconds(1));
		count--;
	}
}

void function_2() {
	int data = 0;
	while(data != 1) {
		std::unique_lock<std::mutex> locker(mu);
		cond.wait(locker, [](){ return !q.empty();}); // spurious wake
		data = q.back();
		q.pop_back();
		locker.unlock();
		cout << "t2 got a value from t1: " << data << std::endl;
	}
}

int main()
{
	cout << "start" << std::endl;

	std::thread t1(function_2);
	std::thread t2(function_1);

	t1.join();
	t2.join();

	cout << "done." << std::endl;
}

#endif

#if LIB_TEST

#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <map>
#include <unordered_map>

int main (int argc, char* argv[])
{
    std::unordered_map<int, std::string> dict = {{1, "one"}, {2, "two"}};
    dict[3] = "three";
    dict.emplace(3, "three");
    dict.emplace(4, "another four");
    dict.emplace(5, "five");

    bool ok = dict.emplace(1, "another one").second;
    std::cout << "inserting 1 -> \"another one\" "
              << (ok ? "succeeded" : "failed") << '\n';

    std::cout << "contents:\n";
    for(auto& p: dict)
        std::cout << " " << p.first << " => " << p.second << '\n';
}

#endif

#if MAP_TEST

#include <iostream>
#include <cstdlib>
#include <boost/flyweight.hpp>
#include <map>

namespace my
{
	using string = boost::flyweight<std::string>;


	struct Bar
	{

	};

	struct Foo
	{
		std::map<my::string, my::Bar*> bars;
	};
}

using namespace my;

int main(int argc, char* argv[])
{
	Foo foo;
	foo.bars.emplace("hello", new Bar());
	foo.bars.emplace(std::string("world"), new Bar());

	return EXIT_SUCCESS;
}

#endif


#if FUNC_PT_MAP

#include <iostream>
#include <cstdlib>
#include <string>

std::string foo() { return "Foo"; }
std::string bar() { return "Bar"; }

int main()
{
    std::map<std::string, std::string (*)()> m;

    // Map the functions to the names
    m["foo"] = &foo;
    m["bar"] = &bar;

    // Display all of the mapped functions
    std::map<std::string, std::string (*)()>::const_iterator it = m.begin();
    std::map<std::string, std::string (*)()>::const_iterator end = m.end();

    while ( it != end ) {
        std::cout<< it->first <<"\t\""
            << (it->second)() <<"\"\n";
        ++it;
    }
}

#endif
