/*
 * algo.cpp
 *
 *  Created on: 2018. 9. 26.
 *      Author: finegs
 */
#include <iostream>
#include <algorithm>
#include <vector>

int main(int argc, char* argv[]) {
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

	return EXIT_SUCCESS;

}


