#include "mainwindow.h"
#include <QMessageBox>
/// @brief MainWindow Constructor
/// @param parent points to QMainWindow from main
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fp.fp_init();
    init();
    //Timer for date/time display
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this,SLOT(update()));
    timer->start(500);
    
    //Timer for input delay
    delay_timer = new QTimer(this);
    delay_timer->setInterval(1500);
    connect(delay_timer, &QTimer::timeout, this, &MainWindow::d_resetFunctionRunningFlag);
    d_functionRunning = false;

    // Email Timer
    email_timer = new QTimer(this);
    email_timer->setInterval(2000);
    connect(email_timer, &QTimer::timeout, this, &MainWindow::e_resetFunctionRunningFlag);
    e_functionRunning = false;

    //Timer for input delay
    //connect(this, &MainWindow::passCardID, this, &AddStudentWindow::receiveVariable, Qt::QueuedConnection);
    running = true;
    fpMode = 1;
    rfidThread = std::thread(&MainWindow::rfidListener, this);
    fingerprintIdentifyThread = std::thread(&MainWindow::fingerprintIdentifyListener, this);
    doorlockThread = std::thread(&MainWindow::doorControl, this);
    
}

/// @brief MainWindow Destructor
MainWindow::~MainWindow()
{
    running = false;
    while(rfidThread.joinable()) {
        rfidThread.join();
    }
    while(fingerprintIdentifyThread.joinable()) {
        fingerprintIdentifyThread.join();
    }
    while(doorlockThread.joinable()) {
        doorlockThread.join();
    }
    delete ui;
}
void MainWindow::on_actionClose_triggered()
{
    running = false;
    while(rfidThread.joinable()) {
        rfidThread.join();
    }
    while(fingerprintIdentifyThread.joinable()) {
        fingerprintIdentifyThread.join();
    }
    while(doorlockThread.joinable()) {
        doorlockThread.join();
    }
    close();
}

/// @brief Initialise UI, doorlock and variable for checking studentwindow, read course database and display on tableview
void MainWindow::init(){
    setWindowTitle("PunchIN - Stduent Mode");
    QFont font = ui->mDateTimeLabel->font();
    font.setPointSize(25);
    font.setBold(true);
    ui->mDateTimeLabel->setFont(font);
    font = ui->label_courseTimetable->font();
    font.setPointSize(18);
    ui->label_courseTimetable->setFont(font);

    font.setPointSize(18);
    ui->labelUpcomingCourseName->setFont(font);
    font.setBold(false);
    ui->labelUpcomingCourse->setFont(font);

    door = false;
    doorlock.init(26);
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
    // Initialise Course Table
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
    ui->tableView->setColumnWidth(0, 110);
    ui->tableView->setColumnWidth(1, 158);
    ui->tableView->setColumnWidth(2, 60);
    ui->tableView->setColumnWidth(3, 60);

    // Initialise Student Table
    sModel = new QStandardItemModel(this);
    sModel->setColumnCount(2);
    sModel->setHeaderData(0, Qt::Horizontal, "Expected Students");
    sModel->setHeaderData(1, Qt::Horizontal, "Arrived Students");
    ui->studentTableView->setModel(sModel);
    ui->studentTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->studentTableView->setColumnWidth(0, 170);
    ui->studentTableView->setColumnWidth(1, 169);

    updateTableView();
}

/// @brief course start 
void MainWindow::update(){
    updateDatetimeDisplay();
    checkCourseStart();
}

/// @brief Open add student window when button is clicked
void MainWindow::on_addNewStudentButton_clicked()
{
    if(adminMode){
        fpMode = 2;
        sWindow = new AddStudentWindow(this);
        connect(this, &MainWindow::passCardID, sWindow, &AddStudentWindow::receiveCardID);
        connect(this, &MainWindow::passFpID, sWindow, &AddStudentWindow::receiveFPID);
        connect(sWindow, &QDialog::finished, this, &MainWindow::onAddStudentWindowClosed);
        studentWindowValid = true;
        //sWindow->setModal(true);
        sWindow->show(); 
        //studentWindowValid = false;
    }else{
        QMessageBox::warning(this, "Permission Denied", "Please switch to admin mode");
    }
}

/// @brief change fingerprint detect mode when add student window is closed
void MainWindow::onAddStudentWindowClosed(){
    studentWindowValid = false;
    fpMode = 1;
}

/// @brief Open a add course window when button is clicked
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

