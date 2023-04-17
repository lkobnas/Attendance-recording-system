#include "addstudentwindow.h"
#include "ui_addstudentwindow.h"


/// @brief AddStudentWindow Constructor
/// @param parent Passing parent object
AddStudentWindow::AddStudentWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddStudentWindow)
{
    sdb.initDB();
    ui->setupUi(this);
    setWindowTitle("Add New Student");
}

/// @brief AddStudentWindow Destructor
AddStudentWindow::~AddStudentWindow()
{
    delete ui;
}

/// @brief Clear all input fields
void AddStudentWindow::on_sClearButton_clicked()
{
    ui->sNameLineEdit->clear();
    ui->sEmailLineEdit->clear();
    ui->sSIDLineEdit->clear();
}

/// @brief Received student cardID from MainWindow, change button to green when succeed
/// @param cardid is the student cardID
void AddStudentWindow::receiveCardID(QString cardid)
{
    qDebug()<< "receiveCardID: "<<cardid;
    cardID = cardid;
    QPalette pal = ui->sCardButton->palette();
    pal.setColor(QPalette::Button, QColor(Qt::green));
    ui->sCardButton->setAutoFillBackground(true);
    ui->sCardButton->setPalette(pal);
    ui->sCardButton->update();
}

/// @brief Received student fingerprint from MainWindow, change button to green when succeed
/// @param fpid 
void AddStudentWindow::receiveFPID(QString fpid)
/// @brief add student card button
{
    qDebug()<< "receiveFPID: "<<fpid;
    fpID = fpid;
    QPalette pal = ui->sFpButton->palette();
    pal.setColor(QPalette::Button, QColor(Qt::green));
    ui->sFpButton->setAutoFillBackground(true);
    ui->sFpButton->setPalette(pal);
    ui->sFpButton->update();
}


/// @brief Show guidelines when card button is clicked
void AddStudentWindow::on_sCardButton_clicked()
{
    QMessageBox::information(this, "Scan your card", "Place your card on the card reader until this button turn green");
}


/// @brief Show guidelines when fingerprint button is clicked
void AddStudentWindow::on_sFpButton_clicked()
{
    QMessageBox::information(this, "Scan your finger", "Gently place your finger on the fingerprint sensor until the button turn green");
}


/// @brief Validate all input fields and add student to database
void AddStudentWindow::on_sAddButton_clicked()
{

    name = ui->sNameLineEdit->text();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Error adding student", "Student name cannot be empty.");
        return;
    } 
    email = ui->sEmailLineEdit->text();
    if (email.isEmpty()) {
        QMessageBox::warning(this, "Error adding student", "Student email cannot be empty.");
        return;
    } 
    sid = ui->sSIDLineEdit->text();
    if (sid.isEmpty()) {
        QMessageBox::warning(this, "Error adding student", "Student ID cannot be empty.");
        return;
    } 
    if(cardID.isEmpty()){
        QMessageBox::warning(this, "Error adding student", "Student card have not registered");
        return;
    }
    if(fpID.isEmpty()){
        QMessageBox::warning(this, "No fingerprint detected", "Fingerprint is not registered");
    }    

    photo = "photoDataHere";
    // Insert new student data to database
    try{
        sdb.insertStudent(sid,name,email,cardID,photo,fpID);
    }catch(QException &e){
        const MyException* myException = dynamic_cast<const MyException*>(&e);
        if (myException) {
            QString errorMessage = myException->message();
            QMessageBox::warning(this, "Error adding student", errorMessage);
            return;
        }
    }
    email_curl.send_email_newStudent(email.toStdString());
    QMessageBox::information(this, "Insert successful", "New student : " +name+ " , added to PunchIN");
    close();

return;
}


/// @brief Close add student window
void AddStudentWindow::on_pushButton_clicked()
{
    this->close();
}

