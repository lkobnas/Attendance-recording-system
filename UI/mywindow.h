#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QBoxLayout>


class MyWindow : public QWidget {
    Q_OBJECT

public:
    MyWindow();

private:
    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;

    QVBoxLayout  *vLayout;

    void record();
    void newStudent();
    void newCourse();

};

#endif // MYWINDOW_H

