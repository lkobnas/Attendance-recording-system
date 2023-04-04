#include "student.h"
class Student {
public:
    // Constructor
    Student() {}

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
    QString name;
    QString studentID;
    QString cardID;
    QByteArray picture;
    QByteArray fpID;
};