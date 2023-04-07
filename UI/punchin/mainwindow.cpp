#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>

#include <QtCore>
#include <QtGui>
#include <QException>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    adminMode = false;
    datetimeDisplay();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addNewStudentButton_clicked()
{
    AddStudentWindow sWindow;
    sWindow.setModal(true);
    sWindow.exec();
}


void MainWindow::on_addNewCourseButton_clicked()
{
    AddCourseWindow cWindow;
    cWindow.setModal(true);
    cWindow.exec();
}

void MainWindow::datetimeDisplay()
{
    QDateTime datetime = QDateTime::currentDateTime();
    //QTime time = QTime::currentTime();
    QString dateTimeText = datetime.toString("dd/mm/yyyy hh:mm:ss");
    QFont font = ui->mDateTimeLabel->font();
    font.setPointSize(30);
    font.setBold(true);
    ui->mDateTimeLabel->setFont(font);
    ui->mDateTimeLabel->setText(dateTimeText);
    
}

void MainWindow::on_actionAdministrator_mode_triggered()
{
    if(adminMode == false){
        QString pw = QInputDialog::getText(this,"Admin Mode","Please enter admin passcode: "); //TODO: check ok button
        if (pw.compare("5220")){
            adminMode = true;
        }else{
            QMessageBox::warning(this, "Error switching to admin mode", "Wrong passcode");
        }
    }
}


void MainWindow::on_actionSwitch_to_student_mode_triggered()
{
    if(adminMode == true){
        adminMode = false;
    }
}

