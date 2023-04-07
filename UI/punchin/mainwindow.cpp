#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

#include <QtCore>
#include <QtGui>
#include <QException>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    sdb.initDB();
    cdb.initDB();
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_sAddButton_clicked()
{
    
    QString name = ui->sNameLineEdit->text();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Error adding student", "Name cannot be empty.");
        return;
    } 
    QString email = ui->sEmailLineEdit->text();
    if (email.isEmpty()) {
        QMessageBox::warning(this, "Error adding student", "Email cannot be empty.");
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
        QMessageBox::warning(this, "Error adding student", e.what());
    }
    


}

void MainWindow::on_sClearButton_clicked()
{

}


void MainWindow::on_sCardButton_clicked()
{

}


void MainWindow::on_sFpButton_clicked()
{

}


void MainWindow::on_sPhotoButton_clicked()
{

}


void MainWindow::on_cClearButton_clicked()
{

}


void MainWindow::on_cAddButton_clicked()
{

}



