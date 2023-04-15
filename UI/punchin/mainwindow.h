#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

#include <sqlite3.h>

#include <QtCore>
#include <QtGui>
#include <QException>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>
#include <QTableView>
#include <QModelIndexList>
#include <QModelIndex>
#include <QItemSelection>

#include <thread>
#include <functional>

#include "addstudentwindow.h"
#include "addcoursewindow.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "../../PN532/rpi_get_uid.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    bool adminMode = true;              ///TEST MODE
    QStandardItemModel* model;
    QStandardItemModel* sModel;
    CourseDB cdb;
    StudentDB sdb;
    QList<Course> courseList;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateTableView();
    void updateStudentTable();

private slots:
    void init();
    
    void update();

    void updateDatetimeDisplay();

    void checkCourseStart();

    void on_addNewStudentButton_clicked();

    void on_addNewCourseButton_clicked();

    void on_actionAdministrator_mode_triggered();

    void on_actionSwitch_to_student_mode_triggered();

    void on_testButton_clicked();

    void on_studentListButton_clicked();

    void on_deleteCourseButton_clicked();

    void onUIDReceived(const QString uid);

    void onAddStudentWindowClosed();

signals:
    void passCardID(QString cardID);

private:
    Ui::MainWindow *ui; 
    AddStudentWindow* sWindow; 
    bool studentWindowValid;
    RFID rfid;
    std::thread rfidThread;
    QTimer* email_timer;
    QTimer* delay_timer;
    bool e_functionRunning;
    bool d_functionRunning;
    void e_resetFunctionRunningFlag();
    void d_resetFunctionRunningFlag();

    void rfidListener();
    void recordAttendanceWindow(QString studentID);
    //std::function<void(const QString&)> rfid_callback_;
};
#endif // MAINWINDOW_H
