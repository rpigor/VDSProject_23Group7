#ifndef VDSPROJECT_REACHABILITY_TESTS_H
#define VDSPROJECT_REACHABILITY_TESTS_H

#include <gtest/gtest.h>
#include "Reachability.h"

using namespace ClassProject;

struct ReachabilityTest : testing::Test {

    std::unique_ptr<ClassProject::ReachabilityInterface> fsm2 = std::make_unique<ClassProject::Reachability>(2);

    std::vector<BDD_ID> stateVars2 = fsm2->getStates();
    std::vector<BDD_ID> transitionFunctions;

};

class ReachabilityExample1Test : public testing::Test {
protected:

    std::unique_ptr<ClassProject::ReachabilityInterface> fsm;

    void SetUp() override
    {
        fsm = std::make_unique<ClassProject::Reachability>(1, 2);

        std::vector<BDD_ID> stateBits = fsm->getStates();
        BDD_ID s0 = stateBits.at(0);

        std::vector<BDD_ID> inputBits = fsm->getInputs();
        BDD_ID done = inputBits.at(1);
        BDD_ID enable = inputBits.at(0);

        std::vector<BDD_ID> transitionFunctions;
        transitionFunctions.push_back(fsm->or2(fsm->and2(fsm->neg(s0), enable), fsm->and2(s0, fsm->neg(done)))); // s0' = neg(s0)*enable + s0*neg(done)
        fsm->setTransitionFunctions(transitionFunctions);

        fsm->setInitState({false});
    }
};

/*  (s0, s1)

           halt
           ┌──┐
           │  ▼
           └┬──┐ !halt
      ┌───► │10├────┐
      │     └──┘    │
      │             ▼
     ┌┴─┐         ┌──┐
┌──► │00│◄────────┤11│
     └──┘         └──┘

 */
class ReachabilityExample2Test : public testing::Test {
protected:

    std::unique_ptr<ClassProject::ReachabilityInterface> fsm;

    void SetUp() override
    {
        fsm = std::make_unique<ClassProject::Reachability>(2, 1);

        std::vector<BDD_ID> stateBits = fsm->getStates();
        BDD_ID s0 = stateBits.at(0);
        BDD_ID s1 = stateBits.at(1);

        std::vector<BDD_ID> inputBits = fsm->getInputs();
        BDD_ID halt = inputBits.at(0);

        std::vector<BDD_ID> transitionFunctions;
        transitionFunctions.push_back(fsm->or2(fsm->and2(fsm->neg(s1), fsm->neg(s0)), fsm->and2(fsm->neg(s1), s0))); // s0' = neg(s1)*neg(s0) + neg(s1)*s0
        transitionFunctions.push_back(fsm->and2(fsm->and2(fsm->neg(s1), s0), fsm->neg(halt))); // s1' = neg(s1)*s0*neg(halt)
        fsm->setTransitionFunctions(transitionFunctions);

        fsm->setInitState({false, false});
    }
};

class ReachabilityCounterTest : public testing::Test {
protected:

    std::unique_ptr<ClassProject::ReachabilityInterface> fsm;

    void SetUp() override
    {
        fsm = std::make_unique<ClassProject::Reachability>(3, 1);

        std::vector<BDD_ID> stateBits = fsm->getStates();
        BDD_ID s0 = stateBits.at(0);
        BDD_ID s1 = stateBits.at(1);
        BDD_ID s2 = stateBits.at(2);

        std::vector<BDD_ID> inputBits = fsm->getInputs();
        BDD_ID start = inputBits.at(0);

        std::vector<BDD_ID> transitionFunctions;
        transitionFunctions.push_back(fsm->and2(fsm->and2(fsm->neg(s0), s1), s2)); // s0' = neg(s0)*s1*s2
        transitionFunctions.push_back(fsm->or2(fsm->and2(fsm->and2(fsm->neg(s0), fsm->neg(s1)), s2), fsm->and2(fsm->and2(fsm->neg(s0), s1), fsm->neg(s2)))); // s1' = neg(s0)*neg(s1)*s2 + neg(s0)*s1*neg(s2)
        transitionFunctions.push_back(fsm->or2(fsm->and2(fsm->and2(fsm->and2(fsm->neg(s0), fsm->neg(s1)), fsm->neg(s2)), start), fsm->and2(fsm->and2(fsm->neg(s0), s1), fsm->neg(s2)))); // s2' = neg(s0)*neg(s1)*neg(s2)*start + neg(s0)*s1*neg(s2)
        fsm->setTransitionFunctions(transitionFunctions);

        fsm->setInitState({false, false, false});
    }
};

TEST_F(ReachabilityTest, ConstructorWorks)
{
    for (int i = 0; i < stateVars2.size(); i++)
    {
        ASSERT_EQ(fsm2->getTopVarName(stateVars2.at(i)), "s" + std::to_string(i));
    }
}

