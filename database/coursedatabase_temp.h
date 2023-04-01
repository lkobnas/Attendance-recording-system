#ifndef COURSEDATABASE_H
#define COURSEDATABASE_H

#include <QString>
#include <QStringList>
#include <QList>

struct Course {
    QString name;
    QString dateTime;
    QStringList studentIds;
};

class CourseDatabase
{
public:
    static bool insertCourse(QString name, QString dateTime, QStringList studentIds);
    static QList<Course> getCourses();
    static bool open();
    static void close();
};

#endif // COURSEDATABASE_H
