#include "studentDB.h"
#include "courseDB.h"
#include <iostream>
int main(){
    //// Testing students.db
    StudentDB db;
    db.initDB();
    db.insertStudent("2426734L","Test Name1","2426734L@student.gla.ac.uk","6F-8D-B3-33-3A","picturedata303103501","23147938749187384179238479183749187349183749817498317491873948174981798374198DAFADFAFHDKJHAKHkfdjahdfkjahfkdjahdfkuakeaf931");
    // //db.deleteStudent("2426734L");
    //Student student = db.getStudent("2426734L");
    // std::cout<<student.name.toStdString()+" "+student.fpId.toStdString()<<std::endl;
    QList<Student> studentlist = db.getAllStudents();
    // std::cout<<studentlist[0].name.toStdString()<<std::endl;

    // Testing courses.db
    CourseDB dbc;
    dbc.initDB();
    dbc.insertCourse("ENG5220","4/4/2023 15:30", studentlist);
    //db.deleteStudent("2426734L");
    Course course = dbc.getCourse("ENG5220");
    std::cout<<course.name.toStdString()+" "+course.studentList[0].name.toStdString()<<std::endl;
    QList<Course> courselist = dbc.getAllCourses();
    std::cout<<courselist[0].name.toStdString()<<std::endl;

    return 0;
    
}
