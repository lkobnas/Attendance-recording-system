#include "mywindow.h"

MyWindow::MyWindow(){

    button1 = new QPushButton("Attendance Recording Mode");
    button2 = new QPushButton("Register New Student");
    button3 = new QPushButton("Create New Course");

    connect(button1,&QPushButton::clicked,this,&MyWindow::record);
    connect(button2,&QPushButton::clicked,this,&MyWindow::addNewStudent);
    connect(button3,&QPushButton::clicked,this,&MyWindow::addNewCourse);

    spaceLabel = new QLabel();
    
    // create record attendance layout
    recordLayout = new QVBoxLayout();
    recordButton = new QPushButton("Record Attendance");
    recordLayout->addWidget(recordButton);
    
    // create student input layout
    s1 = new QHBoxLayout();
    s2 = new QHBoxLayout();
    s3 = new QHBoxLayout();
    studentInputLayout = new QVBoxLayout();
    nameLabel = new QLabel("Student name:");
    nameLineEdit = new QLineEdit();
    studentClearButton = new QPushButton("Clear");
    cardButton = new QPushButton("Scan Student Card");
    fingerprintButton = new QPushButton("Scan Fingerprint");
    studentIDLabel = new QLabel("Student ID");
    studentIDLineEdit = new QLineEdit();
    addStudentButton = new QPushButton("Add Student");

    // add student input widgets to layout
    s1->addWidget(nameLabel);
    s1->addWidget(nameLineEdit);
    s1->addWidget(studentClearButton);
    s2->addWidget(cardButton);
    s2->addWidget(fingerprintButton);
    s3->addWidget(addStudentButton);
    studentInputLayout->addLayout(s1);
    studentInputLayout->addLayout(s2);
    studentInputLayout->addLayout(s3);

    // create course input layout

    courseInputLayout = new QVBoxLayout();
    c1 = new QHBoxLayout();
    c2 = new QHBoxLayout();
    c3 = new QHBoxLayout();
    courseNameLabel = new QLabel("Course Name:");
    courseNameLineEdit = new QLineEdit();
    courseClearButton = new QPushButton("Clear");
    dateTimeLabel = new QLabel("Date/Time (DD/MM/YYYY MM:HH):");
    dateTimeLineEdit = new QLineEdit();
    studentIdsLabel = new QLabel("Student IDs (comma separated):");
    studentIdsLineEdit = new QLineEdit();
    addCourseButton = new QPushButton("Add Course");

    // add course input widgets to layout
    c1->addWidget(courseNameLabel);
    c1->addWidget(courseNameLineEdit);
    c1->addWidget(courseClearButton);
    c2->addWidget(dateTimeLabel);
    c2->addWidget(dateTimeLineEdit);
    c2->addWidget(studentIdsLabel);
    c2->addWidget(studentIdsLineEdit);
    c3->addWidget(addCourseButton);
    courseInputLayout->addLayout(c1);
    courseInputLayout->addLayout(c2);
    courseInputLayout->addLayout(c3);

    // create mainInputLayout
    mainInputLayout = new QVBoxLayout();
    mainInputLayout->addLayout(recordLayout);
    mainInputLayout->addLayout(studentInputLayout);
    mainInputLayout->addLayout(courseInputLayout);

    
    //create student list layout
    studentListLayout = new QVBoxLayout();
    studentLabel = new QLabel("Student List");
    studentList = new QStringList(); 
    courseListLayout = new QVBoxLayout();
    courseLabel = new QLabel("Course List");
    courseList = new QStringList();
    mainListLayout = new QVBoxLayout();

    studentListLayout->addWidget(studentLabel);
    //studentListLayout->addWidget(studentList);
    courseListLayout->addWidget(courseLabel);
    mainListLayout->addLayout(studentListLayout);
    mainListLayout->addLayout(courseListLayout);


    // create and add layouts to main layout
    mainLayout = new QHBoxLayout();
    mainLayout->addLayout(mainInputLayout);
    mainLayout->addLayout(mainListLayout);
    

    setLayout(mainLayout);

}

void MyWindow::record() {
	
}

void MyWindow::addNewStudent() {

}

void MyWindow::addNewCourse() {

}


