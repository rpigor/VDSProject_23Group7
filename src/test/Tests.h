//
// Created by tobias on 21.11.16.
//

#ifndef VDSPROJECT_TESTS_H
#define VDSPROJECT_TESTS_H

#include <gtest/gtest.h>
#include "../Manager.h"

class ManagerTest : public testing::Test {
protected:
    ClassProject::Manager manager;
};

TEST_F(ManagerTest, TrueAndFalseConstantsWork)
{
    EXPECT_EQ(manager.True(), ClassProject::TRUE_ID);
    EXPECT_EQ(manager.False(), ClassProject::FALSE_ID);
}

#endif
