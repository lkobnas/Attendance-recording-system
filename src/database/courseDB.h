#ifndef COURSEDB_H
#define COURSEDB_H

#include "sqlite3.h"
#include "myException.h"
#include <QString>
#include <QList>
#include <QPixmap>

#include "studentDB.h"
#include "../email_curl/email.h"

struct Course {
    QString name;
    QString datetime;
    QList<Student> studentList;
    QList<Student> arrivedStudents;

};

class CourseDB {
    
    public:
        static bool initDB();
        static bool insertCourse(QString name, QString datetime, QString SIDString);
        static bool updateArrived(QString name, QString SIDString);
        static bool deleteCourse(QString name);
        static Course getCourse(QString name);
        static QList<Course> getAllCourses();
        static int callback(void* NotUsed, int argc, char** argv, char** azColName);

    private:
        static QString listToString(QList<Student> studentList);
        static QList<Student> stringtoList(QString s);
        static bool updateStudentList(sqlite3* DB, char* messageError, QString courseName, QString datetime, QString studentList);
};

#endif // COURSEDB_H