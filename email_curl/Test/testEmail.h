#ifndef TESTEMAIL_H
#define TESTEMAIL_H

#include "email.h"
#include "email.cpp"

#include <gtest/gtest.h>

Email email;

TEST(email_test, readCredentialsFile) {

    EXPECT_EQ(email.readCredentialsFile(), true);
}
TEST(email_test, send_email) {
    EXPECT_EQ(email.send_email("eng5220.group38@gmail.com",1), true);
    EXPECT_EQ(email.send_email("eng5220.group38@gmail.com",2), true);
}


#endif