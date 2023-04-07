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
    QString datetime = ui->dateTimeEdit->text();
    if (datetime.isEmpty()) {
        QMessageBox::warning(this, "Error adding course", "Course date/time is invalid");
        return;
    } 
    QString sid = ui->cStudentIDListLineEdit->text();
    if (sid.isEmpty()) {
        QMessageBox::information(this, "Error adding student", "Student ID cannot be empty.");
        return;
    } 
    
    // Insert new student data to database
    // try{
    //     sdb.insertStudent(sid,name,email,cardID,photo,fdID);
    // }catch(QException &e){
    //     const MyException* myException = dynamic_cast<const MyException*>(&e);
    //     if (myException) {
    //         QString errorMessage = myException->message();
    //         QMessageBox::warning(this, "Error adding student", errorMessage);
    //         return;
    //     }
    // }
    QMessageBox::information(this, "DateTime", datetime);


return;
}

void AddCourseWindow::on_cClearButton_clicked()
{
    
}


void AddCourseWindow::on_closeButton_clicked()
{
    this->close();
}

