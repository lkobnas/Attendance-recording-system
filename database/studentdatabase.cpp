#include "studentdatabase.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QBuffer>

bool StudentDatabase::insertStudent(QString name, QString id, QString cardId, QString picturePath, QString fingerprint)
{
    if (!QFile::exists(picturePath)) {
        return false;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("students.db");

    if (!db.open()) {
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO students (name, id, card_id, picture, fingerprint) VALUES (:name, :id, :card_id, :picture, :fingerprint)");

    QFile pictureFile(picturePath);
    if (!pictureFile.open(QIODevice::ReadOnly)) {
        db.close();
        return false;
    }

    QByteArray pictureData = pictureFile.readAll();
    pictureFile.close();

    QPixmap picture;
    picture.loadFromData(pictureData);
    QByteArray pictureBytes;
    QBuffer buffer(&pictureBytes);
    buffer.open(QIODevice::WriteOnly);
    picture.save(&buffer, "PNG");

    query.bindValue(":name", name);
    query.bindValue(":id", id);
    query.bindValue(":card_id", cardId);
    query.bindValue(":picture", pictureBytes);
    query.bindValue(":fingerprint", fingerprint);

    if (!query.exec()) {
        db.close();
        return false;
    }

    db.close();
    return true;
}

Student StudentDatabase::getStudent(QString id)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("students.db");

    if (!db.open()) {
        return Student{"", "", "", QPixmap(), ""};
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM students WHERE id=:id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        db.close();
        return Student{"", "", "", QPixmap(), ""};
    }

    if (!query.next()) {
        db.close();
        return Student{"", "", "", QPixmap(), ""};
    }

    QString name = query.value("name").toString();
    QString cardId = query.value("card_id").toString();
    QByteArray pictureBytes = query.value("picture").toByteArray();
    QPixmap picture;
    picture.loadFromData(pictureBytes);
    QString fingerprint = query.value("fingerprint").toString();

    db.close();

    return Student{name, id, cardId, picture, fingerprint};
}

bool StudentDatabase::open()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("students.db");

    if (!db.open()) {
        return false;
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS students ("
               "name TEXT, "
               "id TEXT PRIMARY KEY, "
               "card_id TEXT, "
               "picture BLOB, "
               "fingerprint TEXT)");

    return true;
}

void StudentDatabase::close()
{
    QSqlDatabase::removeDatabase("QSQLITE");
}
