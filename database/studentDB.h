#ifndef STUDENTDB_H
#define STUDENTDB_H

#include "sqlite3.h"
#include "myException.h"
#include <QString>
#include <QList>
#include <QPixmap>

struct Student
{
    /* data */
    QString sid;
    QString name;
    QString email;
    QString cardId;
    QString picture;
    QString fpId;
};


class StudentDB {
    
    public:
        static bool initDB();
        static bool insertStudent(QString sid, QString name, QString email, QString cardId, QString picture, QString fpId);
        static bool deleteStudent(QString sid);
        static bool checkStudentExist(QString sid);
        static Student getStudent(QString sid);
        static Student getStudentByCardID(QString cardID);
        static Student getStudentByFPID(QString fpID);
        static QList<Student> getAllStudents();
        static int callback(void* NotUsed, int argc, char** argv, char** azColName);
        

};

#endif // STUDENTDB_H