/// @brief update date and time display on mainwindow
void MainWindow::updateDatetimeDisplay()
{
    QDateTime datetime = QDateTime::currentDateTime();
    QString dateTimeText = datetime.toString("dd/MM/yyyy hh:mm:ss");
    ui->mDateTimeLabel->setText(dateTimeText);
    
}

/// @brief update course table
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
    ui->tableView->setModel(model);
    ui->tableView->update();
    updateStudentTable();
}

/// @brief check if any course is starting
void MainWindow::checkCourseStart(){
    QDateTime currentTime = QDateTime::currentDateTime();

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
        if(e_functionRunning){
            return;
        }
        e_functionRunning = true;
        email_timer->start();
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
    }else if(currentTime > courseTime.addMSecs(1000*60*15)){ // TEST for 1 min
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

/// @brief Change to administrator mode
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

/// @brief Change to student mode
void MainWindow::on_actionSwitch_to_student_mode_triggered()
{
    if(adminMode == true){
        adminMode = false;
        setWindowTitle("PunchIN - Stduent Mode");
    }
}

/// @brief Open student list when button is clicked
void MainWindow::on_studentListButton_clicked()
{
    if(!adminMode){
        QMessageBox::warning(this, "Permission Denied", "Please switch to admin mode");
        return;
    }

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
    QTableView* sltableView = new QTableView(popupWindow);
    // Create a QStandardItemModel to represent the data source for the table view

    QStandardItemModel* slmodel= new QStandardItemModel(this);
    slmodel->setRowCount(studentList.size());
    slmodel->setColumnCount(3);
    slmodel->setHeaderData(0, Qt::Horizontal, "Name");
    slmodel->setHeaderData(1, Qt::Horizontal, "Student ID");
    slmodel->setHeaderData(2, Qt::Horizontal, "Email");
    // Create a QTableView object and set the model for the view
    sltableView->setModel(slmodel);
    sltableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // Set the column widths and header titles
    sltableView->setColumnWidth(0, 140);
    sltableView->setColumnWidth(1, 105);
    sltableView->setColumnWidth(2, 220);

    for(int i=0; i<studentList.size();i++){
        slmodel->setData(slmodel->index(i,0),studentList[i].name);
        slmodel->setData(slmodel->index(i,1),studentList[i].sid);
        slmodel->setData(slmodel->index(i,2),studentList[i].email);
    }

    // Add the table view to a layout and set the layout for the popup window
    QVBoxLayout* layout = new QVBoxLayout(popupWindow);
    layout->addWidget(sltableView);
    
    popupWindow->setLayout(layout);

    // Show the popup window
    popupWindow->exec();
}

/// @brief Get selected course in the tableview
/// @param tableView 
/// @return 
QList<QStringList> getSelectedData(QTableView* tableView) {
    QList<QStringList> selectedData;

    // Get the selection model for the QTableView
    QItemSelectionModel* selectionModel = tableView->selectionModel();

    // Get the selected indexes
    QModelIndexList selectedIndexes = selectionModel->selectedIndexes();

    //Iterate over the selected indexes and add the corresponding data to the list
    for (const QModelIndex& index : selectedIndexes) {
        QStringList rowData;
        const QStandardItemModel* tmodel = static_cast<const QStandardItemModel*>(tableView->model());
        rowData.append(tmodel->data(tmodel->index(index.row(), 0)).toString());      
        selectedData.append(rowData);
    }

    return selectedData;
}

/// @brief update student table with upcoming course
void MainWindow::updateStudentTable(){

    QModelIndex coursenameIndex = model->index(0, 0, QModelIndex());
    QVariant coursenameData = model->data(coursenameIndex);
    QString courseName = coursenameData.toString();
    if(courseName.isEmpty()){
        ui->labelUpcomingCourseName->setText("None");
        sModel->removeRows(0, sModel->rowCount());
        ui->studentTableView->update();
        return;
    }
    ui->labelUpcomingCourseName->setText(courseName);
    Course course;
    try{
        course = cdb.getCourse(courseName);       
        sModel->removeRows(0, sModel->rowCount());
        sModel->setRowCount(std::max(course.studentList.size(),course.arrivedStudents.size()));
        for(int i=0;i<course.studentList.size();i++){
            sModel->setData(sModel->index(i,0),course.studentList[i].name);
        }
        for(int i=0;i<course.arrivedStudents.size();i++){
            sModel->setData(sModel->index(i,1),course.arrivedStudents[i].name);
        }
    }catch(QException &e){
        const MyException* myException = dynamic_cast<const MyException*>(&e);
        if (myException) {
            QString errorMessage = myException->message();
            QMessageBox::warning(this, "Course database error", errorMessage);
            return;
        }
    }
    ui->studentTableView->update();
    
}

/// @brief Delete selected course when button is clicked
void MainWindow::on_deleteCourseButton_clicked()
{
    if(!adminMode){
        QMessageBox::warning(this, "Permission Denied", "Please switch to admin mode");
        return;
    }
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

/// @brief Listener for RFID reader
void MainWindow::rfidListener() {
    while (running) {
        std::string uid = rfid.get_uid();
        
        if (!uid.empty()) { 
            QString quid = QString::fromStdString(uid);
            QMetaObject::invokeMethod(this, "onUIDReceived", Qt::QueuedConnection,
                                      Q_ARG(QString, quid));
        }
    }
}


/// @brief Listener for fingerprint sensor
void MainWindow::fingerprintIdentifyListener() {
    while (running) {
        if(fpMode == 1){    //Identify mode: Active in MainWindow
            int fpID = fp.fp_identify();
            if (!(fpID==-1)) { 
                QString qfpID = QString::number(fpID);
                QMetaObject::invokeMethod(this, "onFPIDIdentifyReceived", Qt::QueuedConnection,
                                        Q_ARG(QString, qfpID));
            }
        }else if(fpMode == 2){  //Enroll mode: Active in AddStudentWindow
            int fpID = fp.fp_enroll();
            qDebug() <<"fpID: "<<fpID;
            if (!(fpID==-1)) { 
                QString qfpID = QString::number(fpID);
                
                QMetaObject::invokeMethod(this, "onFPIDAddReceived", Qt::QueuedConnection,
                                        Q_ARG(QString, qfpID));
            }           
        }delay(2000); //Delay must be added in this thread to give time AS608 to process before next detection 
    }
}



/// @brief Record student attendance when rfid or fingerprint detected
/// @param studentID 
void MainWindow::recordAttendanceWindow(QString studentID)
{
    door = true;
    QString s = "Student with SID: "+studentID+" . Your attendance have been recorded";
    QMessageBox* popup = new QMessageBox(QMessageBox::Information, "Success", s, QMessageBox::Close, nullptr);
    popup->setAttribute(Qt::WA_DeleteOnClose); // delete the popup automatically when it's closed
    QTimer::singleShot(3000, popup, &QMessageBox::close); // close the popup after 3 seconds
    popup->show(); 
}

/// @brief email timer running flag
void MainWindow::e_resetFunctionRunningFlag(){
    e_functionRunning = false;
}
/// @brief delay timer running flag
void MainWindow::d_resetFunctionRunningFlag(){
    d_functionRunning = false;
}

/// @brief Triggered whe Student card detected, update attendance list updated and send attendance record email
/// @param uid is student card id that return by rfid.cpp
void MainWindow::onUIDReceived(const QString uid) {

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
}


/// @brief Triggered whe Student card detected, update attendance list updated and send attendance record email
/// @param fpid 
void MainWindow::onFPIDIdentifyReceived(QString fpid){
  Student student;
    try{
        student = sdb.getStudentByFPID(fpid);
    }catch(QException &e){
        const MyException* myException = dynamic_cast<const MyException*>(&e);
        if (myException) {
            QString errorMessage = myException->message();
            QMessageBox::warning(this, "Student database error", errorMessage);
            return;
        }
    }
    if(student.sid.isEmpty()){
        QMessageBox* popup = new QMessageBox(QMessageBox::Warning, "Invalid Fingerprint", "Fingerprint is not registered in the system", QMessageBox::Close, nullptr);
        popup->setAttribute(Qt::WA_DeleteOnClose); // delete the popup automatically when it's closed
        QTimer::singleShot(3000, popup, &QMessageBox::close); // close the popup after 3 seconds
        popup->show(); 
        return;
    }
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
}

/// @brief Triggered when add student window is opened, fingerprint sensor will switch to add mode and emit fpID to addstudentwindow.cpp
/// @param fpid 
void MainWindow::onFPIDAddReceived(QString fpid){
    qDebug()<<"onFPIDAddReceived emit";
    connect(this, &MainWindow::passFpID, sWindow, 
    [=](const QVariant& value) {
        sWindow->receiveFPID(value.toString());
    }, Qt::QueuedConnection);
    // Emit the signal with the variable
    emit passFpID(fpid);        
    return;

}

/// @brief Trigger Door Control function
void MainWindow::doorControl(){
    while(running){
        if(door){
            doorlock.run();
            door = false;
        }
    }

}
