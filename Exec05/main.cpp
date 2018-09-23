/*
 * main.cpp
 *
 *  Created on: 2017. 12. 10.
 *      Author: finegs
 */

#if 0
#include <stdio.h>
#include <iostream>

#ifdef _DEBUG
setbuff(stdout, NULL);
setbuff(stderr, NULL);
#endif

int main()
{
	using namespace std;

	int examples;

	cout << "Enter examples" << endl;

	cin >> examples;

	cout << examples << endl;

	examples = 25;

	cout << "I have ";
	cout << examples;
	cout << " examples. ";
	cout << endl;
	examples = examples -1;
	cout << "I have " << examples << " examples." << endl;

	std::string str;
	std::string base = "The quick brown for jumps over a lazy dog.";

	  str.assign(base,10,9);
	  std::cout << str << '\n';         // "brown fox"

	  str.assign("pangrams are cool",7);
	  std::cout << str << '\n';         // "pangram"

	  str.assign("c-string");
	  std::cout << str << '\n';         // "c-string"

	  str.assign(10,'*');
	  std::cout << str << '\n';         // "**********"

//	  str.assign(10, 0x2D);
//	  std::cout << str << '\n';         // "----------"

	  str.assign(base.begin()+16,base.end()-12);
	  std::cout << str << '\n';         // "fox jumps over"
	return 0;
}





#endif

// condition_variable example
#include <iostream>           // std::cout
#include <thread>             // std::thread
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable

std::mutex mtx;
std::condition_variable mainCmdCV;
bool ready = false;

void print_id (int id) {
  std::unique_lock<std::mutex> lck(mtx);
  while (!ready) mainCmdCV.wait(lck);
  // ...
  std::cout << "thread [" << std::this_thread::get_id() << "] id = " << id << '\n';
}

void go() {
  std::unique_lock<std::mutex> lck(mtx);
  ready = true;
  mainCmdCV.notify_all();
}

int main ()
{
  std::thread threads[10];
  // spawn 10 threads:
  for (int i=0; i<10; ++i)
    threads[i] = std::thread(print_id,i);

  std::cout << "10 threads ready to race...\n";
  go();                       // go!

  for (auto& th : threads) th.join();

  return EXIT_SUCCESS;
}
