#include "courseDB.h"
#include <iostream>
using namespace std;


/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool CourseDB::initDB()
{
    sqlite3* DB;
    char* messageError;
	string sql = "CREATE TABLE IF NOT EXISTS COURSES("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"NAME       TEXT NOT NULL, "
		"DATETIME   TEXT NOT NULL, "
        "PENDING    TEXT, "
		"ARRIVED    TEXT);";
    try
	{
        int exit = 0;
        exit = sqlite3_open("courses.db", &DB);
		/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
		exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
		if (exit != SQLITE_OK) {
            sqlite3_free(messageError);
			throw MyException("Error in createTable function.");
			
		}

		sqlite3_close(DB);
	}
	catch (const exception& e)
	{
		throw MyException(e.what());
        return false;
	}
	return true;
}

bool CourseDB::insertCourse(QString name, QString datetime, QString SIDString)
{

	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open("courses.db", &DB);
    
	sqlite3_stmt* checkStmt;
    int checkResult = sqlite3_prepare_v2(DB, "SELECT * FROM COURSES WHERE NAME = ?;", -1, &checkStmt, nullptr);
    if (checkResult != SQLITE_OK) {
        checkResult == -1;     
    }
    if (checkResult != -1){
        sqlite3_bind_text(checkStmt, 1, name.toUtf8().constData(), -1, SQLITE_STATIC);
        checkResult = sqlite3_step(checkStmt);
        sqlite3_finalize(checkStmt);
    }
    
    if (checkResult == SQLITE_ROW) {
        try{
            updateStudentList(DB, messageError, name, SIDString);
        }catch (const QException& e){
            const MyException* myException = dynamic_cast<const MyException*>(&e);
            if (myException) {
                QString errorMessage = myException->message();
            }
            //throw MyException(e);
            throw;
        }
        return false; //Course not added so return false
    }

    QString s;
    bool removedNotExist = false;
    QString error_msg;
    if (!SIDString.isEmpty()){
        s = SIDString.replace(" ","");
        QList<QString> ilist = s.split(",");
        for(int i=0; i<ilist.size(); i++){
            if(StudentDB::checkStudentExist(ilist[i])==false){
                s.replace(ilist[i],"");
                removedNotExist = true;
            }
        }
        s.replace(",,",",");
        if(s.startsWith(",")){
            s.remove(0,1);
        }
        if(s.endsWith(",")){
            s.remove(s.length()-1, 1);
        }
    }else{
        s = "";
    }
    
    //cout<<"Insert: "<<s.toStdString()<<endl;
	string sql = "INSERT INTO COURSES (NAME, DATETIME, PENDING) VALUES('"+name.toStdString()+"', '"+datetime.toStdString()+"', '"+s.toStdString()+"');";
    
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
        sqlite3_free(messageError);
		throw MyException("Error in insertCoursefunction.");
	}
    if (removedNotExist){
        throw MyException("Non-registered student have been removed");
    }

	return true;
}
QString CourseDB::listToString(QList<Student> studentList){

    QString s;
    if(studentList.isEmpty()){
        s.append("");
        return s;
    }
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
        if(!StudentDB::checkStudentExist(sidList[i])){
            continue;
        }
        Student student = StudentDB::getStudent(sidList[i]);
            studentList.append(student);
    }
    return studentList;
}

