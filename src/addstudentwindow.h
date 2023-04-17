#ifndef ADDSTUDENTWINDOW_H
#define ADDSTUDENTWINDOW_H

#include "database/studentDB.h"
#include "email_curl/email.h"
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QException>
#include <QDialog>
#include <QDebug>

namespace Ui {
class AddStudentWindow;
}

/// @brief A class for add student window. Inherits from QDialog
class AddStudentWindow : public QDialog
{
    Q_OBJECT
    StudentDB sdb;
    Email email_curl;

public:
    explicit AddStudentWindow(QWidget *parent = nullptr);
    ~AddStudentWindow();

public slots:
    void receiveCardID(QString cardid);
    void receiveFPID(QString fpid);

private slots:
    void on_sClearButton_clicked();

    void on_sCardButton_clicked();

    void on_sFpButton_clicked();

    void on_sAddButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::AddStudentWindow *ui;

    QString name;
    QString email;
    QString sid;
    QString cardID;
    QString photo;
    QString fpID;
};

#endif // ADDSTUDENTWINDOW_H
