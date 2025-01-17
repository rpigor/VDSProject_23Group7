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

class ManagerExampleTest : public testing::Test {
protected:

    ClassProject::Manager manager;

    ClassProject::BDD_ID functionId;

    ClassProject::BDD_ID orABId;

    ClassProject::BDD_ID andCDId;

    void SetUp() override
    {
        auto aVarId = manager.createVar("a");
        auto bVarId = manager.createVar("b");
        auto cVarId = manager.createVar("c");
        auto dVarId = manager.createVar("d");

        orABId = manager.or2(aVarId, bVarId);
        andCDId = manager.and2(cVarId, dVarId);
        functionId = manager.and2(orABId, andCDId);
    }
};

TEST_F(ManagerTest, CreateVarWorks)
{
    ClassProject::BDD_ID aVarId = manager.createVar("a");
    EXPECT_EQ(aVarId, 2);
    EXPECT_EQ(manager.coFactorTrue(aVarId), ClassProject::TRUE_ID);

    manager.createVar("b");

    ClassProject::BDD_ID cVarId = manager.createVar("c");
    EXPECT_EQ(cVarId, 4);
    EXPECT_EQ(manager.coFactorFalse(cVarId), ClassProject::FALSE_ID);
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
    ClassProject::BDD_ID andABId = manager.and2(aVarId, bVarId);

    EXPECT_FALSE(manager.isConstant(aVarId));
    EXPECT_FALSE(manager.isConstant(bVarId));
    EXPECT_FALSE(manager.isConstant(andABId));
}

TEST_F(ManagerTest, IsVariableWorks)
{
    EXPECT_FALSE(manager.isVariable(ClassProject::TRUE_ID));
    EXPECT_FALSE(manager.isVariable(ClassProject::FALSE_ID));

    ClassProject::BDD_ID aVarId = manager.createVar("a");
    ClassProject::BDD_ID bVarId = manager.createVar("b");
    ClassProject::BDD_ID negBId = manager.neg(bVarId);
    ClassProject::BDD_ID orABId = manager.or2(aVarId, bVarId);

    EXPECT_TRUE(manager.isVariable(aVarId));
    EXPECT_TRUE(manager.isVariable(bVarId));
    EXPECT_FALSE(manager.isVariable(negBId));
    EXPECT_FALSE(manager.isVariable(orABId));
}

TEST_F(ManagerTest, TopVarWorks)
{
    // topVar of constant node doesn't really make sense; it's dependend on implementation
    // EXPECT_EQ(manager.topVar(ClassProject::TRUE_ID), ClassProject::TRUE_ID);
    // EXPECT_EQ(manager.topVar(ClassProject::FALSE_ID), ClassProject::FALSE_ID);

    ClassProject::BDD_ID aVarId = manager.createVar("a");
    ClassProject::BDD_ID bVarId = manager.createVar("b");
    ClassProject::BDD_ID cVarId = manager.createVar("c");
    ClassProject::BDD_ID andBCId = manager.and2(bVarId, cVarId);
    ClassProject::BDD_ID orCCId = manager.or2(cVarId, cVarId);

    EXPECT_EQ(manager.topVar(aVarId), aVarId);
    EXPECT_EQ(manager.topVar(andBCId), bVarId);
    EXPECT_EQ(manager.topVar(orCCId), cVarId);
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
    EXPECT_EQ(manager.coFactorTrue(negAId), ClassProject::FALSE_ID);
    EXPECT_EQ(manager.coFactorFalse(negAId), ClassProject::TRUE_ID);
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

    ClassProject::BDD_ID andABId = manager.ite(aVarId, bVarId, ClassProject::FALSE_ID);
    ClassProject::BDD_ID bVarWithNodeMergingId = manager.ite(aVarId, andABId, orABId);
    ClassProject::BDD_ID negBId = manager.neg(bVarId);
    ClassProject::BDD_ID negBWithNodemergingId = manager.neg(bVarWithNodeMergingId);
    EXPECT_EQ(bVarWithNodeMergingId, bVarId);
    EXPECT_EQ(negBWithNodemergingId, negBId);
    EXPECT_EQ(manager.coFactorTrue(bVarWithNodeMergingId), ClassProject::TRUE_ID);
    EXPECT_EQ(manager.coFactorTrue(negBWithNodemergingId), ClassProject::FALSE_ID);
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
    ClassProject::BDD_ID iteNegAId = manager.ite(aVarId, ClassProject::FALSE_ID, ClassProject::TRUE_ID);

    EXPECT_EQ(manager.coFactorTrue(negAId), ClassProject::FALSE_ID);
    EXPECT_EQ(manager.coFactorFalse(negAId), ClassProject::TRUE_ID);
    EXPECT_EQ(aVarId, negNegAId);
    EXPECT_EQ(negAId, iteNegAId);
}

TEST_F(ManagerTest, Nand2Works)
{
    ClassProject::BDD_ID aVarId = manager.createVar("a");
    ClassProject::BDD_ID bVarId = manager.createVar("b");
    ClassProject::BDD_ID negBId = manager.neg(bVarId);
    ClassProject::BDD_ID nandABId = manager.nand2(aVarId, bVarId);

    EXPECT_EQ(manager.coFactorTrue(manager.coFactorTrue(nandABId)), ClassProject::FALSE_ID);
    EXPECT_EQ(manager.coFactorTrue(nandABId), negBId);
    EXPECT_EQ(manager.coFactorFalse(nandABId), ClassProject::TRUE_ID);
    EXPECT_EQ(manager.coFactorFalse(manager.coFactorFalse(nandABId)), ClassProject::TRUE_ID);
}

