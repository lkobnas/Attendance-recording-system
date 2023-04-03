#include "studentDB.h"
#include <iostream>
int main(){
    StudentDB db;
    db.initDB();
    //db.insertStudent("2426734L","Test Name1","2426734L@student.gla.ac.uk","6F-8D-B3-33-3A","picturedata303103501","23147938749187384179238479183749187349183749817498317491873948174981798374198DAFADFAFHDKJHAKHkfdjahdfkjahfkdjahdfkuakeaf931");
    //db.deleteStudent("2426734L");
    Student student = db.getStudent("2426734L");
    std::cout<<student.name.toStdString()+" "+student.fpId.toStdString()<<std::endl;
    QList<Student> list = db.getAllStudents();
    std::cout<<list[0].name.toStdString()<<std::endl;
    return 0;
    
}
