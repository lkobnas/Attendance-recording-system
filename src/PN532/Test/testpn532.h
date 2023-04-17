#ifndef TESTPN532_H
#define TESTPN532_H

#include "../rfid.h"
#include "../rfid.cpp"
#include <gtest/gtest.h>

RFID pn532;
TEST(RFID_test, RFID_getid) {

    EXPECT_EQ(pn532.get_uid(),true);
}


#endif