
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

	return 0;
}


#endif

#if 0

#include <iostream>
#include <queue>
#include <string>

int main()
{
	std::priority_queue<std::string> mypq;

	std::string line;

	std::cin >> line;

	mypq.emplace(line);

	std::cout << line << std::endl;


	mypq.emplace("orange");
	mypq.emplace("strawberry");
	mypq.emplace("apple");
	mypq.emplace("pear");

	std::cout << "mypq contains: ";
	while(!mypq.empty())
	{
		std::cout << ' ' << mypq.top();
		mypq.pop();
	}
	std::cout << '\n';

	return 0;
}

#endif

#include <chrono>
#include <iostream>

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

void print_ms(const TimePoint& point)
{
    using Ms = std::chrono::milliseconds;
    const Clock::duration since_epoch = point.time_since_epoch();
    std::cout << std::chrono::duration_cast<Ms>(since_epoch).count() << " ms\n";
}

int main()
{
    const TimePoint default_value = TimePoint(); // (1)
    print_ms(default_value); // 0 ms

    const Clock::duration duration_4_seconds = std::chrono::seconds(4);
    const TimePoint time_point_4_seconds(duration_4_seconds); // (2)
      // 4 seconds from start of epoch
    print_ms(time_point_4_seconds); // 4000 ms

    const TimePoint time_point_now = Clock::now(); // (3)
    print_ms(time_point_now); // 43098276 ms


}
