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

TEST_F(ManagerTest, IsConstantWorks)
{
    EXPECT_TRUE(manager.isConstant(ClassProject::TRUE_ID));
    EXPECT_TRUE(manager.isConstant(ClassProject::FALSE_ID));

    ClassProject::BDD_ID aVarId = manager.createVar("a");
    ClassProject::BDD_ID bVarId = manager.createVar("b");
    EXPECT_FALSE(manager.isConstant(aVarId));
    EXPECT_FALSE(manager.isConstant(bVarId));
}

TEST_F(ManagerTest, IsVariableWorks)
{
    EXPECT_FALSE(manager.isVariable(ClassProject::TRUE_ID));
    EXPECT_FALSE(manager.isVariable(ClassProject::FALSE_ID));

    ClassProject::BDD_ID aVarId = manager.createVar("a");
    ClassProject::BDD_ID bVarId = manager.createVar("b");
    EXPECT_TRUE(manager.isVariable(aVarId));
    EXPECT_TRUE(manager.isVariable(bVarId));
}

TEST_F(ManagerTest, TopVarWorks)
{
    EXPECT_EQ(manager.topVar(ClassProject::TRUE_ID), ClassProject::TRUE_ID);
    EXPECT_EQ(manager.topVar(ClassProject::FALSE_ID), ClassProject::FALSE_ID);

    ClassProject::BDD_ID aVarId = manager.createVar("a");
    EXPECT_EQ(manager.topVar(aVarId), aVarId);
}

TEST_F(ManagerTest, uniqueTableSizeWorks)
{
    EXPECT_EQ(manager.uniqueTableSize(), 2);

    manager.createVar("a");
    manager.createVar("b");

    EXPECT_EQ(manager.uniqueTableSize(), 4);
}

#endif
