#include "addcoursewindow.h"
#include "ui_addcoursewindow.h"

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QException>

AddCourseWindow::AddCourseWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddCourseWindow)
{
    cdb.initDB();
    ui->setupUi(this);
    setWindowTitle("Add New Course");
    ui->cNoteLabel->setText("Note: You can add a student into an existing class.");
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime().addMSecs(1000*60*5)); // 5 minutes
    ui->dateTimeEdit->setDisplayFormat("yyyy-MM-dd hh:mm");
}

AddCourseWindow::~AddCourseWindow()
{
    delete ui;
}

void AddCourseWindow::on_cAddButton_clicked()
{
    QString name = ui->cNameLineEdit->text();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Error adding course", "Course name cannot be empty.");
        return;
    } 
    QString datetime = ui->dateTimeEdit->text();       // TODO: Check time is valid or not > current time
    if (datetime.isEmpty()) {
        QMessageBox::warning(this, "Error adding course", "Course date/time is invalid");
        return;
    } 
    QDateTime currentTime = QDateTime::currentDateTime();
    QDateTime inputTime = QDateTime::fromString(datetime, "yyyy-MM-dd hh:mm"); 
    if (inputTime < currentTime){
        QMessageBox::warning(this, "Error adding course", "Cannot add course from the past");
        return;       
    }

    QString sidList = ui->cStudentIDListLineEdit->text();  

    //Insert new course to database
    try{
        cdb.insertCourse(name, datetime, sidList);
    }catch(QException &e){
        const MyException* myException = dynamic_cast<const MyException*>(&e);
        if (myException) {
            QString errorMessage = myException->message();
            QMessageBox::warning(this, "Error adding course", errorMessage);
            return;
        }
    }
    QMessageBox::information(this, "Success", "New course inserted to database");
    close();
return;
}

void AddCourseWindow::on_cClearButton_clicked()
{
    ui->cNameLineEdit->clear();
    ui->cStudentIDListLineEdit->clear();
}


void AddCourseWindow::on_closeButton_clicked()
{
    this->close();
}

