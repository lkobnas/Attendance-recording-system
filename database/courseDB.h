#ifndef COURSEDB_H
#define COURSEDB_H

#include "sqlite3.h"
#include <QString>
#include <QByteArray>
#include <QList>
#include <QPixmap>

#include "studentDB.h"

struct Course {
    QString name;
    QString datetime;
    QList<Student> studentList;

};

class CourseDB {
    
    public:
        static bool initDB();
        static bool insertCourse(QString name, QString datetime, QString SIDString);
        static bool deleteCourse(QString name);
        static Course getCourse(QString name);
        static QList<Course> getAllCourses();
        static int callback(void* NotUsed, int argc, char** argv, char** azColName);

    private:
        static QString listToString(QList<Student> studentList);
        static QList<Student> stringtoList(QString s);
        static bool updateStudentList(sqlite3* DB, char* messageError, QString courseName, QString studentList);
};

#endif // COURSEDB_H