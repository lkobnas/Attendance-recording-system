#include "studentDB.h"
#include <iostream>
#include <QSqlDatabase>
using namespace std;

/// @brief Initialise the list of students in the database
/// @return Execute successfully, return true
bool StudentDB::initDB()
{
    sqlite3* DB;
    char* messageError;
	string sql = "CREATE TABLE IF NOT EXISTS STUDENTS("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"SID       TEXT NOT NULL, "
		"NAME      TEXT NOT NULL, "
		"EMAIL     TEXT NOT NULL, "
		"CARDID    TEXT NOT NULL, "
		"PHOTO     BLOB, "
		"FPID      BLOB);";
    try
	{
        int exit = 0;
        exit = sqlite3_open("students.db", &DB);
		/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
		exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
		if (exit != SQLITE_OK) {
			sqlite3_free(messageError);
            throw MyException("Error in createTable function");
		}

		sqlite3_close(DB);
	}
	catch (const exception& e)
	{
		throw MyException(e.what());
	}
	return true;
}

/// @brief The database insert inserts a new student information consisting of six parameters: sid (student ID), name, email, cardId (student card ID), picture (number of bytes for the student's photo) and fpId (number of bytes for the student's fingerprint ID).
/// @param sid student ID
/// @param name student name
/// @param email student email
/// @param cardId student card ID
/// @param picture a byte array of the student's photo
/// @param fpId a byte array of the student's fingerprint ID
/// @return The function returns true if the student was successfully inserted into the database
bool StudentDB::insertStudent(QString sid, QString name, QString email, QString cardId, QString picture, QString fpId)
{

	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open("students.db", &DB);
    
	sqlite3_stmt* checkStmt;
    int checkResult = sqlite3_prepare_v2(DB, "SELECT * FROM students WHERE SID = ?;", -1, &checkStmt, nullptr);
    if (checkResult != SQLITE_OK) {
        checkResult == -1;     
    }
    if (checkResult != -1){
        sqlite3_bind_text(checkStmt, 1, sid.toUtf8().constData(), -1, SQLITE_STATIC);
        checkResult = sqlite3_step(checkStmt);
        sqlite3_finalize(checkStmt);
    }
    
    if (checkResult == SQLITE_ROW) {
        QString s = "Student with ID " + sid + " already exists in database";
        throw MyException(s);
    }

	string sql = "INSERT INTO STUDENTS (SID, NAME, EMAIL, CARDID, PHOTO, FPID) VALUES('"+sid.toStdString()+"', '"+name.toStdString()+"', '"+email.toStdString()+"', '"+cardId.toStdString()+"', '"+picture.toStdString()+"', '"+fpId.toStdString()+"');";
    
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		sqlite3_free(messageError);
        throw MyException("Error in insertStudent function");
	}
	
    return true;
}
/// @brief Remove a student from the STUDENTS table in the SQLite database
/// @param sid student ID
/// @return true
bool StudentDB::deleteStudent(QString sid)
{
    if(!checkStudentExist(sid)){
        return false;
    }
	sqlite3* DB;
	char* messageError;
	//QString sql = "DELETE FROM STUDENTS WHERE SID = ?;";
	sqlite3_open("students.db", &DB);
    sqlite3_stmt* stmt;
    const char* query = "DELETE FROM STUDENTS WHERE SID = ?;";
    sqlite3_prepare_v2(DB, query, -1, &stmt, nullptr);

    // Bind the studentID parameter to the statement
    sqlite3_bind_text(stmt, 1, sid.toUtf8().constData(), -1, SQLITE_TRANSIENT);

    // Execute the statement
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        throw MyException(sqlite3_errmsg(DB));
    }

    // Clean up the statement
    sqlite3_finalize(stmt);

	return true;
}
/// @brief check if a student with a given sid exists in the database
/// @param sid student ID
/// @return true
bool StudentDB::checkStudentExist(QString sid)
{
    QString sql = "SELECT * FROM STUDENTS WHERE SID = ?;";
    sqlite3_stmt* stmt;
	sqlite3* DB;
	int exit = sqlite3_open("students.db", &DB);
    int rc = sqlite3_prepare_v2(DB, sql.toUtf8().constData(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        throw MyException(sqlite3_errmsg(DB));
    }
    rc = sqlite3_bind_text(stmt, 1, sid.toUtf8().constData(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        throw MyException(sqlite3_errmsg(DB));
    }
    rc = sqlite3_step(stmt);

    Student student;
    //student.id = sqlite3_column_int(stmt, 0);
	student.sid = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    sqlite3_finalize(stmt);
    if(student.sid.isEmpty()){
        return false;
    }
return true;
}

/// @brief takes a QString sid as input and returns a Student object
/// @param sid student ID
/// @return statement is finalized using sqlite3_finalize, and the Student object is returned
Student StudentDB::getStudent(QString sid){

    QString sql = "SELECT * FROM STUDENTS WHERE SID = ?;";
    sqlite3_stmt* stmt;
	sqlite3* DB;
	int exit = sqlite3_open("students.db", &DB);
    int rc = sqlite3_prepare_v2(DB, sql.toUtf8().constData(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        throw MyException(sqlite3_errmsg(DB));
    }
    rc = sqlite3_bind_text(stmt, 1, sid.toUtf8().constData(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        throw MyException(sqlite3_errmsg(DB));
    }
    rc = sqlite3_step(stmt);

    Student student;
    //student.id = sqlite3_column_int(stmt, 0);
	student.sid = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    student.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    student.email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    student.cardId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
	student.picture = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 5));
    student.fpId = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 6));
	//QByteArray arr= QByteArray((const char *)dataBlob->pbData, dataBlob->cbData);
    sqlite3_finalize(stmt);

    return student;
}

