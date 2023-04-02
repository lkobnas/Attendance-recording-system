#include "studentDB.h"
#include "iostream"
//#include <QSqlDatabase>
using namespace std;

bool StudentDB::initDB()
{
    sqlite3* DB;
    char* messageError;
	string sql = "CREATE TABLE IF NOT EXISTS STUDENTS("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"NAME      TEXT NOT NULL, "
		"EMAIL     TEXT NOT NULL, "
		"CARDID    TEXT NOT NULL, "
		"FPID      TEXT NOT NULL );";

    try
	{
        int exit = 0;
        exit = sqlite3_open("students.db", &DB);
		/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
		exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
		if (exit != SQLITE_OK) {
			cerr << "Error in createTable function." << endl;
			sqlite3_free(messageError);
		}
		else
			cout << "Table created Successfully" << endl;
		sqlite3_close(DB);
	}
	catch (const exception& e)
	{
		cerr << e.what();
        return false;
	}

	return true;
}

bool StudentDB::insertStudent(QString name, QString email, QString cardId, QByteArray fpId)
{
	sqlite3* DB;
	char* messageError;
		
	string sql = "INSERT INTO STUDENTS (NAME, EMAIL, CARDID, FPID) VALUES('"+name.toStdString()+"', '"+email.toStdString()+"', '"+cardId.toStdString()+"', '"+fpId.toStdString()+"');";


	int exit = sqlite3_open("students.db", &DB);
    
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in insertData function." << endl;
		sqlite3_free(messageError);
		return false;
	}
	else
		cout << "Records inserted Successfully!" << endl;

	return true;
}

bool StudentDB::deleteStudent(int id)
{
	sqlite3* DB;
	char* messageError;
	string sql = "DELETE FROM STUDENTS WHERE ID = "+id;
	string end = ";";
	sql = sql + end;

	int exit = sqlite3_open("students.db", &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	exit = sqlite3_exec(DB, sql.c_str(), callback, NULL, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in deleteData function." << endl;
		sqlite3_free(messageError);
	}
	else
		cout << "Records deleted Successfully!" << endl;

	return true;
}
