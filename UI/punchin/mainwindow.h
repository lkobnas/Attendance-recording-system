#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "addstudentwindow.h"
#include "addcoursewindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    bool adminMode = false;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void init();
    
    void update();

    void updateDatetimeDisplay();

    void on_addNewStudentButton_clicked();

    void on_addNewCourseButton_clicked();

    void on_actionAdministrator_mode_triggered();

    void on_actionSwitch_to_student_mode_triggered();

    void updateTableView();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
