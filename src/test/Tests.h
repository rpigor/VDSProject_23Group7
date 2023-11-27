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

TEST_F(ManagerTest, IteWorks)
{
    ClassProject::BDD_ID aVarId = manager.createVar("a");
    ClassProject::BDD_ID bVarId = manager.createVar("b");

    EXPECT_EQ(manager.getTopVarName(bVarId), "b");

    EXPECT_EQ(manager.ite(aVarId, ClassProject::TRUE_ID, ClassProject::FALSE_ID), aVarId);
    EXPECT_EQ(manager.ite(ClassProject::TRUE_ID, ClassProject::FALSE_ID, bVarId), ClassProject::FALSE_ID);
    EXPECT_EQ(manager.ite(ClassProject::FALSE_ID, ClassProject::TRUE_ID, ClassProject::TRUE_ID), ClassProject::TRUE_ID);

    ClassProject::BDD_ID negAId = manager.ite(aVarId, ClassProject::FALSE_ID, ClassProject::TRUE_ID);
    EXPECT_EQ(manager.coFactorTrue(negAId), ClassProject::TRUE_ID);
    EXPECT_EQ(manager.coFactorFalse(negAId), ClassProject::FALSE_ID);
    EXPECT_EQ(manager.getTopVarName(negAId), "a");

    ClassProject::BDD_ID orABId = manager.ite(aVarId, ClassProject::TRUE_ID, bVarId);
    EXPECT_EQ(manager.coFactorTrue(orABId), ClassProject::TRUE_ID);
    EXPECT_EQ(manager.coFactorTrue(orABId, bVarId), ClassProject::TRUE_ID);
    EXPECT_EQ(manager.coFactorFalse(orABId), bVarId);
    EXPECT_EQ(manager.coFactorFalse(orABId, bVarId), aVarId);
    EXPECT_EQ(manager.getTopVarName(orABId), "a");

    ClassProject::BDD_ID cVarId = manager.createVar("c");
    ClassProject::BDD_ID andCOrABId = manager.ite(orABId, cVarId, ClassProject::FALSE_ID);
    EXPECT_EQ(manager.coFactorTrue(andCOrABId), cVarId);
    EXPECT_EQ(manager.coFactorTrue(andCOrABId, cVarId), orABId);
    EXPECT_EQ(manager.coFactorFalse(andCOrABId, cVarId), ClassProject::FALSE_ID);
    EXPECT_EQ(manager.getTopVarName(andCOrABId), "a");
}

TEST_F(ManagerTest, CoFactorTrueWorks)
{
    ClassProject::BDD_ID aVarId = manager.createVar("a");
    ClassProject::BDD_ID bVarId = manager.createVar("b");

    EXPECT_EQ(manager.coFactorTrue(ClassProject::TRUE_ID), ClassProject::TRUE_ID);
    EXPECT_EQ(manager.coFactorTrue(ClassProject::FALSE_ID, ClassProject::TRUE_ID), ClassProject::FALSE_ID);
    EXPECT_EQ(manager.coFactorTrue(ClassProject::TRUE_ID, aVarId), ClassProject::TRUE_ID);
    EXPECT_EQ(manager.coFactorTrue(ClassProject::FALSE_ID, bVarId), ClassProject::FALSE_ID);
    EXPECT_EQ(manager.coFactorTrue(aVarId, aVarId), ClassProject::TRUE_ID);
    EXPECT_EQ(manager.coFactorTrue(bVarId, aVarId), bVarId);
    EXPECT_EQ(manager.coFactorTrue(aVarId, ClassProject::FALSE_ID), aVarId);

    ClassProject::BDD_ID andABId = manager.ite(aVarId, bVarId, ClassProject::FALSE_ID);
    EXPECT_EQ(manager.coFactorTrue(andABId), bVarId);
    EXPECT_EQ(manager.coFactorTrue(andABId, bVarId), aVarId);

    ClassProject::BDD_ID orABId = manager.ite(aVarId, ClassProject::TRUE_ID, bVarId);
    EXPECT_EQ(manager.coFactorTrue(orABId), ClassProject::TRUE_ID);
    EXPECT_EQ(manager.coFactorTrue(orABId, bVarId), ClassProject::TRUE_ID);

}

TEST_F(ManagerTest, CoFactorFalseWorks)
{
    ClassProject::BDD_ID aVarId = manager.createVar("a");
    ClassProject::BDD_ID bVarId = manager.createVar("b");

    EXPECT_EQ(manager.coFactorFalse(ClassProject::TRUE_ID), ClassProject::TRUE_ID);
    EXPECT_EQ(manager.coFactorFalse(ClassProject::FALSE_ID, ClassProject::TRUE_ID), ClassProject::FALSE_ID);
    EXPECT_EQ(manager.coFactorFalse(ClassProject::TRUE_ID, aVarId), ClassProject::TRUE_ID);
    EXPECT_EQ(manager.coFactorFalse(ClassProject::FALSE_ID, bVarId), ClassProject::FALSE_ID);
    EXPECT_EQ(manager.coFactorFalse(aVarId, aVarId), ClassProject::FALSE_ID);
    EXPECT_EQ(manager.coFactorFalse(bVarId, aVarId), bVarId);
    EXPECT_EQ(manager.coFactorFalse(aVarId, ClassProject::FALSE_ID), aVarId);

    ClassProject::BDD_ID andABId = manager.ite(aVarId, bVarId, ClassProject::FALSE_ID);
    EXPECT_EQ(manager.coFactorFalse(andABId), ClassProject::FALSE_ID);
    EXPECT_EQ(manager.coFactorFalse(andABId, bVarId), ClassProject::FALSE_ID);

    ClassProject::BDD_ID orABId = manager.ite(aVarId, ClassProject::TRUE_ID, bVarId);
    EXPECT_EQ(manager.coFactorFalse(orABId), bVarId);
    EXPECT_EQ(manager.coFactorFalse(orABId, bVarId), aVarId);
}

