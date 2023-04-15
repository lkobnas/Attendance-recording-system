#include "mainwindow.h"

#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
    //Timer for date/time display
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this,SLOT(update()));
    timer->start(500);
    
    //Timer for input delay
    delay_timer = new QTimer(this);
    delay_timer->setInterval(1500);
    connect(delay_timer, SIGNAL(timeout()),this,SLOT(onUIDReceived()));

    // Email Timer
    email_timer = new QTimer(this);
    email_timer->setInterval(2000);
    connect(email_timer, &QTimer::timeout, this, &MainWindow::checkCourseStart);

    //Timer for input delay
    //connect(this, &MainWindow::passCardID, this, &AddStudentWindow::receiveVariable, Qt::QueuedConnection);
    rfidThread = std::thread(&MainWindow::rfidListener, this);

}

MainWindow::~MainWindow()
{
    rfidThread.detach();
    rfidThread.join();
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

    studentWindowValid = false;
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
    model = new QStandardItemModel(this);
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
    ui->tableView->setColumnWidth(0, 105);
    ui->tableView->setColumnWidth(1, 160);
    ui->tableView->setColumnWidth(2, 60);
    ui->tableView->setColumnWidth(3, 40);

    //ui->tableView->setHorizontalHeaderLabels(QStringList() << "Course Name" << "Date Time" << "Arrived/Total Students");
    updateTableView();
}

void MainWindow::update(){
    updateDatetimeDisplay();
    checkCourseStart();
}

void MainWindow::on_addNewStudentButton_clicked()
{
    if(adminMode){
        sWindow = new AddStudentWindow(this);
        connect(this, &MainWindow::passCardID, sWindow, &AddStudentWindow::receiveCardID);
        connect(sWindow, &QDialog::finished, this, &MainWindow::onAddStudentWindowClosed);
        studentWindowValid = true;
        //sWindow->setModal(true);
        sWindow->show(); 
        //studentWindowValid = false;
    }else{
        QMessageBox::warning(this, "Permission Denied", "Please switch to admin mode");
    }
}

void MainWindow::onAddStudentWindowClosed(){
    studentWindowValid = false;
}

void MainWindow::on_addNewCourseButton_clicked()
{
    if(adminMode){
        AddCourseWindow cWindow;
        cWindow.setModal(true);
        cWindow.exec();
        updateTableView();
    }else{
        QMessageBox::warning(this, "Permission Denied", "Please switch to admin mode");
    }
}

void MainWindow::updateDatetimeDisplay()
{
    QDateTime datetime = QDateTime::currentDateTime();
    QString dateTimeText = datetime.toString("dd/MM/yyyy hh:mm:ss");
    ui->mDateTimeLabel->setText(dateTimeText);
    
}

void MainWindow::updateTableView()
{
    try{
        courseList = cdb.getAllCourses();
        model->setRowCount(courseList.size());
        for(int i=0;i<courseList.size();i++){
            model->setData(model->index(i,0),courseList[i].name);
            model->setData(model->index(i,1),courseList[i].datetime);
            model->setData(model->index(i,2),courseList[i].arrivedStudents.size());
            model->setData(model->index(i,3),courseList[i].studentList.size() + courseList[i].arrivedStudents.size());
        }
    }catch(QException &e){
        const MyException* myException = dynamic_cast<const MyException*>(&e);
        if (myException) {
            QString errorMessage = myException->message();
            QMessageBox::warning(this, "Course database error", errorMessage);
            return;
        }
    }
    ui->tableView->update();
}

