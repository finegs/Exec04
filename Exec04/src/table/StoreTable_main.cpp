/*
 * StoreTable.cpp
 *
 *  Created on: 2018. 9. 30.
 *      Author: finegs
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <algorithm>

using namespace std;

struct StoreItem {
	int data_field;
};

struct StoreItemComp
{
	explicit StoreItemComp(int i) : _n(i) { }
	bool operator() (const StoreItem& i) const { return i.data_field == _n; }
	private:
		int _n;
};

struct StoreTable {
	std::vector<StoreItem> v_records;

	void insert_record(int item) {
		StoreItem storeItem;
		storeItem.data_field = item;
		v_records.push_back(storeItem);
	}

	void insert_record(StoreItem i) {
		v_records.push_back(i);
	}

	void update_data_field(size_t pos, int item){
		if(pos < v_records.size()) v_records[pos].data_field = item;
	}

	void delete_record(const int item) {
//		StoreItem i;
//		i.data_field = item;
//		auto itr = std::find_if(v_records.begin(), v_records.end(), StoreItemComp(item));
		auto itr = std::find_if(v_records.begin(), v_records.end(), [item](const StoreItem& i) -> bool { return i.data_field == item;});
//		auto itr = std::find(v_records.begin(), v_records.end(), i);
		if(itr != v_records.end()) { v_records.erase(itr); }
	}

	void clear() {
		v_records.clear();
	}

	friend std::ostream& operator<<(std::ostream& ostr, const StoreTable& table);

	friend std::istream& operator>>(std::istream& istr, StoreTable& table);
};

std::ostream& operator<<(std::ostream& ostr, const StoreTable& table) {
	for(auto storeItem : table.v_records) ostr << storeItem.data_field << '\n';
	return ostr;
}

std::istream& operator>>(std::istream& istr, StoreTable& table) {
	int iValue;
	table.clear();
	while(istr >> iValue) {
		StoreItem si;
		si.data_field = iValue;
		table.v_records.push_back(si);

	}
	return istr;
}


int main(int argc, char* argv[]) {
	StoreTable table;
	for(int i = 0;i<5;i++) {
		StoreItem si;
		std::cin >> si.data_field;
		table.insert_record(si);
	}

	std::ofstream ofile("my_file.txt");
	ofile<<table;
	ofile.close();


	table.clear();
	std::ifstream ifile("my_file.txt");
	ifile>>table;
	ifile.close();
	std::cout << table;

	return EXIT_SUCCESS;
}
