#ifndef STUDENTDATABASE_H
#define STUDENTDATABASE_H

#include <QString>
#include <QByteArray>
#include <QList>
#include <QPixmap>

struct Student {
    QString name;
    QString studentId;
    QString cardId;
    QByteArray pictureData;
    QByteArray fingerprintData;
};

class StudentDatabase
{
public:
    static bool insertStudent(QString name, QString studentId, QString cardId, QPixmap picture, QByteArray fingerprint);
    static bool updateStudent(QString studentId, QString cardId, QPixmap picture, QByteArray fingerprint);
    static bool deleteStudent(QString studentId);
    static Student getStudent(QString studentId);
    static QList<Student> getStudents();
    static bool open();
    static void close();
};

#endif // STUDENTDATABASE_H
