#include "courseDB.h"
#include <iostream>
//#include <QSqlDatabase>
using namespace std;

bool CourseDB::initDB()
{
    sqlite3* DB;
    char* messageError;
	string sql = "CREATE TABLE IF NOT EXISTS COURSES("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"NAME          TEXT NOT NULL, "
		"DATETIME      TEXT NOT NULL, "
		"STUDENT_LIST  TEXT);";
    try
	{
        int exit = 0;
        exit = sqlite3_open("courses.db", &DB);
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

bool CourseDB::insertCourse(QString name, QString datetime, QList<Student> studentList)
{

	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open("courses.db", &DB);
    
	sqlite3_stmt* checkStmt;
    int checkResult = sqlite3_prepare_v2(DB, "SELECT * FROM COURSES WHERE NAME = ?;", -1, &checkStmt, nullptr);
    if (checkResult != SQLITE_OK) {
        cerr << "Failed to prepare statement:" << sqlite3_errmsg(DB);
        checkResult == -1;     
    }
    if (checkResult != -1){
        sqlite3_bind_text(checkStmt, 1, name.toUtf8().constData(), -1, SQLITE_STATIC);
        checkResult = sqlite3_step(checkStmt);
        sqlite3_finalize(checkStmt);
    }
    
    if (checkResult == SQLITE_ROW) {
        cerr << "Course with ID" << name.toStdString() << "already exists in course database.";
        updateStudentList(studentList);
        return false;
    }
    // if (checkResult != SQLITE_DONE) {
    //     cerr << "Failed to execute statement:" << sqlite3_errmsg(DB);
    // }
    QString s;
    if (studentList.isEmpty()){
        s = "";
    }else{
        s = listToString(studentList);
    }
	string sql = "INSERT INTO COURSES (NAME, DATETIME, STUDENT_LIST) VALUES('"+name.toStdString()+"', '"+datetime.toStdString()+"', '"+s.toStdString()+"');";
    
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in insertCoursefunction." << endl;
		sqlite3_free(messageError);
		return false;
	}
	else
		cout << "Records inserted Successfully!" << endl;

	return true;
}
QString CourseDB::listToString(QList<Student> studentList){

    QString s;
    s.append(studentList[0].sid);
    for (int i=1; i<studentList.size(); i++){
        s.append(",");
        s.append(studentList[i].sid);
    }
    return s;
}
QList<Student> CourseDB::stringtoList(QString s){
    QList<Student> studentList;
    QList<QString> sidList;
    sidList = s.split(",");
    for(int i=0;i<sidList.size();i++){
        studentList.append(StudentDB::getStudent(sidList[i]));
    }
    return studentList;
}

bool CourseDB::updateStudentList(QList<Student> studentList){
    return false;
}

bool CourseDB::deleteCourse(QString name)
{
	sqlite3* DB;
	char* messageError;
	
	sqlite3_open("courses.db", &DB);
    // Prepare the delete statement
    sqlite3_stmt* stmt;
    const char* query = "DELETE FROM COURSES WHERE NAME = ?;";
    sqlite3_prepare_v2(DB, query, -1, &stmt, nullptr);

    // Bind the studentID parameter to the statement
    sqlite3_bind_text(stmt, 1, name.toUtf8().constData(), -1, SQLITE_TRANSIENT);

    // Execute the statement
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "Failed to delete course:" << sqlite3_errmsg(DB);
		return false;
    }else{
        cout<< "Records deleted sucessfully"<<endl;
    }

    // Clean up the statement
    sqlite3_finalize(stmt);

	// int exit = sqlite3_open("students.db", &DB);
	// /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	// exit = sqlite3_exec(DB, sql.c_str(), callback, NULL, &messageError);
	// if (exit != SQLITE_OK) {
	// 	cerr << "Error in deleteStudent function." << endl;
	// 	sqlite3_free(messageError);
	// }
	// else
	// 	cout << "Records deleted Successfully!" << endl;

	return true;
}

Course CourseDB::getCourse(QString name){

    QString sql = "SELECT * FROM COURSES WHERE NAME = ?;";
    sqlite3_stmt* stmt;
	sqlite3* DB;
	int exit = sqlite3_open("courses.db", &DB);
    int rc = sqlite3_prepare_v2(DB, sql.toUtf8().constData(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(DB) << std::endl;
        return Course();
    }
    rc = sqlite3_bind_text(stmt, 1, name.toUtf8().constData(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        std::cerr << "Error binding SQL parameter: " << sqlite3_errmsg(DB) << std::endl;
        sqlite3_finalize(stmt);
        return Course();
    }
    rc = sqlite3_step(stmt);

    Course course;
    //course.id = sqlite3_column_int(stmt, 0);
    course.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    course.datetime = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    QString s;
    s = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    course.studentList = stringtoList(s);
	//QByteArray arr= QByteArray((const char *)dataBlob->pbData, dataBlob->cbData);
    sqlite3_finalize(stmt);

    return course;
}

QList<Course> CourseDB::getAllCourses() {
    QList<Course> courses;
    sqlite3_stmt* stmt;
	sqlite3* DB;
	sqlite3_open("courses.db", &DB);
    std::string sql = "SELECT * FROM COURSES";
    int rc = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        // handle error
        return courses;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Course course;
        //s.id = sqlite3_column_int(stmt, 0);
		course.name = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        course.datetime = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        QString s;
        s = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        course.studentList = stringtoList(s);
        courses.append(course);
    }

    sqlite3_finalize(stmt);
    return courses;
}

int CourseDB::callback(void* NotUsed, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++) {
		// column name and value
		cout << azColName[i] << ": " << argv[i] << endl;
	}

	cout << endl;

	return 0;
}
