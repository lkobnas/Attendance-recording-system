#include "studentDB.h"
#include "courseDB.h"
#include <iostream>
int main(){
    //// Testing students.db
    StudentDB db;
    db.initDB();
    db.insertStudent("2993326W","Test Name1","2993326W@student.gla.ac.uk","1F-8D-B3-33-3A","picturedata233501","2314735829582938538479183749187349183749817498317491873948174981798374198DAFADFAFHDKJHAKHkfdjahdfkjahfkdjahdfkuakeaf931");
    db.insertStudent("3559320A","Test Name2","3559320A@student.gla.ac.uk","9C-3A-92-D3-2B","picturedata9957401","178035125132183749187349183749817498317491873948174981798374198DAFADFAFHDKJHAKHkfdjahdfkjahfkdjahdfkuakeaf931");
    // //db.deleteStudent("2426734L");
    //Student student = db.getStudent("2426734L");
    // std::cout<<student.name.toStdString()+" "+student.fpId.toStdString()<<std::endl;
    QList<Student> studentlist = db.getAllStudents();
    // std::cout<<studentlist[0].name.toStdString()<<std::endl;

    // Testing courses.db
    CourseDB dbc;
    dbc.initDB();
    dbc.insertCourse("ENG5220","4/4/2023 15:30", "2993326W,  2426734L  ,   3559320A");
    //dbc.deleteCourse("ENG5220");
    //Course course = dbc.getCourse("ENG5220");
    //std::cout<<course.name.toStdString()+" "+course.studentList[0].name.toStdString()<<std::endl;
    //QList<Course> courselist = dbc.getAllCourses();
    //std::cout<<courselist[0].name.toStdString()<<std::endl;

    return 0;
    
}
