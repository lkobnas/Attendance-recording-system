#include "coursedatabase.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStringList>

bool CourseDatabase::insertCourse(QString name, QString dateTime, QStringList studentIds)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("courses.db");

    if (!db.open()) {
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO courses (name, date_time, student_ids) VALUES (:name, :date_time, :student_ids)");

    QString studentIdsString = studentIds.join(",");
    query.bindValue(":name", name);
    query.bindValue(":date_time", dateTime);
    query.bindValue(":student_ids", studentIdsString);

    if (!query.exec()) {
        db.close();
        return false;
    }

    db.close();
    return true;
}

QList<Course> CourseDatabase::getCourses()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("courses.db");

    if (!db.open()) {
        return QList<Course>();
    }

    QSqlQuery query;
    query.exec("SELECT * FROM courses");

    QList<Course> courses;

    while (query.next()) {
        QString name = query.value("name").toString();
        QString dateTime = query.value("date_time").toString();
        QStringList studentIds = query.value("student_ids").toString().split(",");
        courses.append(Course{name, dateTime, studentIds});
    }

    db.close();

    return courses;
}

bool CourseDatabase::open()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("courses.db");

    if (!db.open()) {
        return false;
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS courses ("
               "name TEXT, "
               "date_time TEXT, "
               "student_ids TEXT)");

    return true;
}

void CourseDatabase::close()
{
    QSqlDatabase::removeDatabase("QSQLITE");
}
