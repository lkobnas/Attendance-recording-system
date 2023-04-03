#include "sqlite3.h"
#include <QString>
#include <QByteArray>
#include <QList>
#include <QPixmap>

struct Student {
    QString id;
    QString sid;
    QString name;
    QString email;
    QString cardId;
    QByteArray picture;
    QByteArray fpId;
};

class StudentDB{
    
    public:
        static bool initDB();
        static bool insertStudent(QString sid, QString name, QString email, QString cardId, QByteArray picture, QByteArray fpId);
        static bool deleteStudent(QString sid);
        static Student getStudent(QString sid);
        static QList<Student> getAllStudents();
        static int callback(void* NotUsed, int argc, char** argv, char** azColName);

};