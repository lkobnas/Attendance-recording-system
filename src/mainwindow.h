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
#include "PN532/rfid.h"
#include "doorlock/doorlock.h"
#include "AS608/fingerprint.h"          

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/// @brief A class to wrap the QT generated header file and handle GUI functionality. Inherits from QMainWindow
class MainWindow : public QMainWindow
{
    Q_OBJECT
    bool adminMode = false;             
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

    void onFPIDIdentifyReceived(const QString fpid);

    void onFPIDAddReceived(QString fpid);

    void onAddStudentWindowClosed();

    void on_actionClose_triggered();

signals:
    void passCardID(QString cardID);
    void passFpID(QString fpID);

private:
    Ui::MainWindow *ui; 
    AddStudentWindow* sWindow; 
    bool studentWindowValid;

    RFID rfid;
    Fingerprint fp;
    Doorlock doorlock;
    std::thread rfidThread;
    std::thread fingerprintIdentifyThread;
    std::thread doorlockThread;
    int fpMode;
    bool running;
    bool door;


    QTimer* email_timer;
    QTimer* delay_timer;
    bool e_functionRunning;
    bool d_functionRunning;
    void e_resetFunctionRunningFlag();
    void d_resetFunctionRunningFlag();

    void rfidListener();
    void fingerprintIdentifyListener();
    void recordAttendanceWindow(QString studentID);
    void doorControl();

};
#endif // MAINWINDOW_H
