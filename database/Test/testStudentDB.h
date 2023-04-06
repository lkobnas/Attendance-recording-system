#ifndef TESTSTUDENTDB_H
#define TESTSTUDENTDB_H

#include "studentDB.h"
#include "studentDB.cpp"
#include <gtest/gtest.h>

StudentDB sdb;
TEST(studentDB_test, studentDB_init) {

    EXPECT_EQ(sdb.initDB(),true);
}

TEST(studentDB_test, studentDB_insertStudent) {
    EXPECT_EQ(sdb.insertStudent("9553252Q","John Wick","9553252Q@student.gla.ac.uk","5A-8D-B3-33-3A","picturedata266501","005391582938538479183749187349183749817498317491873948174981798374198DAFADFAFHDKJHAKHkfdjahdfkjahfkdjahdfkuakeaf931"),true); 
    EXPECT_EQ(sdb.insertStudent("9553252Q","John Wick","9553252Q@student.gla.ac.uk","5A-8D-B3-33-3A","picturedata266501","005391582938538479183749187349183749817498317491873948174981798374198DAFADFAFHDKJHAKHkfdjahdfkjahfkdjahdfkuakeaf931"),false); 
    EXPECT_EQ(sdb.insertStudent("3559320A","Test Name2","3559320A@student.gla.ac.uk","9C-3A-92-D3-2B","picturedata9957401","178035125132183749187349183749817498317491873948174981798374198DAFADFAFHDKJHAKHkfdjahdfkjahfkdjahdfkuakeaf931"),true);
    EXPECT_EQ(sdb.insertStudent("2426734L","A","email@email.com","cardID","",""),true);
    EXPECT_EQ(sdb.insertStudent("2993326W","B C","2993326W@student.gla.ac.uk","cardIDDD","",""),true);

}

TEST(studentDB_test, studentDB_checkStudentExist) {
    EXPECT_EQ(sdb.checkStudentExist("3559320A"),true);
    EXPECT_EQ(sdb.checkStudentExist("9553252Q"),true);
    EXPECT_EQ(sdb.checkStudentExist("NOTEXIST"),false);
    EXPECT_EQ(sdb.checkStudentExist("2993326W"),true);

}

TEST(studentDB_test, studentDB_getStudent) {
    EXPECT_EQ(sdb.getStudent("3559320A").sid,"3559320A");
    EXPECT_EQ(sdb.getStudent("9553252Q").email,"9553252Q@student.gla.ac.uk");
    //EXPECT
    EXPECT_EQ(sdb.getStudent("NOTEXIST").name,"");
}

TEST(studentDB_test, studentDB_deleteStudent) {
    EXPECT_EQ(sdb.deleteStudent("3559320A"),true);
    EXPECT_EQ(sdb.deleteStudent("NOTEXIST"),false);

}

TEST(studentDB_test, studentDB_getAllStudent) {
    QList<Student> studentlist;
    EXPECT_NO_FATAL_FAILURE(studentlist = sdb.getAllStudents());
    EXPECT_EQ(studentlist[0].cardId,"5A-8D-B3-33-3A");
    EXPECT_EQ(studentlist[0].fpId,"005391582938538479183749187349183749817498317491873948174981798374198DAFADFAFHDKJHAKHkfdjahdfkjahfkdjahdfkuakeaf931");
    EXPECT_EQ(studentlist[0].picture,"picturedata266501");
    EXPECT_EQ(studentlist[1].picture,"");
    
}


#endif