void MainWindow::checkCourseStart(){
    QDateTime currentTime = QDateTime::currentDateTime();
    //QString currentTimeText = currentTime.toString("dd/MM/yyyy hh:mm:ss");
    QAbstractItemModel* model = ui->tableView->model();
    QModelIndex datetimeIndex = model->index(0, 1, QModelIndex());
    QModelIndex coursenameIndex = model->index(0, 0, QModelIndex());
    QVariant datetimeData = model->data(datetimeIndex);
    QVariant coursenameData = model->data(coursenameIndex);

    QString courseName = coursenameData.toString();
    QDateTime courseTime = QDateTime::fromString(datetimeData.toString(), "yyyy-MM-dd hh:mm");
    if(courseTime.isNull()){
        return;
    }
 
    if(qAbs(currentTime.secsTo(courseTime)) <= 1){
        if(email_timer->isActive()){
            return;
        }
        //late email    
        qDebug() << "late_email triggered";
        Course course = cdb.getCourse(courseName);
        QList<Student> studentList = course.studentList;
        Email email;
        for(int i=0;i<studentList.size();i++){
            email.send_email_lateReminder(studentList[i].email.toStdString(),courseName.toStdString(),datetimeData.toString().toStdString());
            qDebug()<< studentList[i].email;
        }
        email_timer->start();
    }else if(currentTime > courseTime.addMSecs(1000*60*1)){ // TEST for 1 min
        //delete course
        try{
            qDebug() << "auto_delete triggered";
            cdb.deleteCourse(courseName);
        }catch(QException &e){
            const MyException* myException = dynamic_cast<const MyException*>(&e);
            if (myException) {
                QString errorMessage = myException->message();
                QMessageBox::warning(this, "Course database error", errorMessage);
                return;
            }
        }
        QMessageBox* popup = new QMessageBox(QMessageBox::Information, "Auto delete triggered", "Course Deleted", QMessageBox::Close, nullptr);
        popup->setAttribute(Qt::WA_DeleteOnClose); // delete the popup automatically when it's closed
        QTimer::singleShot(3000, popup, &QMessageBox::close); // close the popup after 3 seconds
        popup->show(); 
        updateTableView();        
    }
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

QList<QStringList> getSelectedData(QTableView* tableView) {
    QList<QStringList> selectedData;

    // Get the selection model for the QTableView
    QItemSelectionModel* selectionModel = tableView->selectionModel();

    // Get the selected indexes
    QModelIndexList selectedIndexes = selectionModel->selectedIndexes();

    //Iterate over the selected indexes and add the corresponding data to the list
    for (const QModelIndex& index : selectedIndexes) {
        QStringList rowData;
        const QStandardItemModel* model = static_cast<const QStandardItemModel*>(tableView->model());
        rowData.append(model->data(model->index(index.row(), 0)).toString());      
        selectedData.append(rowData);
    }

    return selectedData;
}


void MainWindow::on_testButton_clicked()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    //QString currentTimeText = currentTime.toString("dd/MM/yyyy hh:mm:ss");
    QAbstractItemModel* model = ui->tableView->model();
    QModelIndex datetimeIndex = model->index(0, 1, QModelIndex());
    QModelIndex coursenameIndex = model->index(0, 0, QModelIndex());
    QVariant datetimeData = model->data(datetimeIndex);
    QVariant coursenameData = model->data(coursenameIndex);

    QString courseName = coursenameData.toString();
    QDateTime courseTime = QDateTime::fromString(datetimeData.toString(), "yyyy-MM-dd hh:mm");
    if(courseTime.isNull()){
        return;
    }
        //late email    
    qDebug() << "late_email triggered";
    Course course = cdb.getCourse(courseName);
    QList<Student> studentList = course.studentList;    
    Email email;
    for(int i=0;i<studentList.size();i++){
        qDebug() << studentList[i].email;
        email.send_email_lateReminder(studentList[i].email.toStdString(),courseName.toStdString(),datetimeData.toString().toStdString());
    }
    
    
}

void MainWindow::on_studentListButton_clicked()
{
    QList<Student> studentList;
    
    try{
        sdb.initDB();
        studentList  = sdb.getAllStudents();
        
    }catch(QException &e){
        const MyException* myException = dynamic_cast<const MyException*>(&e);
        if (myException) {
            QString errorMessage = myException->message();
            QMessageBox::warning(this, "Student database error", errorMessage);
            return;
        }
    }
    
    QDialog* popupWindow = new QDialog(this);
    popupWindow->setWindowTitle("Student List");
    popupWindow->resize(500, 300);

    // Create a new QTableView and set its model to a QStandardItemModel with 3 columns
    QTableView* tableView = new QTableView(popupWindow);
    // Create a QStandardItemModel to represent the data source for the table view
    model = new QStandardItemModel(this);
    model->setRowCount(studentList.size());
    model->setColumnCount(3);
    model->setHeaderData(0, Qt::Horizontal, "Name");
    model->setHeaderData(1, Qt::Horizontal, "Student ID");
    model->setHeaderData(2, Qt::Horizontal, "Email");
    // Create a QTableView object and set the model for the view
    tableView->setModel(model);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // Set the column widths and header titles
    tableView->setColumnWidth(0, 140);
    tableView->setColumnWidth(1, 105);
    tableView->setColumnWidth(2, 220);

    for(int i=0; i<studentList.size();i++){
        model->setData(model->index(i,0),studentList[i].name);
        model->setData(model->index(i,1),studentList[i].sid);
        model->setData(model->index(i,2),studentList[i].email);
    }

    // Add the table view to a layout and set the layout for the popup window
    QVBoxLayout* layout = new QVBoxLayout(popupWindow);
    layout->addWidget(tableView);
    
    popupWindow->setLayout(layout);

    // Show the popup window
    popupWindow->exec();
}


