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

TEST_F(ManagerTest, CreateVarWorks)
{
    ClassProject::BDD_ID aVarId = manager.createVar("a");
    EXPECT_EQ(aVarId, 2);
    manager.createVar("b");
    ClassProject::BDD_ID cVarId = manager.createVar("c");
    EXPECT_EQ(cVarId, 4);
}

TEST_F(ManagerTest, TrueAndFalseConstantsWork)
{
    EXPECT_EQ(manager.True(), ClassProject::TRUE_ID);
    EXPECT_EQ(manager.False(), ClassProject::FALSE_ID);
}

TEST_F(ManagerTest, uniqueTableSizeWorks)
{
    EXPECT_EQ(manager.uniqueTableSize(), 2);

    manager.createVar("a");
    manager.createVar("b");

    EXPECT_EQ(manager.uniqueTableSize(), 4);
}

#endif
