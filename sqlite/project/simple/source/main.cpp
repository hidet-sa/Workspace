#include <stdlib.h>
#include <stdio.h>
#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include "sqlite3.h"

// common function --------------------------------------------------------------------------------
#define ARRAY_SIZE(A) (sizeof(A)/sizeof((A)[0]))

void DebugLog(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	{
		char tmp[1024];
		vsnprintf(tmp, ARRAY_SIZE(tmp), fmt, ap);
		OutputDebugStringA(tmp);
	}
	va_end(ap);
}

#if defined(_DEBUG)
#define Log(format, ...) DebugLog(format, ##__VA_ARGS__);
#else
#define Log(...)	do {} while(0);
#endif

// sqliteラッパークラス ---------------------------------------------------------------------------
class SQLite3 {
public:
	SQLite3(std::string &dbName);
	virtual ~SQLite3();
public:
	bool exec(std::string &sql);
	void createTable(std::string &tableName);
	void select();
	void update();
	void remove(std::string &tableName);
	void checkConnect();
	int getTables(std::vector<std::string> &tableList);
	const char *getDbName() { return _dbName.c_str(); }
private:
	void freeErrorMessage();
private:
	sqlite3 *_db;
	std::string _dbName;
	char *_errorMessage;
};

SQLite3::SQLite3(std::string &dbName) : _db(nullptr), _errorMessage(nullptr)
{
	auto ret = sqlite3_open(dbName.c_str(), &_db);
	if (ret != SQLITE_OK) {
		_db = nullptr;
		assert(ret);
		return;
	}
	_dbName = dbName;
}

bool SQLite3::exec(std::string &sql)
{
	freeErrorMessage();

	char *result = nullptr;
	auto callback = [](void *result_, int argc, char **argv, char **colNames) {
		for (int i = 0; i < argc; i++) {
			if (argv[i] != nullptr) {
				Log("%s = %s\n", colNames[i], argv[i] ? argv[i] : "NULL");
			}
		}
		//std::vector<Row> *ret = reinterpret_cast<std::vector<Row> *>(result_);
		//ret->push_back(Row());
		//for (int i = 0; i < argc; i++) {
		//if (argv[i] != nullptr) {
		//ret->back()
		//.push_back(std::make_pair(colNames[i],
		//Value(new std::string(argv[i]))));
		//}
		//else {
		//ret->back()
		//.push_back(std::make_pair(colNames[i], Value(nullptr)));
		//}
		//}
		return SQLITE_OK;
	};

	auto ret = sqlite3_exec(
		_db,			// DBコネクション
		sql.c_str(),	// SQL文
		callback,		// コールバック関数
		result,			// CB関数に渡す引数
		&_errorMessage	// エラーメッセージ
	);

	if (SQLITE_OK != ret) {
		freeErrorMessage();
		return false;
	}
	return true;
}

void SQLite3::freeErrorMessage()
{
	if (_errorMessage != nullptr) {
		sqlite3_free(_errorMessage);
		_errorMessage = nullptr;
	}
}

SQLite3::~SQLite3()
{
	freeErrorMessage();
	if (_db != nullptr) {
		sqlite3_close(_db);
		_db = nullptr;
	}
}

// main -------------------------------------------------------------------------------------------
int main()
{
	auto db = new SQLite3(std::string("test.db"));
	if (db == nullptr) {
		return 1;
	}

	//テーブルを作成する(create table)
	db->exec(std::string("create table student(id int, name varchar(64), age int, sex int);"));
	//データを新規作成する(insert)
	db->exec(std::string("insert into student(id, name, age, sex) values(1, 'aaaaa', 10, 0);"));
	db->exec(std::string("insert into student(id, name, age, sex) values(2, 'bbbbb', 21, 1);"));
	db->exec(std::string("insert into student(id, name, age, sex) values(3, 'ccccc', 32, 2);"));

	db->exec(std::string("select * from student;"));

	//データを更新する。
	db->exec(std::string("update student set name='ddddd' where id=1;"));

	db->exec(std::string("select * from student;"));

	//データを削除する。
	db->exec(std::string("delete from student where id = 1;"));
	db->exec(std::string("delete from student where id = 2;"));
	db->exec(std::string("delete from student where id = 3;"));

	//テーブル削除。
	db->exec(std::string("drop table student;"));

	//テーブル一覧。
	db->exec(std::string("select name from sqlite_master where type='table';"));

	//接続中のデータベースを確認する
	Log("%s", db->getDbName());

	//close
	delete db;

	return 0;
}
