#include "studentDB.h"
#include <iostream>
int main(){
    StudentDB db;
    db.initDB();
    //db.insertStudent("Test Name2","2426777L@student.gla.ac.uk","6F-8D-B3-33-3A","23147938749187384179238479183749187349183749817498317491873948174981798374198DAFADFAFHDKJHAKHkfdjahdfkjahfkdjahdfkuakeaf931");
    //db.deleteStudent("1");
    Student student = db.getStudent("1");
    std::cout<<student.name.toStdString()+" "+student.fpId.toStdString()<<std::endl;
    QList<Student> list = db.getAllStudents();
    std::cout<<list[1].name.toStdString()<<std::endl;
    return 0;
    
}
