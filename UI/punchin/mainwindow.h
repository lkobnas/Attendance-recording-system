#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

#include "addstudentwindow.h"
#include "addcoursewindow.h"
#include "../../camera/camera.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    bool adminMode = true;              ///TEST MODE
    QStandardItemModel* model;
    CourseDB cdb;
    StudentDB sdb;
    QList<Course> courseList;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateTableView();

private slots:
    void init();
    
    void update();

    void updateDatetimeDisplay();

    void on_addNewStudentButton_clicked();

    void on_addNewCourseButton_clicked();

    void on_actionAdministrator_mode_triggered();

    void on_actionSwitch_to_student_mode_triggered();

    void on_testButton_clicked();

    void on_studentListButton_clicked();

    void on_deleteCourseButton_clicked();

    void onCameraNewFrame(const QImage& image);

private:
    Ui::MainWindow *ui;
    Camera* camera;
};
#endif // MAINWINDOW_H
