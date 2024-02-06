#include "Reachability.h"
#include <string>
#include <algorithm>

using namespace ClassProject;

Reachability::Reachability(unsigned int stateSize, unsigned int inputSize)
: ReachabilityInterface(stateSize, inputSize)
{
    if (stateSize == 0)
    {
        throw std::runtime_error("State must be at least one bit wide.");
    }

    transitionRelation = True();
    characteristicInitState = True();
    for (int i = 0; i < stateSize; i++)
    {
        BDD_ID stateVar = createVar("s" + std::to_string(i));
        BDD_ID nextStateVar = createVar("s" + std::to_string(i) + "'");
        currStateBits.push_back(stateVar);
        nextStateBits.push_back(nextStateVar);

        // compute transition relation of identity function
        transitionRelation = and2(transitionRelation, xnor2(nextStateVar, stateVar));

        // compute characteristic function of the default initial state (0,0)
        characteristicInitState = and2(characteristicInitState, neg(stateVar));
    }

    for (int i = 0; i < inputSize; i++)
    {
        inputBits.push_back(createVar("i" + std::to_string(i)));
    }
}

Reachability::Reachability(unsigned int stateSize)
: Reachability(stateSize, 0)
{

}

BDD_ID Reachability::boolToBDD(bool boolean)
{
    return boolean ? True() : False();
}

BDD_ID Reachability::exists(BDD_ID function, BDD_ID var)
{
    return or2(coFactorTrue(function, var), coFactorFalse(function, var));
}

BDD_ID Reachability::computeImage(BDD_ID characteristicState)
{
    // compute img(s0', s1') = ∃i ∃s0 ∃s1 c_R ∗ τ
    BDD_ID nextStateImg = and2(characteristicState, transitionRelation);
    for (BDD_ID stateBit : currStateBits)
    {
        nextStateImg = exists(nextStateImg, stateBit);
    }
    for (BDD_ID inputBit : inputBits)
    {
        nextStateImg = exists(nextStateImg, inputBit);
    }

    // compute img(s0, s1) = ∃s0' ∃s1' (s0 == s0') ∗ (s1 == s1') ∗ img(s0', s1')
    BDD_ID currStateImg = True();
    for (int i = 0; i < currStateBits.size(); i++)
    {
        currStateImg = and2(currStateImg, xnor2(currStateBits.at(i), nextStateBits.at(i)));
    }
    currStateImg = and2(currStateImg, nextStateImg);
    for (BDD_ID nextStateBit : nextStateBits)
    {
        currStateImg = exists(currStateImg, nextStateBit);
    }

    return currStateImg;
}

bool Reachability::isInStateSet(std::vector<bool> state, BDD_ID stateSet)
{
    BDD_ID existCheck = stateSet;
    for (int i = 0; i < currStateBits.size(); i++)
    {
        existCheck = state.at(i) ? coFactorTrue(existCheck, currStateBits.at(i)) : coFactorFalse(existCheck, currStateBits.at(i));
    }
    return existCheck == True();
}

const std::vector<BDD_ID> &Reachability::getStates() const
{
    return currStateBits;
}

const std::vector<BDD_ID> &Reachability::getInputs() const
{
    return inputBits;
}

bool Reachability::isReachable(const std::vector<bool> &stateVector)
{
    return stateDistance(stateVector) != -1;
}

/*
    R_{it} := S0;
    do
    {
        R := R_{it};
        R_{it} := R \cup img(\delta, R);
    }
    until (R_{it} = R);
*/
int Reachability::stateDistance(const std::vector<bool> &stateVector)
{
    if (stateVector.size() != currStateBits.size())
    {
        throw std::runtime_error("Bit width does not match the bit width of other states.");
    }

    if (isInStateSet(stateVector, characteristicInitState))
    {
        return 0;
    }

    int distance = 1;
    BDD_ID r;
    BDD_ID rIt = characteristicInitState;
    do
    {
        r = rIt;
        rIt = or2(r, computeImage(r));

        if (isInStateSet(stateVector, rIt))
        {
            return distance;
        }

        distance++;
    } while (rIt != r);

    return -1;
}

void Reachability::setTransitionFunctions(const std::vector<BDD_ID> &transitionFunctions)
{
    if (transitionFunctions.size() != nextStateBits.size())
    {
        throw std::runtime_error("A transition function must be provided for each state bit.");
    }
    else if (std::any_of(transitionFunctions.cbegin(), transitionFunctions.cend(), [this](BDD_ID func){ return func > this->uniqueTableSize(); }))
    {
        throw std::runtime_error("One of the provided transition functions has an unknown BDD_ID.");
    }

    // update transition relation
    transitionRelation = True();
    for (int i = 0; i < nextStateBits.size(); i++)
    {
        transitionRelation = and2(transitionRelation, xnor2(nextStateBits.at(i), transitionFunctions.at(i)));
    }
}

void Reachability::setInitState(const std::vector<bool> &stateVector)
{
    if (stateVector.size() != currStateBits.size())
    {
        throw std::runtime_error("Initial state bit width does not match the bit width of other states.");
    }

    // update the characteristic function of the initial state
    characteristicInitState = True();
    for (int i = 0; i < nextStateBits.size(); i++)
    {
        BDD_ID iTerm = xnor2(currStateBits.at(i), boolToBDD(stateVector.at(i)));
        characteristicInitState = and2(characteristicInitState, iTerm);
    }
}