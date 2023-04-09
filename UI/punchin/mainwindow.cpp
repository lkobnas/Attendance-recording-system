#include "mainwindow.h"
#include "./ui_mainwindow.h"
//#include "courseDB.h"

#include <sqlite3.h>

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>
#include <QTableView>

#include <QtCore>
#include <QtGui>
#include <QException>

//#include <QSqlDatabase>
//#include <QSqlTableModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this,SLOT(update()));
    timer->start(500);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init(){
    setWindowTitle("PunchIN - Stduent Mode");
    QFont font = ui->mDateTimeLabel->font();
    font.setPointSize(25);
    font.setBold(true);
    ui->mDateTimeLabel->setFont(font);
    font = ui->label_courseTimetable->font();
    font.setPointSize(18);
    ui->label_courseTimetable->setFont(font);

    CourseDB cdb;
    QList<Course> courseList;
    try{
        cdb.initDB();
        courseList = cdb.getAllCourses();
    }catch(QException &e){
        const MyException* myException = dynamic_cast<const MyException*>(&e);
        if (myException) {
            QString errorMessage = myException->message();
            QMessageBox::warning(this, "Course database error", errorMessage);
            return;
        }
    }
    
    // Create a QStandardItemModel to represent the data source for the table view
    QStandardItemModel *model = new QStandardItemModel(this);
    model->setRowCount(courseList.size());
    model->setColumnCount(4);
    model->setHeaderData(0, Qt::Horizontal, "Course Name");
    model->setHeaderData(1, Qt::Horizontal, "Date Time");
    model->setHeaderData(2, Qt::Horizontal, "Arrived");
    model->setHeaderData(3, Qt::Horizontal, "Total");
    // Create a QTableView object and set the model for the view
    ui->tableView->setModel(model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Set the column widths and header titles
    ui->tableView->setColumnWidth(0, 100);
    ui->tableView->setColumnWidth(1, 160);
    ui->tableView->setColumnWidth(2, 60);
    ui->tableView->setColumnWidth(3, 40);
    //ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for(int i=0;i<courseList.size();i++){
        model->setData(model->index(i,0),courseList[i].name);
        model->setData(model->index(i,1),courseList[i].datetime);
        model->setData(model->index(i,2),courseList[i].arrivedStudents.size());
        model->setData(model->index(i,3),courseList[i].studentList.size());
    }
    //ui->tableView->setHorizontalHeaderLabels(QStringList() << "Course Name" << "Date Time" << "Arrived/Total Students");

}

void MainWindow::update(){
    updateDatetimeDisplay();
}

void MainWindow::on_addNewStudentButton_clicked()
{
    AddStudentWindow sWindow;
    sWindow.setModal(true);
    sWindow.exec();
}


void MainWindow::on_addNewCourseButton_clicked()
{
    AddCourseWindow cWindow;
    cWindow.setModal(true);
    cWindow.exec();
}

void MainWindow::updateDatetimeDisplay()
{
    QDateTime datetime = QDateTime::currentDateTime();
    QString dateTimeText = datetime.toString("dd/MM/yyyy hh:mm:ss");
    ui->mDateTimeLabel->setText(dateTimeText);
    
}

void MainWindow::updateTableView()
{
    
    
}

void MainWindow::on_actionAdministrator_mode_triggered()
{
    if(adminMode == false){
        bool ok;
        QString pw = QInputDialog::getText(this,"Admin Mode","Please enter admin passcode: ",QLineEdit::Password); //TODO: check ok button
        if (!pw.isEmpty()){
            if(pw == "5220"){              
                adminMode = true;
                setWindowTitle("PunchIN - Admin Mode");
                QMessageBox::information(this, "Admin Mode Active", "Switched to Admin mode");
            }else{
                QMessageBox::warning(this, "Error switching to admin mode", "Wrong passcode");
            }
        }                   
    }
}

void MainWindow::on_actionSwitch_to_student_mode_triggered()
{
    if(adminMode == true){
        adminMode = false;
        setWindowTitle("PunchIN - Stduent Mode");
    }
}

void getUpdatedTime(){

}

