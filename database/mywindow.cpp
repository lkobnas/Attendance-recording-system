#include "mywindow.h"

MyWindow::MyWindow(){

    button1 = new QPushButton("Attendance Recording Mode");
    button2 = new QPushButton("Register New Student");
    button3 = new QPushButton("Create New Course");

    connect(button1,&QPushButton::clicked,this,&MyWindow::record);
    connect(button2,&QPushButton::clicked,this,&MyWindow::newStudent);
    connect(button3,&QPushButton::clicked,this,&MyWindow::newCourse);

    // create student input layout
    
    studentInputLayout = new QHBoxLayout();
    nameLabel = new QLabel("Name:");
    nameLineEdit = new QLineEdit();
    studentIdLabel = new QLabel("Student ID:");
    studentIdLineEdit = new QLineEdit();
    cardIdLabel = new QLabel("Card ID:");
    cardIdLineEdit = new QLineEdit();
    addStudentButton = new QPushButton("Add Student");

    // create course input layout
    courseInputLayout = new QHBoxLayout();
    courseNameLabel = new QLabel("Course Name:");
    courseNameLineEdit = new QLineEdit();
    dateTimeLabel = new QLabel("Date/Time (DD/MM/YYYY MM/HH):");
    dateTimeLineEdit = new QLineEdit();
    studentIdsLabel = new QLabel("Student IDs (comma separated):");
    studentIdsLineEdit = new QLineEdit();
    addCourseButton = new QPushButton("Add Course");

    // add student input widgets to layout
    studentInputLayout->addWidget(nameLabel);
    studentInputLayout->addWidget(nameLineEdit);
    studentInputLayout->addWidget(studentIdLabel);
    studentInputLayout->addWidget(studentIdLineEdit);
    studentInputLayout->addWidget(cardIdLabel);
    studentInputLayout->addWidget(cardIdLineEdit);
    studentInputLayout->addWidget(addStudentButton);

    // add course input widgets to layout
    courseInputLayout->addWidget(courseNameLabel);
    courseInputLayout->addWidget(courseNameLineEdit);
    courseInputLayout->addWidget(dateTimeLabel);
    courseInputLayout->addWidget(dateTimeLineEdit);
    courseInputLayout->addWidget(studentIdsLabel);
    courseInputLayout->addWidget(studentIdsLineEdit);
    courseInputLayout->addWidget(addCourseButton);

    // create and add layouts to main layout
    mainLayout = new QVBoxLayout();
    mainLayout->addLayout(studentInputLayout);
    mainLayout->addLayout(courseInputLayout);

    vLayout = new QVBoxLayout();
    vLayout->addWidget(button1);
    vLayout->addWidget(button2);
    vLayout->addWidget(button3);

    setLayout(mainLayout);

}

void MyWindow::record() {
	
}

void MyWindow::newStudent() {

}

void MyWindow::newCourse() {

}


