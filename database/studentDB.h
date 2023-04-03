#include "sqlite3.h"
#include <QString>
#include <QByteArray>
#include <QList>
#include <QPixmap>

struct Student {
    int id;
    QString name;
    QString email;
    QString cardId;
    QByteArray fpId;
};

class StudentDB{
    
    public:
        static bool initDB();
        static bool insertStudent(QString name, QString email, QString cardId, QByteArray fpId);
        static bool deleteStudent(QString id);
        static Student getStudent(QString id);
        static QList<Student> getAllStudents();
        static int callback(void* NotUsed, int argc, char** argv, char** azColName);

};