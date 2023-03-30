#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include "studentdatabase.h"
#include "coursedatabase.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // create main window and layout
    QWidget mainWindow;
    QVBoxLayout mainLayout;

    // create student input layout
    QHBoxLayout studentInputLayout;
    QLabel nameLabel("Name:");
    QLineEdit nameLineEdit;
    QLabel studentIdLabel("Student ID:");
    QLineEdit studentIdLineEdit;
    QLabel cardIdLabel("Card ID:");
    QLineEdit cardIdLineEdit;
    QPushButton addStudentButton("Add Student");

    // create course input layout
    QHBoxLayout courseInputLayout;
    QLabel courseNameLabel("Course Name:");
    QLineEdit courseNameLineEdit;
    QLabel dateTimeLabel("Date/Time:");
    QLineEdit dateTimeLineEdit;
    QLabel studentIdsLabel("Student IDs (comma separated):");
    QLineEdit studentIdsLineEdit;
    QPushButton addCourseButton("Add Course");

    // add student input widgets to layout
    studentInputLayout.addWidget(&nameLabel);
    studentInputLayout.addWidget(&nameLineEdit);
    studentInputLayout.addWidget(&studentIdLabel);
    studentInputLayout.addWidget(&studentIdLineEdit);
    studentInputLayout.addWidget(&cardIdLabel);
    studentInputLayout.addWidget(&cardIdLineEdit);
    studentInputLayout.addWidget(&addStudentButton);

    // add course input widgets to layout
    courseInputLayout.addWidget(&courseNameLabel);
    courseInputLayout.addWidget(&courseNameLineEdit);
    courseInputLayout.addWidget(&dateTimeLabel);
    courseInputLayout.addWidget(&dateTimeLineEdit);
    courseInputLayout.addWidget(&studentIdsLabel);
    courseInputLayout.addWidget(&studentIdsLineEdit);
    courseInputLayout.addWidget(&addCourseButton);

    // add layouts to main layout
    mainLayout.addLayout(&studentInputLayout);
    mainLayout.addLayout(&courseInputLayout);

    // add main layout to main window
    mainWindow.setLayout(&mainLayout);

    // connect add student button to slot
    QObject::connect(&addStudentButton, &QPushButton::clicked, [&](){
        QString name = nameLineEdit.text();
        QString studentId = studentIdLineEdit.text();
        QString cardId = cardIdLineEdit.text();
        if (name.isEmpty() || studentId.isEmpty() || cardId.isEmpty()) {
            QMessageBox::warning(&mainWindow, "Error", "Please fill in all fields.");
            return;
        }
        if (StudentDatabase::insertStudent(name, studentId, cardId, QPixmap(), QByteArray())) {
            QMessageBox::information(&mainWindow, "Success", "Student added to database.");
            nameLineEdit.clear();
            studentIdLineEdit.clear();
            cardIdLineEdit.clear();
        } else {
            QMessageBox::warning(&mainWindow, "Error", "Failed to add student to database.");
        }
    });

    // connect add course button to slot
    QObject::connect(&addCourseButton, &QPushButton::clicked, [&](){
        QString name = courseNameLineEdit.text();
        QString dateTime = dateTimeLineEdit.text();
        QStringList studentIds = studentIdsLineEdit.text().split(",");
        if (name.isEmpty() || dateTime.isEmpty() || studentIds.isEmpty()) {
            QMessageBox::warning(&mainWindow, "Error", "Please fill in all fields.");
            return;
        }
        if (CourseDatabase::insertCourse(name, dateTime, studentIds)) {
            QMessageBox::information(&mainWindow, "Success", "Course added to database.");
            courseNameLineEdit.clear();
            dateTimeLineEdit.clear();
            studentIdsLineEdit.clear();
        } else {
            QMessageBox::warning(&mainWindow, "Error", "Failed
