#include "addstudentwindow.h"
#include "ui_addstudentwindow.h"

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QException>


AddStudentWindow::AddStudentWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddStudentWindow)
{
    sdb.initDB();
    ui->setupUi(this);
    setWindowTitle("Add New Student");
}

AddStudentWindow::~AddStudentWindow()
{
    delete ui;
}

void AddStudentWindow::on_sClearButton_clicked()
{
    ui->sNameLineEdit->clear();
    ui->sEmailLineEdit->clear();
    ui->sSIDLineEdit->clear();
}


void AddStudentWindow::on_sCardButton_clicked()
{

}


void AddStudentWindow::on_sFpButton_clicked()
{

}


void AddStudentWindow::on_sPhotoButton_clicked()
{

}


void AddStudentWindow::on_sAddButton_clicked()
{

    QString name = ui->sNameLineEdit->text();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Error adding student", "Student name cannot be empty.");
        return;
    } 
    QString email = ui->sEmailLineEdit->text();
    if (email.isEmpty()) {
        QMessageBox::warning(this, "Error adding student", "Student email cannot be empty.");
        return;
    } 
    QString sid = ui->sSIDLineEdit->text();
    if (sid.isEmpty()) {
        QMessageBox::warning(this, "Error adding student", "Student ID cannot be empty.");
        return;
    } 
    // TODO Check rfid card
    QString cardID = "cardIDHere";
    QByteArray photo = "photoDataHere";
    QByteArray fdID = "fingerprintDataHere";
    // Insert new student data to database
    try{
        sdb.insertStudent(sid,name,email,cardID,photo,fdID);
    }catch(QException &e){
        const MyException* myException = dynamic_cast<const MyException*>(&e);
        if (myException) {
            QString errorMessage = myException->message();
            QMessageBox::warning(this, "Error adding student", errorMessage);
            return;
        }
    }
    QMessageBox::information(this, "Insert successful", "New student " +name+ " added to database");


return;
}


void AddStudentWindow::on_pushButton_clicked()
{
    this->close();
}

