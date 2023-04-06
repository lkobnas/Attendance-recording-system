#ifndef TESTCOURSEDB_H
#define TESTCOURSEDB_H

#include <gtest/gtest.h>
#include "courseDB.h"
#include "courseDB.cpp"
CourseDB cdb;

TEST(courseDB_test, courseDB_init) {
    EXPECT_EQ(cdb.initDB(),true);
}

TEST(courseDB_test, courseDB_insertCourse) {
    EXPECT_EQ(cdb.insertCourse("ENG5220","4/4/2023 15:30", "   2993326W,  2426734L,   3559320A"),true);
    EXPECT_EQ(cdb.insertCourse("ENG5220","4/4/2023 15:30", "      9553252Q,   3559320A"),false);
    EXPECT_EQ(cdb.insertCourse("ENG5220","4/4/2023 15:30", "    9553252Q, 3559320A"),false);
    EXPECT_EQ(cdb.insertCourse("ENG5330","7/4/2023 12:30", "2993326W,  2426734L,   3559320A"),true);
    EXPECT_EQ(cdb.insertCourse("ENG5330","7/4/2023 12:30", ""),false);

}

TEST(courseDB_test, courseDB_updateArrivedList) {
    EXPECT_EQ(cdb.updateArrived("ENG5220","2993326W"),true);
    EXPECT_EQ(cdb.updateArrived("ENG5220","2993326W"),false);
    EXPECT_EQ(cdb.updateArrived("ENG5220","NOTEXIST"),false);
    EXPECT_EQ(cdb.updateArrived("ENG5220","2426734L"),true);
    EXPECT_EQ(cdb.updateArrived("ENG5330","2993326W"),true);
    EXPECT_EQ(cdb.updateArrived("ENG5330","2993326W"),false);
}

TEST(courseDB_test, courseDB_getCourse) {
    EXPECT_EQ(cdb.getCourse("ENG5220").name,"ENG5220");
    EXPECT_EQ(cdb.getCourse("ENG5220").datetime,"4/4/2023 15:30");
    EXPECT_EQ(cdb.getCourse("ENG5330").name,"ENG5330");
    EXPECT_EQ(cdb.getCourse("ENG5330").datetime,"7/4/2023 12:30");
    EXPECT_EQ(cdb.getCourse("ENG5220").studentList.size(),1);
    EXPECT_EQ(cdb.getCourse("ENG5220").arrivedStudents.size(),2);
    EXPECT_EQ(cdb.getCourse("ENG5330").studentList.size(),1);
    EXPECT_EQ(cdb.getCourse("ENG5330").arrivedStudents.size(),1);
}

TEST(courseDB_test, courseDB_deleteCourse) {
    EXPECT_EQ(cdb.deleteCourse("ENG5330"),true);
    EXPECT_EQ(cdb.deleteCourse("NOTEXIST"),true);   //Delete non-existing course will never happen, therefore check function is not needed

}

TEST(courseDB_test, courseDB_getAllCourse) {
    QList<Course> courselist;
    EXPECT_NO_FATAL_FAILURE(courselist = cdb.getAllCourses());
    EXPECT_EQ(courselist[0].name,"ENG5220");
    EXPECT_EQ(courselist[0].datetime,"4/4/2023 15:30");
    EXPECT_EQ(courselist[0].arrivedStudents.size(),2);
    EXPECT_EQ(courselist[0].studentList.size(),1);
    
}


#endif