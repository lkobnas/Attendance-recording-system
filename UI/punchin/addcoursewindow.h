#ifndef ADDCOURSEWINDOW_H
#define ADDCOURSEWINDOW_H

#include "../../database/courseDB.h"
#include <QDialog>

namespace Ui {
class AddCourseWindow;
}

class AddCourseWindow : public QDialog
{
    Q_OBJECT
    CourseDB cdb;

public:
    explicit AddCourseWindow(QWidget *parent = nullptr);
    ~AddCourseWindow();

private slots:

    void on_cClearButton_clicked();

    void on_cAddButton_clicked();

    void on_closeButton_clicked();

private:
    Ui::AddCourseWindow *ui;
};

#endif // ADDCOURSEWINDOW_H
