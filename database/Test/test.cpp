#include <gtest/gtest.h>

#include "testStudentDB.h"
#include "testCourseDB.h"


int main(int argc, char** argv) {
    std::remove("students.db");
    std::remove("courses.db");
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
