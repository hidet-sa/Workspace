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

// sqlite���b�p�[�N���X ---------------------------------------------------------------------------
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
		_db,			// DB�R�l�N�V����
		sql.c_str(),	// SQL��
		callback,		// �R�[���o�b�N�֐�
		result,			// CB�֐��ɓn������
		&_errorMessage	// �G���[���b�Z�[�W
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

	//�e�[�u�����쐬����(create table)
	db->exec(std::string("create table student(id int, name varchar(64), age int, sex int);"));
	//�f�[�^��V�K�쐬����(insert)
	db->exec(std::string("insert into student(id, name, age, sex) values(1, 'aaaaa', 10, 0);"));
	db->exec(std::string("insert into student(id, name, age, sex) values(2, 'bbbbb', 21, 1);"));
	db->exec(std::string("insert into student(id, name, age, sex) values(3, 'ccccc', 32, 2);"));

	db->exec(std::string("select * from student;"));

	//�f�[�^���X�V����B
	db->exec(std::string("update student set name='ddddd' where id=1;"));

	db->exec(std::string("select * from student;"));

	//�f�[�^���폜����B
	db->exec(std::string("delete from student where id = 1;"));
	db->exec(std::string("delete from student where id = 2;"));
	db->exec(std::string("delete from student where id = 3;"));

	//�e�[�u���폜�B
	db->exec(std::string("drop table student;"));

	//�e�[�u���ꗗ�B
	db->exec(std::string("select name from sqlite_master where type='table';"));

	//�ڑ����̃f�[�^�x�[�X���m�F����
	Log("%s", db->getDbName());

	//close
	delete db;

	return 0;
}
