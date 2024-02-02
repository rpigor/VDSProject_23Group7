#include "Reachability.h"

using namespace ClassProject;

Reachability::Reachability(unsigned int stateSize, unsigned int inputSize)
: ReachabilityInterface(stateSize, inputSize), initState(stateSize, false)
{
    if (stateSize == 0)
    {
        throw std::runtime_error("FSM requires at least one state!");
    }

    for (int i = 0; i < stateSize; i++)
    {
        states.push_back(createVar("s" + std::to_string(i)));
    }

    for (int i = 0; i < inputSize; i++)
    {
        inputs.push_back(createVar("i" + std::to_string(i)));
    }

    // TODO: initialize transition functions and compute characteristic functions/relations
}

Reachability::Reachability(unsigned int stateSize)
: Reachability(stateSize, 0)
{

}

const std::vector<BDD_ID> &Reachability::getStates() const
{
    return states;
}

const std::vector<BDD_ID> &Reachability::getInputs() const
{
    return inputs;
}

void Reachability::setTransitionFunctions(const std::vector<BDD_ID> &transitionFunctions)
{
    this->transitionFunctions = transitionFunctions;
}

void Reachability::setInitState(const std::vector<bool> &stateVector)
{
    initState = stateVector;
}