TEST_F(ManagerTest, Nor2Works)
{
    ClassProject::BDD_ID aVarId = manager.createVar("a");
    ClassProject::BDD_ID bVarId = manager.createVar("b");
    ClassProject::BDD_ID negBId = manager.neg(bVarId);
    ClassProject::BDD_ID norABId = manager.nor2(aVarId, bVarId);

    EXPECT_EQ(manager.coFactorFalse(manager.coFactorFalse(norABId)), ClassProject::TRUE_ID);
    EXPECT_EQ(manager.coFactorTrue(norABId), ClassProject::FALSE_ID);
    EXPECT_EQ(manager.coFactorFalse(norABId), negBId);
    EXPECT_EQ(manager.coFactorTrue(manager.coFactorTrue(norABId)), ClassProject::FALSE_ID);
}

TEST_F(ManagerTest, Xnor2Works)
{
    ClassProject::BDD_ID aVarId = manager.createVar("a");
    ClassProject::BDD_ID bVarId = manager.createVar("b");
    ClassProject::BDD_ID negBId = manager.neg(bVarId);
    ClassProject::BDD_ID xnorABId = manager.xnor2(aVarId, bVarId);

    EXPECT_EQ(manager.coFactorTrue(manager.coFactorTrue(xnorABId)), ClassProject::TRUE_ID);
    EXPECT_EQ(manager.coFactorTrue(xnorABId), bVarId);
    EXPECT_EQ(manager.coFactorFalse(xnorABId), negBId);
    EXPECT_EQ(manager.coFactorFalse(manager.coFactorFalse(xnorABId)), ClassProject::TRUE_ID);
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

TEST_F(ManagerTest, FindVarsWorks)
{
    std::set<ClassProject::BDD_ID> andABVars;
    std::set<ClassProject::BDD_ID> orCAndABVars;
    ClassProject::BDD_ID aVarId = manager.createVar("a");
    ClassProject::BDD_ID bVarId = manager.createVar("b");
    ClassProject::BDD_ID cVarId = manager.createVar("c");
    ClassProject::BDD_ID andABId = manager.and2(aVarId, bVarId);
    ClassProject::BDD_ID orCAndABId = manager.or2(cVarId, andABId);
    manager.findVars(andABId, andABVars);
    manager.findVars(orCAndABId, orCAndABVars);

    std::set<ClassProject::BDD_ID> andABGroundTruthVars = {aVarId, bVarId};
    std::set<ClassProject::BDD_ID> orCAndABGroundTruthVars = {aVarId, bVarId, cVarId};
    EXPECT_EQ(andABVars, andABGroundTruthVars);
    EXPECT_EQ(orCAndABVars, orCAndABGroundTruthVars);
}

TEST_F(ManagerTest, UniqueTableSizeWorks)
{
    EXPECT_EQ(manager.uniqueTableSize(), 2);

    manager.createVar("a");
    manager.createVar("b");

    EXPECT_EQ(manager.uniqueTableSize(), 4);
}

TEST_F(ManagerExampleTest, FindNodesWorks)
{
    std::set<ClassProject::BDD_ID> nodes;
    manager.findNodes(functionId, nodes);

    EXPECT_EQ(nodes.size(), 6); // the other 4 nodes are unreachable from functionId

    unsigned int constNum = 0;
    unsigned int varNum = 0;
    unsigned int funcNum = 0;
    for (const ClassProject::BDD_ID &node : nodes)
    {
        if (manager.isVariable(node))
        {
            varNum++;
        }
        else if (manager.isConstant(node))
        {
            constNum++;
        }
        else
        {
            funcNum++;
        }
    }

    EXPECT_EQ(constNum, 2);
    EXPECT_EQ(varNum, 1);
    EXPECT_EQ(funcNum, 3);
}

TEST_F(ManagerExampleTest, FindVarsWorks)
{
    std::set<ClassProject::BDD_ID> vars;
    manager.findVars(functionId, vars);

    EXPECT_EQ(vars.size(), 4);
    ClassProject::BDD_ID id = 2;
    for (const ClassProject::BDD_ID &var : vars)
    {
        EXPECT_TRUE(manager.isVariable(var));
        EXPECT_EQ(var, id++);
    }
}

TEST_F(ManagerExampleTest, UniqueTableSizeWorks)
{
    EXPECT_EQ(manager.uniqueTableSize(), 10);

    manager.createVar("e");
    manager.createVar("f");

    EXPECT_EQ(manager.uniqueTableSize(), 12);
}

TEST_F(ManagerTest, ComplementedEdgesWork)
{
    ClassProject::BDD_ID aVarId = manager.createVar("a");
    ClassProject::BDD_ID bVarId = manager.createVar("b");
    ClassProject::BDD_ID negAId = manager.neg(aVarId);
    ClassProject::BDD_ID negBId = manager.neg(bVarId);

    ClassProject::BDD_ID orNegANegBId = manager.or2(negAId, negBId);
    ClassProject::BDD_ID andABId = manager.and2(aVarId, bVarId);

    EXPECT_NE(orNegANegBId, andABId);
    EXPECT_EQ(manager.coFactorTrue(orNegANegBId, aVarId), negBId);
    EXPECT_EQ(manager.coFactorFalse(negAId, aVarId), ClassProject::TRUE_ID);
}

#endif
