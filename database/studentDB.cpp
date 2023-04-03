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
		"FPID      BLOB NOT NULL );";

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
		cerr << "Error in insertStudent function." << endl;
		sqlite3_free(messageError);
		return false;
	}
	else
		cout << "Records inserted Successfully!" << endl;

	return true;
}

bool StudentDB::deleteStudent(QString id)
{
	sqlite3* DB;
	char* messageError;
	string sql = "DELETE FROM STUDENTS WHERE ID = "+id.toStdString()+";";
	//string end = ";";
	//sql = sql + end;

	int exit = sqlite3_open("students.db", &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	exit = sqlite3_exec(DB, sql.c_str(), callback, NULL, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in deleteStudent function." << endl;
		sqlite3_free(messageError);
	}
	else
		cout << "Records deleted Successfully!" << endl;

	return true;
}

Student StudentDB::getStudent(QString id)
{
	sqlite3* DB;
	sqlite3_open("students.db", &DB);
    sqlite3_stmt* stmt;
    char* errMsg = nullptr;
    std::string sql = "SELECT * FROM students WHERE id = ?;";

    int result = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(DB) << std::endl;
        return Student();
    }

    result = sqlite3_bind_int(stmt, 1, id.toInt());
    if (result != SQLITE_OK) {
        std::cerr << "Error binding parameter: " << sqlite3_errmsg(DB) << std::endl;
        sqlite3_finalize(stmt);
        return Student();
    }

    result = sqlite3_step(stmt);
    if (result != SQLITE_ROW) {
        std::cerr << "Error retrieving student: " << sqlite3_errmsg(DB) << std::endl;
        sqlite3_finalize(stmt);
        return Student();
    }

    Student student;
    student.id = sqlite3_column_int(stmt, 0);
    student.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    student.email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    student.cardId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    student.fpId = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 4));
	//QByteArray arr= QByteArray((const char *)dataBlob->pbData, dataBlob->cbData);
    sqlite3_finalize(stmt);

    return student;
}

QList<Student> StudentDB::getAllStudents() {
    QList<Student> students;
    sqlite3_stmt* stmt;
	sqlite3* DB;
	sqlite3_open("students.db", &DB);
    std::string sql = "SELECT * FROM STUDENTS";
    int rc = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        // handle error
        return students;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Student s;
        s.id = sqlite3_column_int(stmt, 0);
        s.name = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        s.email = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        s.cardId = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        s.fpId = QByteArray(reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 4)));
                            //sqlite3_column_bytes(stmt, 4));

        students.append(s);
    }

    sqlite3_finalize(stmt);
    return students;
}

int StudentDB::callback(void* NotUsed, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++) {
		// column name and value
		cout << azColName[i] << ": " << argv[i] << endl;
	}

	cout << endl;

	return 0;
}
