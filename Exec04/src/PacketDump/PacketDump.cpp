/*
 * PacketDump.cpp
 *
 *  Created on: 2018. 9. 30.
 *      Author: finegs
 */
#include<cstdlib>
#include<iostream>
#include<fstream>
#include<map>
#include<string>
#include<vector>

using namespace std;
struct Symbol {
	string name;
};
struct Message {
	string get_message_content() { return content; }
	Symbol symbol;
	string content;
};

struct Packet {
	string get_header() const { return header; }
	string header;
	vector<Message> messages;
};



void write_packet_to_symbo_file(string path, Packet p) {
	map<string, ofstream&> outs;
	for(Message m : p.messages) {
		map<string, ofstream&>::iterator it = outs.find(m.symbol.name);
		if(it == outs.end()) {
			string full_path = path + m.symbol.name + ".CAP";
//			ofstream& of = outs[m.symbol.name];
//			create new ofstream with full_path
			ofstream of(full_path, std::ofstream::out);
			of.open(full_path, ios_base::app);
			of << p.get_header();
			outs.emplace(m.symbol.name, of);
		}

		map<string, ofstream&>::iterator it2 = outs.find(m.symbol.name);
		if(it2!=outs.end())
			it2->second << m.get_message_content();
	}
}



