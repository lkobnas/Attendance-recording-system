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
    QVBoxLayout *mainLayout;

    // create student input layout
    QHBoxLayout *studentInputLayout;
    QLabel *nameLabel;
    QLineEdit *nameLineEdit;
    QLabel *studentIdLabel;
    QLineEdit *studentIdLineEdit;
    QLabel *cardIdLabel;
    QLineEdit *cardIdLineEdit;
    QPushButton *addStudentButton;

    // create course input layout
    QHBoxLayout *courseInputLayout;
    QLabel *courseNameLabel;
    QLineEdit *courseNameLineEdit;
    QLabel *dateTimeLabel;
    QLineEdit *dateTimeLineEdit;
    QLabel *studentIdsLabel;
    QLineEdit *studentIdsLineEdit;
    QPushButton *addCourseButton;

    QVBoxLayout *vLayout;

    void record();
    void newStudent();
    void newCourse();

};

#endif // MYWINDOW_H

