/*
 * gistfile1.cpp
 *
 *  Created on: 2018. 10. 3.
 *      Author: finegs
 */
// A small example program using SQLite with C++
#include <iostream>
#include <sqlite3.h>
#include <thread>
#include <future>
#include <chrono>
#include <vector>

using namespace std;

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	for(i=0; i<argc; i++)
	{
		cout<<azColName[i]<<" = " << (argv[i] ? argv[i] : "NULL")<< (argc-1 == i ? "" : ", ");
	}
	cout<<"\n";
	return 0;
}


int createDBnCreateTable(sqlite3** db);
int run(sqlite3** db, int total) noexcept;
int dropTablenCloseDB(sqlite3** db);

sqlite3 *db;

int main(int argc, char* argv[]) {

	int waitCnt;

	//	std::thread task(run, 100);

	std::vector<std::future<int>> futures;

	createDBnCreateTable(&db);

	for (int i = 0; i < 10; ++i) {
		futures.push_back(std::async(run, &db, 1000));
	}

	waitCnt = 0;
	bool isDoneAll = false;
	while(!isDoneAll)
	{
		for (const std::future<int>& f : futures) {
			if(f.wait_for(10ms) != std::future_status::ready) {
				isDoneAll = false;
				break;
			}
		}

		if(!isDoneAll) {
			this_thread::sleep_for(chrono::seconds(1));
			cout << "�ޡޡޡޡޡޡޡޡޡ�   Main : waiting for join. " << ++waitCnt << " WaitCount �ޡޡޡޡޡޡޡޡޡ�" << endl;
		}
	}

	dropTablenCloseDB(&db);

	return EXIT_SUCCESS;
}

int createDBnCreateTable(sqlite3** db) {
	char *zErrMsg = 0;
	const char *createTableDDL;
	int rc;

	rc = sqlite3_open("familyGuy.db", db);

	if( rc )
	{
		cout<<"Can't open database: "<<sqlite3_errmsg(*db)<<"\n";
	}
	else
	{
		cout<<"Open database successfully\n\n";
	}

	createTableDDL = "CREATE TABLE myTable (FirstName varchar(30), LastName varchar(30), Age smallint, Hometown varchar(30), Job varchar(30))";

	rc = sqlite3_exec(*db, createTableDDL, callback, 0, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		cout<<"SQL error: "<<sqlite3_errmsg(*db)<<"\n";
		sqlite3_free(zErrMsg);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}


int dropTablenCloseDB(sqlite3** db) {
	char *zErrMsg = 0;
	const int queryCnt = 2;
	const char *query[queryCnt];
	int rc;

	if(!*db) return EXIT_SUCCESS;

	query[0] = "DELETE FROM myTable";

	query[1] = "DROP TABLE myTable";

	for(int i = 0; i < queryCnt; i++)
	{
		rc = sqlite3_exec(*db, query[i], callback, 0, &zErrMsg);
		if( rc != SQLITE_OK )
		{
			cout<<"SQL error: "<<sqlite3_errmsg(*db)<<"\n";
			sqlite3_free(zErrMsg);
			break;
		}
	}

	rc = sqlite3_close(*db);

	db = nullptr;

	if( rc )
	{
		cout<<"Can't close database: "<<sqlite3_errmsg(*db)<<"\n";
	}
	else
	{
		cout<<"Close database successfully\n\n";
	}

	return EXIT_SUCCESS;
}

int run(sqlite3** db, int total) noexcept
{
	int subRun = 0;
	std::vector<std::string> querys;
	while(subRun < total) {
		char *zErrMsg = 0;
		int rc;

		if(!db) break;

		querys.clear();

		querys.push_back("INSERT INTO myTable (FirstName, LastName, Age, Hometown, Job) VALUES ('Peter', 'Griffin', 41, 'Quahog', 'Brewery')");

		querys.push_back("INSERT INTO myTable (FirstName, LastName, Age, Hometown, Job) VALUES ('Lois', 'Griffin', 40, 'Newport', 'Piano Teacher')");

		querys.push_back("INSERT INTO myTable (FirstName, LastName, Age, Hometown, Job) VALUES ('Joseph', 'Swanson', 39, 'Quahog', 'Police Officer')");

		querys.push_back("INSERT INTO myTable (FirstName, LastName, Age, Hometown, Job) VALUES ('Glenn', 'Quagmire', 41, 'Quahog', 'Pilot')");

		querys.push_back("SELECT * FROM myTable");

		for(size_t i = 0; i < querys.size(); i++)
		{
			rc = sqlite3_exec(*db, querys[i].c_str(), callback, 0, &zErrMsg);
			if( rc!=SQLITE_OK )
			{
				cout<<"SQL error: "<<sqlite3_errmsg(*db)<<"\n";
				sqlite3_free(zErrMsg);
				break;
			}
		}

		this_thread::sleep_for(std::chrono::milliseconds(100));

		++subRun;

		cout << " ##### [" << subRun << "/" << total << "] #####" << endl;

	}

	return EXIT_SUCCESS;
}



