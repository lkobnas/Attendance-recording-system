#ifndef TESTEMAIL_H
#define TESTEMAIL_H

#include "email.h"
#include "email.cpp"

#include <gtest/gtest.h>

Email email;

TEST(email_test, readCredentialsFile) {

    EXPECT_EQ(email.readCredentialsFile(), true);
}
TEST(email_test, send_email_record) {
    EXPECT_EQ(email.send_email_record("eng5220.group38@gmail.com","ENG5220"), true);
}
TEST(email_test, send_email_newStudent) {
    EXPECT_EQ(email.send_email_newStudent("eng5220.group38@gmail.com"), true);
}
TEST(email_test, send_email_enrollToCourse) {
    EXPECT_EQ(email.send_email_enrollToCourse("eng5220.group38@gmail.com","ENG5220","04/05/2023 13:50"), true);
}
TEST(email_test, send_email_lateReminder) {
    EXPECT_EQ(email.send_email_lateReminder("eng5220.group38@gmail.com","ENG5220","04/05/2023 13:50"), true);
}

#endif