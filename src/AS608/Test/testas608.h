#ifndef TESTAS608_H
#define TESTAS608_H

#include "../fingerprint.h"
#include "../fingerprint.cpp"
#include <gtest/gtest.h>

Fingerprint as608;
TEST(Fingerprint_test, Fingerprint_fucntion) {

    EXPECT_EQ(pn532.get_uid(),true);
}


#endif