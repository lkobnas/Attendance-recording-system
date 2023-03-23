#include "mywindow.h"

MyWindow::MyWindow(){

button1 = new QPushButton("Attendance Recording Mode");
button2 = new QPushButton("Register New Student");
button3 = new QPushButton("Create New Course");

connect(button1,&QPushButton::clicked,this,&MyWindow::record);
connect(button2,&QPushButton::clicked,this,&MyWindow::newStudent);
connect(button3,&QPushButton::clicked,this,&MyWindow::newCourse);

vLayout = new QVBoxLayout();
vLayout->addWidget(button1);
vLayout->addWidget(button2);
vLayout->addWidget(button3);

setLayout(vLayout);

}

void MyWindow::record() {
	
}

void MyWindow::newStudent() {

}

void MyWindow::newCourse() {

}


