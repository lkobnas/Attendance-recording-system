#ifndef STUDENT_H
#define STUDENT_H

#include <QString>
#include <QByteArray>

class Student {
public:
    // Getters
    QString getName() const { return name; }
    QString getStudentID() const { return studentID; }
    QString getCardID() const { return cardID; }
    QByteArray getPicture() const { return picture; }
    QByteArray getFpID() const { return fpID; }

    // Setters
    void setName(const QString& value) { name = value; }
    void setStudentID(const QString& value) { studentID = value; }
    void setCardID(const QString& value) { cardID = value; }
    void setPicture(const QByteArray& value) { picture = value; }
    void setFpID(const QByteArray& value) { fpID = value; }
private:
    QString sid;
    QString name;
    QString email;
    QString cardId;
    QByteArray picture;
    QByteArray fpId;

};

#endif // STUDENT_H