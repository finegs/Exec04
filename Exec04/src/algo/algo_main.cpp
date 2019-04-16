/*
 * algo.cpp
 *
 *  Created on: 2018. 9. 26.
 *      Author: finegs
 */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <thread>

#include <boost/asio/ip/tcp.hpp>

namespace ip = boost::asio::ip;

using namespace std;

typedef struct BSTNode_ {
	BSTNode_ *left,*right;
	int data;
} BSTNode;

void log(const char *msg);
void handleUserCmd();

BSTNode* findMinNode(BSTNode* parent);
BSTNode* findMaxNode(BSTNode* parent);
BSTNode* findNode(BSTNode* parent, int data);
BSTNode* insertNode(BSTNode *parent, int data);
BSTNode* deleteNode(BSTNode *parent, int data);

bool isRun;

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

#if 0
	ip::tcp::iostream s("stackoverflow.com", "http");
	s << "GET /questions/38767653/how-to-fix-undefined-reference-to-imp-wsacleanup-boost-asio-in-clion HTTP/1.0\r\n"
			<< "Host: stackoverflow.com\r\n"
			<< "Accept: */*\r\n"
			<< "Connection: close\r\n\r\n";
	std::cout << s.rdbuf();
#endif


	thread usrCmdWorker(handleUserCmd);

	usrCmdWorker.join();

	return EXIT_SUCCESS;
}

void log(const char* msg) {
	std::cout << msg << std::endl;
}

void handleUserCmd() {

	string s, p, cmd, buf;
	unique_ptr<char*> lm(char[200]);

	vector<string> cmds;

	log("Command Reader is created and started");

	isRun = true;

	while(isRun) {
		cmds.clear();

		getline(cin, cmd);
		stringstream ss(cmd);

		while(ss>>buf) cmds.push_back(buf);

		if(!cmd.empty() && (cmds.size() < 1 || (cmd = cmds.at(0)).size() < 1)) {
			log("Empty Command : ");
			continue;
		}

		std::transform(cmd.begin(),
				cmd.end(),
				cmd.begin(),
				::tolower);

		if("stop" == cmd) {
			log("Notifying stop..");
			isRun = false;
		}
		else if("start" == cmd) {
			log("Starting ...");
		}
	}
}

BSTNode* findNode(BSTNode* parent, int data) {
	if(parent==NULL) return NULL;
	while(parent) {
		if(data == parent->data) return parent;
		else if(data > parent->data) parent = parent->right;
		else parent = parent->left;
	}
	return NULL;
}


BSTNode* findMinNode(BSTNode* parent) {
	if(parent == NULL) {
		return NULL;
	}
	while(parent->left != NULL)
		parent = parent->left;
	return parent;

}

BSTNode* findMaxNode(BSTNode* parent) {
	if(parent == NULL) {
		return NULL;
	}
	while(parent->right != NULL)
		parent = parent->right;
	return parent;
}

BSTNode* insertNode(BSTNode *parent, int data) {
	if(parent == NULL) {
		parent = (BSTNode*)malloc(sizeof(BSTNode));
		if(parent == NULL) {
			printf("insertNode : malloc failed, data=%d\n", data);
			return NULL;
		}
		else {
			parent->data = data;
			parent->left = parent->right = NULL;
		}
	}
	else {
		if(data < parent->data) {
			parent->left = insertNode(parent->left, data);
		}
		else if(data > parent->data) {
			parent->right = insertNode(parent->right, data);
		}
	}

	return parent;
}


BSTNode* deleteNode(BSTNode *parent, int data) {

	BSTNode *temp;

	if(parent == NULL) {
		printf("Parent is NULL\n");
	}
	else if(data < parent->data) {
		parent->left = deleteNode(parent->left, data);
	}
	else if(data > parent->data) {
		parent->right = deleteNode(parent->right, data);
	}
	else { // Found
		if(parent->left && parent->right) {
			temp = findMaxNode(parent->left);
			parent->data = temp->data;
			parent->left = deleteNode(parent->left, parent->data);
		}
		else {
			temp = parent;
			if(parent->left == NULL) parent = parent->right;
			if(parent->right == NULL) parent = parent->left;
		}
	}
	return parent;
}
