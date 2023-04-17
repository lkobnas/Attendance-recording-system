#ifndef MYEXCEPTION_H
#define MYEXCEPTION_H

#include <QException>
#include <QString>

class MyException : public QException {
public:
    MyException(QString message) : m_message(message) {}

    const QString& message() const {
        return m_message;
    }

private:
    QString m_message;
};

#endif