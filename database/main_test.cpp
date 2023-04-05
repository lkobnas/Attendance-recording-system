#include "studentDB.h"
#include "courseDB.h"
#include <iostream>
int main(){
    //// Testing students.db
    StudentDB db;
    db.initDB();
    db.insertStudent("9553252Q","John Wick","9553252Q@student.gla.ac.uk","5A-8D-B3-33-3A","picturedata266501","005391582938538479183749187349183749817498317491873948174981798374198DAFADFAFHDKJHAKHkfdjahdfkjahfkdjahdfkuakeaf931");
    //db.insertStudent("3559320A","Test Name2","3559320A@student.gla.ac.uk","9C-3A-92-D3-2B","picturedata9957401","178035125132183749187349183749817498317491873948174981798374198DAFADFAFHDKJHAKHkfdjahdfkjahfkdjahdfkuakeaf931");
    // //db.deleteStudent("2426734L");
    //Student student = db.getStudent("2426734L");
    // std::cout<<student.name.toStdString()+" "+student.fpId.toStdString()<<std::endl;
    //QList<Student> studentlist = db.getAllStudents();
    // std::cout<<studentlist[0].name.toStdString()<<std::endl;

    // Testing courses.db
    CourseDB dbc;
    dbc.initDB();
    //dbc.insertCourse("ENG5220","4/4/2023 15:30", "2993326W,  2426734L,   3559320A");
    dbc.insertCourse("ENG5220","4/4/2023 15:30", "      9553252Q,   3559320A");
    dbc.updateArrived("ENG5220", "notexistID");
    dbc.updateArrived("ENG5220", "2426734L");
    dbc.updateArrived("ENG5220", "2993326W");
    dbc.updateArrived("ENG5220", "3559320A");
    //dbc.deleteCourse("ENG5220");
    //Course course = dbc.getCourse("ENG5220");
    //std::cout<<course.name.toStdString()+" "+course.studentList[0].name.toStdString()<<std::endl;
    //QList<Course> courselist = dbc.getAllCourses();
    //std::cout<<courselist[0].name.toStdString()<<std::endl;

    return 0;
    
}
