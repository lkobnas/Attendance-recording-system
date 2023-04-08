#ifndef ADDSTUDENTWINDOW_H
#define ADDSTUDENTWINDOW_H

#include "../../database/studentDB.h"
#include <QDialog>

namespace Ui {
class AddStudentWindow;
}

class AddStudentWindow : public QDialog
{
    Q_OBJECT
    StudentDB sdb;

public:
    explicit AddStudentWindow(QWidget *parent = nullptr);
    ~AddStudentWindow();

private slots:
    void on_sClearButton_clicked();

    void on_sCardButton_clicked();

    void on_sFpButton_clicked();

    void on_sPhotoButton_clicked();

    void on_sAddButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::AddStudentWindow *ui;
};

#endif // ADDSTUDENTWINDOW_H
