/*
 * algo.cpp
 *
 *  Created on: 2018. 9. 26.
 *      Author: finegs
 */
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstdint>
#include <functional>
#include <boost/asio.hpp>

namespace ip = boost::asio::ip;

int main(int argc, char* argv[]) {

#if 0
	std::vector<int> vec1{1,1,4,3,5,8,6,7,9,2};
	std::vector<int> vec2{1,2,3};

	std::sort(vec1.begin(), vec1.end());
	std::vector<int> vec(vec1);

	vec1.reserve(vec1.size() + vec2.size());
	vec1.insert(vec1.end(), vec2.begin(), vec2.end());

	for(auto v:vec1) std::cout << v << " ";
	std::cout << std::endl;

	std::inplace_merge(vec1.begin(), vec1.end()-vec2.size(), vec1.end());
	for(auto v:vec1) std::cout << v << " ";
	std::cout << std::endl;

	vec2.push_back(10);
	for(auto v:vec) std::cout << v << " ";
	std::cout << std::endl;
	for (auto v:vec2) std::cout << v << " ";
	std::cout << std::endl;

	std::vector<int> res;
	std::set_symmetric_difference(vec.begin(), vec.end(), vec2.begin(), vec2.end(), std::back_inserter(res));

	for(auto v:res) std::cout << v << " ";
	std::cout << std::endl;
	res = {};
	std::set_union(vec.begin(), vec.end(), vec2.begin(), vec2.end(), std::back_inserter(res));

	for(auto v:res) std::cout << v << " ";
	std::cout << std::endl;

#endif

#if 0
	int x = 0;
	std::function<int(std::vector<uint8_t>)> exampleLamda =
			[&x] (std::vector<uint8_t> data)->int
			{
				x = 1;
				std::cout << "Hello from lamda : " << data.size() << std::endl;
				return 2*x;
			};


	std::function<double(std::vector<int>)> l1 =
			[&x] (std::vector<int> data)->int
			{
				x = 2;
				std::cout << "Hello from l1 : " << data.size() << std::endl;
				return (double)4*x;
			};

	std::vector<uint8_t> testData(512);

	int returnValue = exampleLamda(testData);
	std::cout << x << " " << returnValue << std::endl;

	std::vector<int> aa(1024);
	std::cout << x << " " << l1(aa) << std::endl;
	std::cout << x << std::endl;
#endif

//	ip::tcp::iostream s("ai.eecs.umich.edu", "http");
//	s << "GET /people/dreeves/Fox-In-Socks.txt HTTP/1.0\r\n"
//			<< "Host: ai.eecs.umich.edu\r\n"
//			<< "Accept: */*\r\n"
//			<< "Connection: close\r\n\r\n";

	ip::tcp::iostream s("stackoverflow.com", "http");
	s << "GET /questions/38767653/how-to-fix-undefined-reference-to-imp-wsacleanup-boost-asio-in-clion HTTP/1.0\r\n"
			<< "Host: stackoverflow.com\r\n"
			<< "Accept: */*\r\n"
			<< "Connection: close\r\n\r\n";
	std::cout << s.rdbuf();

	return EXIT_SUCCESS;
}