TEST_F(ManagerTest, And2Works)
{
    ClassProject::BDD_ID aVarId = manager.createVar("a");
    ClassProject::BDD_ID bVarId = manager.createVar("b");
    ClassProject::BDD_ID andABId = manager.and2(aVarId, bVarId);
    ClassProject::BDD_ID cVarId = manager.createVar("c");
    ClassProject::BDD_ID andBCVarId = manager.and2(bVarId, cVarId);
    ClassProject::BDD_ID andCAndABId = manager.and2(cVarId, andABId);

    EXPECT_EQ(manager.coFactorTrue(andABId), bVarId);
    EXPECT_EQ(manager.coFactorFalse(andABId), ClassProject::FALSE_ID);
    EXPECT_EQ(manager.coFactorTrue(andBCVarId, cVarId), bVarId);
    EXPECT_EQ(manager.coFactorTrue(andCAndABId), andBCVarId);
    EXPECT_EQ(manager.coFactorFalse(andCAndABId), ClassProject::FALSE_ID);
}

TEST_F(ManagerTest, Or2Works)
{
    ClassProject::BDD_ID aVarId = manager.createVar("a");
    ClassProject::BDD_ID bVarId = manager.createVar("b");
    ClassProject::BDD_ID orABId = manager.or2(aVarId, bVarId);
    ClassProject::BDD_ID cVarId = manager.createVar("c");
    ClassProject::BDD_ID orBCId = manager.or2(bVarId, cVarId);
    ClassProject::BDD_ID orCOrABId = manager.or2(cVarId, orABId);

    EXPECT_EQ(manager.coFactorTrue(orABId), ClassProject::TRUE_ID);
    EXPECT_EQ(manager.coFactorFalse(orABId), bVarId);
    EXPECT_EQ(manager.coFactorFalse(orBCId, cVarId), bVarId);
    EXPECT_EQ(manager.coFactorTrue(orCOrABId), ClassProject::TRUE_ID);
    EXPECT_EQ(manager.coFactorFalse(orCOrABId), orBCId);
}

TEST_F(ManagerTest, Xor2Works)
{
    ClassProject::BDD_ID aVarId = manager.createVar("a");
    ClassProject::BDD_ID bVarId = manager.createVar("b");
    ClassProject::BDD_ID xorABId = manager.xor2(aVarId, bVarId);
    ClassProject::BDD_ID negBId = manager.ite(bVarId, ClassProject::FALSE_ID, ClassProject::TRUE_ID);

    EXPECT_EQ(manager.coFactorFalse(manager.coFactorFalse(xorABId)), ClassProject::FALSE_ID);
    EXPECT_EQ(manager.coFactorFalse(xorABId), bVarId);
    EXPECT_EQ(manager.coFactorTrue(xorABId), negBId);
    EXPECT_EQ(manager.coFactorTrue(manager.coFactorTrue(xorABId)), ClassProject::FALSE_ID);
}

TEST_F(ManagerTest, NegWorks)
{
    ClassProject::BDD_ID aVarId = manager.createVar("a");
    ClassProject::BDD_ID negAId = manager.neg(aVarId);
    ClassProject::BDD_ID negNegAId = manager.neg(negAId);

    EXPECT_EQ(manager.coFactorTrue(negAId), ClassProject::FALSE_ID);
    EXPECT_EQ(manager.coFactorFalse(negAId), ClassProject::TRUE_ID);
    EXPECT_EQ(aVarId, negNegAId);
}

TEST_F(ManagerTest, GetTopVarNameWorks)
{
    ClassProject::BDD_ID aVarId = manager.createVar("a");
    ClassProject::BDD_ID bVarId = manager.createVar("b");

    EXPECT_EQ(manager.getTopVarName(aVarId), "a");
    EXPECT_EQ(manager.getTopVarName(bVarId), "b");
}

TEST_F(ManagerTest, FindNodesWorks)
{
    std::set<ClassProject::BDD_ID> nodes;
    manager.createVar("a");
    ClassProject::BDD_ID bVarId = manager.createVar("b");
    manager.findNodes(bVarId, nodes);
    std::set<ClassProject::BDD_ID> groundTruthNodes = {bVarId, ClassProject::TRUE_ID, ClassProject::FALSE_ID};

    EXPECT_EQ(nodes, groundTruthNodes);
}

TEST_F(ManagerTest, uniqueTableSizeWorks)
{
    EXPECT_EQ(manager.uniqueTableSize(), 2);

    manager.createVar("a");
    manager.createVar("b");

    EXPECT_EQ(manager.uniqueTableSize(), 4);
}

#endif