/// @brief takes a cardID string parameter and returns a Student object
/// @param cardID student card ID
/// @return returns the Student object with the retrieved data
Student StudentDB::getStudentByCardID(QString cardID){

    QString sql = "SELECT * FROM STUDENTS WHERE CARDID = ?;";
    sqlite3_stmt* stmt;
	sqlite3* DB;
	int exit = sqlite3_open("students.db", &DB);
    int rc = sqlite3_prepare_v2(DB, sql.toUtf8().constData(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        throw MyException(sqlite3_errmsg(DB));
    }
    rc = sqlite3_bind_text(stmt, 1, cardID.toUtf8().constData(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        throw MyException(sqlite3_errmsg(DB));
    }
    rc = sqlite3_step(stmt);

    Student student;
    //student.id = sqlite3_column_int(stmt, 0);
	student.sid = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    student.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    student.email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    student.cardId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
	student.picture = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 5));
    student.fpId = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 6));
	//QByteArray arr= QByteArray((const char *)dataBlob->pbData, dataBlob->cbData);
    sqlite3_finalize(stmt);

    return student;
}

/// @brief  retrieves a student's information from the STUDENTS table in an SQLite database based on their fingerprint ID
/// @param fpID a byte array of the student's fingerprint ID
/// @return returns the Student object with the retrieved data
Student StudentDB::getStudentByFPID(QString fpID){

    QString sql = "SELECT * FROM STUDENTS WHERE FPID = ?;";
    sqlite3_stmt* stmt;
	sqlite3* DB;
	int exit = sqlite3_open("students.db", &DB);
    int rc = sqlite3_prepare_v2(DB, sql.toUtf8().constData(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        throw MyException(sqlite3_errmsg(DB));
    }
    rc = sqlite3_bind_text(stmt, 1, fpID.toUtf8().constData(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        throw MyException(sqlite3_errmsg(DB));
    }
    rc = sqlite3_step(stmt);

    Student student;
    //student.id = sqlite3_column_int(stmt, 0);
	student.sid = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    student.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    student.email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    student.cardId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
	student.picture = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 5));
    student.fpId = reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 6));
	//QByteArray arr= QByteArray((const char *)dataBlob->pbData, dataBlob->cbData);
    sqlite3_finalize(stmt);

    return student;
}

/// @brief Get all student records from the SQLite database students.db and return them encapsulated in a QList
/// @return returns the Student object with the retrieved data
QList<Student> StudentDB::getAllStudents() {
    QList<Student> students;
    sqlite3_stmt* stmt;
	sqlite3* DB;
	sqlite3_open("students.db", &DB);
    std::string sql = "SELECT * FROM STUDENTS";
    int rc = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        throw MyException("Failed to prepare sql in getAllStudents");
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Student s;

		s.sid = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        s.name = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        s.email = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        s.cardId = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
		s.picture = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 5)));
        s.fpId = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 6)));
                           
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
