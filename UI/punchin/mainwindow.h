#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../../../database/studentDB.h"
#include "../../../database/courseDB.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    StudentDB sdb;
    CourseDB cdb;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_sAddButton_clicked();

    void on_sClearButton_clicked();

    void on_sCardButton_clicked();

    void on_sFpButton_clicked();

    void on_sPhotoButton_clicked();

    void on_cClearButton_clicked();

    void on_cAddButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
