#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QBoxLayout>
#include <QLabel>
#include <QFormLayout>
#include <QMessageBox>
#include <QLineEdit>


class MyWindow : public QWidget {
    Q_OBJECT

public:
    MyWindow();

private:
    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;

    // create main window and layout
    QVBoxLayout *mainInputLayout;
    QVBoxLayout *mainListLayout;
    QHBoxLayout *mainLayout;

    // create student input layout
    QVBoxLayout *recordLayout;
    QPushButton *recordButton;

    // empty label for spacing
    QLabel *spaceLabel;

    // create student input layout
    QVBoxLayout *studentInputLayout;
    QHBoxLayout *s1;
    QHBoxLayout *s2;
    QHBoxLayout *s3;
    QLabel *nameLabel;
    QLineEdit *nameLineEdit;
    QPushButton *studentClearButton;
    QPushButton *cardButton;
    QPushButton *fingerprintButton;
    QLabel *studentIDLabel;
    QLineEdit *studentIDLineEdit; 
    QPushButton *addStudentButton;

    // create course input layout
    QVBoxLayout *courseInputLayout;
    QHBoxLayout *c1;
    QHBoxLayout *c2;
    QHBoxLayout *c3;
    QLabel *courseNameLabel;
    QLineEdit *courseNameLineEdit;
    QPushButton *courseClearButton;
    QLabel *dateTimeLabel;
    QLineEdit *dateTimeLineEdit;
    QLabel *studentIdsLabel;
    QLineEdit *studentIdsLineEdit;
    QPushButton *addCourseButton;

    
    // create list layout
    QVBoxLayout *studentListLayout;
    QLabel *studentLabel;
    QStringList *studentList;  
    QVBoxLayout *courseListLayout;
    QLabel *courseLabel;
    QStringList *courseList;  

    QTextLayout *



    void record();
    void addNewStudent();
    void addNewCourse();

};

#endif // MYWINDOW_H