bool CourseDB::updateStudentList(sqlite3* DB, char* messageError, QString courseName, QString inputList){
    inputList.replace(" ","");
    QList<QString> ilist = inputList.split(",");
    
    Course course = getCourse(courseName);
    QList<Student> sqlist = course.studentList;
    QList<Student> aqlist = course.arrivedStudents;
    QString slist = listToString(sqlist);
    QString alist = listToString(aqlist);

    bool change = false;
 
    for (auto it = ilist.begin(); it != ilist.end(); ) {
        if (alist.contains(*it)) {
            it = ilist.erase(it);
        } else {
            ++it;
        }
    }

    if(ilist.isEmpty()){
        throw MyException("Nothing to update in Student list");
    }

    for(int i=0;i<ilist.size();i++){
        if(slist.contains(ilist[i]) || !StudentDB::checkStudentExist(ilist[i])){  
            continue;
        }else{         
            slist.append(",");
            slist.append(ilist[i]);
            change = true;
        }
    }


    slist.replace(",,",",");
    if(slist.startsWith(",")){
        slist.remove(0,1);
    }
    if(slist.endsWith(",")){
        slist.remove(slist.length()-1, 1);
    }
    
    if(change == false){
        throw MyException("Nothing to update in Student list");
    }else{
        string sql = "UPDATE COURSES SET PENDING = '"+slist.toStdString()+"' WHERE NAME = '"+courseName.toStdString()+"';"; 
        int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
        if (exit != SQLITE_OK) {
            sqlite3_free(messageError);
            throw MyException("Error in insertCoursefunction.");            
        }
    }

    return true;
}

bool CourseDB::updateArrived(QString courseName, QString targetSID){
    targetSID.replace(" ","");
    
    Course course = getCourse(courseName);
    QList<Student> sqlist = course.studentList;
    QList<Student> aqlist = course.arrivedStudents;
    QString slist = listToString(sqlist);
    QString alist = listToString(aqlist);

    if(!slist.contains(targetSID)){
        QString s = targetSID + " is not registered in this class or is already here";
        throw MyException(s);
    }
      
    if(!alist.isEmpty()){
        if(alist.contains(targetSID)){
            QString s = targetSID + " is already here";
            throw MyException(s);
        }
        alist.append(",");
        alist.append(targetSID);
    }else{
        alist.append(targetSID);
    }
    
       
    slist.replace(targetSID,"");
    slist.replace(",,",",");
    if(slist.startsWith(",")){
        slist.remove(0,1);
    }
    if(slist.endsWith(",")){
        slist.remove(slist.length()-1, 1);
    }
    sqlite3* DB;
    char* messageError;
	int exit = sqlite3_open("courses.db", &DB);
    
    string sql = "UPDATE COURSES SET PENDING = '"+slist.toStdString()+"' WHERE NAME = '"+courseName.toStdString()+"';"; 
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        sqlite3_free(messageError);
        throw MyException("Error in updating slist in updateArrived");
    }
    
    sql = "UPDATE COURSES SET ARRIVED = '"+alist.toStdString()+"' WHERE NAME = '"+courseName.toStdString()+"';"; 
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        sqlite3_free(messageError);
        throw MyException("Error in updating alist in updateArrived");
    }

    return true;
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


    if (sqlite3_step(stmt) != SQLITE_DONE) {
       throw MyException(sqlite3_errmsg(DB));
    }
    sqlite3_finalize(stmt);

	return true;
}

Course CourseDB::getCourse(QString name){

    QString sql = "SELECT * FROM COURSES WHERE NAME = ?;";
    sqlite3_stmt* stmt;
	sqlite3* DB;
	int exit = sqlite3_open("courses.db", &DB);
    int rc = sqlite3_prepare_v2(DB, sql.toUtf8().constData(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        throw MyException(sqlite3_errmsg(DB));
    }
    rc = sqlite3_bind_text(stmt, 1, name.toUtf8().constData(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        throw MyException(sqlite3_errmsg(DB));
        
    }
    rc = sqlite3_step(stmt);

    Course course;
    //course.id = sqlite3_column_int(stmt, 0);
    course.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    course.datetime = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    QString s;
    const unsigned char* slist = sqlite3_column_text(stmt, 3);
    QString sqlist = QString::fromUtf8((const char*)slist);
    //cout<<"sqlist from get: "<<sqlist.toStdString()<<endl;
    course.studentList = stringtoList(sqlist);

    //s = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    //course.studentList = stringtoList(s);
    s = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
    course.arrivedStudents = stringtoList(s);
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
        QString slist = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        course.studentList = stringtoList(slist);
        QString alist = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        course.arrivedStudents = stringtoList(alist);
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