TEST_F(ReachabilityTest, SetTransitionWorks)
{
    BDD_ID s0 = stateVars2.at(0);
    BDD_ID s1 = stateVars2.at(1);

    transitionFunctions.push_back(fsm2->neg(s0));
    transitionFunctions.push_back(fsm2->neg(s1));
    transitionFunctions.push_back(fsm2->and2(fsm2->neg(s1), s0));

    EXPECT_THROW(fsm2->setTransitionFunctions(transitionFunctions), std::runtime_error);

    transitionFunctions.pop_back();
    transitionFunctions.pop_back();
    EXPECT_THROW(fsm2->setTransitionFunctions(transitionFunctions), std::runtime_error);

    transitionFunctions.pop_back();
    EXPECT_THROW(fsm2->setTransitionFunctions(transitionFunctions), std::runtime_error);
}

TEST_F(ReachabilityTest, SetInitStateWorks)
{
    EXPECT_THROW(fsm2->setInitState({false, false, true});, std::runtime_error);
    EXPECT_THROW(fsm2->setInitState({false});, std::runtime_error);
    EXPECT_THROW(fsm2->setInitState({});, std::runtime_error);
}

TEST_F(ReachabilityTest, ExampleWorks)
{
    BDD_ID s0 = stateVars2.at(0);
    BDD_ID s1 = stateVars2.at(1);

    transitionFunctions.push_back(fsm2->neg(s0)); // s0' = not(s0)
    transitionFunctions.push_back(fsm2->neg(s1)); // s1' = not(s1)
    fsm2->setTransitionFunctions(transitionFunctions);

    fsm2->setInitState({false,false});

    ASSERT_TRUE(fsm2->isReachable({false, false}));
    ASSERT_FALSE(fsm2->isReachable({false, true}));
    ASSERT_FALSE(fsm2->isReachable({true, false}));
    ASSERT_TRUE(fsm2->isReachable({true, true}));
}

TEST_F(ReachabilityExample1Test, IsReachableWorks)
{
    ASSERT_THROW(fsm->isReachable({false, false}), std::runtime_error);

    ASSERT_TRUE(fsm->isReachable({false}));
    ASSERT_TRUE(fsm->isReachable({true}));
}

TEST_F(ReachabilityExample1Test, StateDistanceWorks)
{
    ASSERT_THROW(fsm->stateDistance({false, false}), std::runtime_error);

    ASSERT_EQ(fsm->stateDistance({false}), 0);
    ASSERT_EQ(fsm->stateDistance({true}), 1);
}

TEST_F(ReachabilityExample2Test, IsReachableWorks)
{
    ASSERT_THROW(fsm->isReachable({false}), std::runtime_error);

    ASSERT_TRUE(fsm->isReachable({false, false}));
    ASSERT_FALSE(fsm->isReachable({false, true}));
    ASSERT_TRUE(fsm->isReachable({true, false}));
    ASSERT_TRUE(fsm->isReachable({true, true}));
}

TEST_F(ReachabilityExample2Test, StateDistanceWorks)
{
    ASSERT_THROW(fsm->stateDistance({false}), std::runtime_error);

    ASSERT_EQ(fsm->stateDistance({false, false}), 0);
    ASSERT_EQ(fsm->stateDistance({false, true}), -1);
    ASSERT_EQ(fsm->stateDistance({true, false}), 1);
    ASSERT_EQ(fsm->stateDistance({true, true}), 2);
}

TEST_F(ReachabilityCounterTest, IsReachableWorks)
{
    ASSERT_THROW(fsm->isReachable({false, false, true, false}), std::runtime_error);

    ASSERT_TRUE(fsm->isReachable({false, false, false}));
    ASSERT_TRUE(fsm->isReachable({false, false, true}));
    ASSERT_TRUE(fsm->isReachable({false, true, false}));
    ASSERT_TRUE(fsm->isReachable({false, true, true}));
    ASSERT_TRUE(fsm->isReachable({true, false, false}));
    ASSERT_FALSE(fsm->isReachable({true, false, true}));
    ASSERT_FALSE(fsm->isReachable({true, true, false}));
    ASSERT_FALSE(fsm->isReachable({true, true, true}));
}

TEST_F(ReachabilityCounterTest, StateDistanceWorks)
{
    ASSERT_THROW(fsm->stateDistance({}), std::runtime_error);

    ASSERT_EQ(fsm->stateDistance({false, false, false}), 0);
    ASSERT_EQ(fsm->stateDistance({false, false, true}), 1);
    ASSERT_EQ(fsm->stateDistance({false, true, false}), 2);
    ASSERT_EQ(fsm->stateDistance({false, true, true}), 3);
    ASSERT_EQ(fsm->stateDistance({true, false, false}), 4);
    ASSERT_EQ(fsm->stateDistance({true, false, true}), -1);
    ASSERT_EQ(fsm->stateDistance({true, true, false}), -1);
    ASSERT_EQ(fsm->stateDistance({true, true, true}), -1);
}

#endif