void MainWindow::on_deleteCourseButton_clicked()
{
    QList<QStringList> selectedData = getSelectedData(ui->tableView);
    if(selectedData.size()<1){
        QMessageBox::warning(this, "Error deleting course", "Please select a course");
    }else if(selectedData.size()>1){
        QMessageBox::warning(this, "Error deleting course", "Please only select one course at one time");
    }else{
        try{
            cdb.deleteCourse(selectedData[0][0]);
        }catch(QException &e){
            const MyException* myException = dynamic_cast<const MyException*>(&e);
            if (myException) {
                QString errorMessage = myException->message();
                QMessageBox::warning(this, "Course database error", errorMessage);
                return;
            }
        }
        QMessageBox::information(this, "Success", "Course deleted");
        updateTableView();
    }

}

void MainWindow::cardCallback(const QString &uid)
{
    qDebug() << "RFID input received: " << uid;

}

void MainWindow::rfidListener() {
    while (true) {
        std::string uid = rfid.get_uid();
        
        if (!uid.empty()) { 
            QString quid = QString::fromStdString(uid);
            QMetaObject::invokeMethod(this, "onUIDReceived", Qt::QueuedConnection,
                                      Q_ARG(QString, quid));
        }
    }
}

void MainWindow::recordAttendanceWindow(QString studentID)
{
    //doorlock
    qDebug() << "recordAttendanceWindow triggered";
    QString s = "Your attendance have been recorded, your SID: "+studentID;
    QMessageBox* popup = new QMessageBox(QMessageBox::Information, "Success", s, QMessageBox::Close, nullptr);
    popup->setAttribute(Qt::WA_DeleteOnClose); // delete the popup automatically when it's closed
    QTimer::singleShot(3000, popup, &QMessageBox::close); // close the popup after 3 seconds
    popup->show(); 
}


void MainWindow::onUIDReceived(const QString uid) {
    //update database arrived
    if(delay_timer->isActive()){
        return;
    }
    delay_timer->start();

    if(studentWindowValid){       
        connect(this, &MainWindow::passCardID, sWindow, 
        [=](const QVariant& value) {
            sWindow->receiveCardID(value.toString());
        }, Qt::QueuedConnection);
        // Emit the signal with the variable
        emit passCardID(uid);        
        return;
    }

    Student student;
    try{
        student = sdb.getStudentByCardID(uid);
    }catch(QException &e){
        const MyException* myException = dynamic_cast<const MyException*>(&e);
        if (myException) {
            QString errorMessage = myException->message();
            QMessageBox::warning(this, "Student database error", errorMessage);
            return;
        }
    }
    if(student.sid.isEmpty()){
        QMessageBox* popup = new QMessageBox(QMessageBox::Warning, "Invalid Card", "You are not registered in the system", QMessageBox::Close, nullptr);
        popup->setAttribute(Qt::WA_DeleteOnClose); // delete the popup automatically when it's closed
        QTimer::singleShot(3000, popup, &QMessageBox::close); // close the popup after 3 seconds
        popup->show(); 
        return;
    }
    QAbstractItemModel* model = ui->tableView->model();
    QModelIndex firstIndex = model->index(0, 0, QModelIndex());
    // Get the data stored in the first index
    QVariant data = model->data(firstIndex);
    // Convert the data to a QString if necessary
    QString courseName = data.toString();
    if(courseName.isEmpty()){
        QMessageBox* popup = new QMessageBox(QMessageBox::Warning, "No coming class", "No upcoming class", QMessageBox::Close, nullptr);
        popup->setAttribute(Qt::WA_DeleteOnClose); // delete the popup automatically when it's closed
        QTimer::singleShot(3000, popup, &QMessageBox::close); // close the popup after 3 seconds
        popup->show(); 
        return;
    }
    try{
        cdb.updateArrived(courseName, student.sid);
    }catch(QException &e){
        const MyException* myException = dynamic_cast<const MyException*>(&e);
        if (myException) {
            QString errorMessage = myException->message();
            QMessageBox::warning(this, "Failed to record attendance", errorMessage);
            return;
        }
    }    
    // Send Attendance recorded email
    Email email_curl;
    email_curl.send_email_record(student.email.toStdString(), courseName.toStdString());
    recordAttendanceWindow(student.sid);
    updateTableView();
    // Update the UI with the RFID input

}

