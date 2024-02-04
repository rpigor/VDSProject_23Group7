//
// Created by ludwig on 04.12.18
// Refactored by deutschmann on 19.04.21
//

#ifndef VDSPROJECT_REACHABILITYINTERFACE_H
#define VDSPROJECT_REACHABILITYINTERFACE_H

#include "../Manager.h"

namespace ClassProject {
    
    class ReachabilityInterface : public Manager {
    public:

        ReachabilityInterface() = delete;
        virtual ~ReachabilityInterface() = default;

        /**
         * The constructor initializes a default state machine with the given number of variables.
         * All state variables should be created within the constructor.
         * The default transition function for each state bit is the identity function.
         * For the default initial state, all bits are assumed to be set to false.
         * Hence, after calling the constructor, the only reachable state should be the initial state.
         *
         * @param stateSize vector specifying the number of bits
         * @param inputSize number of boolean input bits, defaults to zero
         * @throws std::runtime_error if stateSize is zero
         */
        explicit ReachabilityInterface(unsigned int stateSize, unsigned int inputSize) {};

        /**
         * Returns a vector containing all state bits of the state machine.
         *
         * @returns vector with the BDD_ID of each state bit
         */
        virtual const std::vector<BDD_ID> &getStates() const = 0;

        /**
         * Returns a vector containing all input bits of the state machine.
         *
         * @returns vector with the BDD_ID of each input bit
         */
        virtual const std::vector<BDD_ID> &getInputs() const = 0;

        /**
         * This function computes whether a specific state is in the reachable state space or not.
         * The input state is provided as a vector of boolean values for each state bit.
         *
         * @param stateVector provides the assignment for each state bit
         * @returns true, if the given state is in the reachable state set
         * @throws std::runtime_error if size does not match with number of state bits
         */
        virtual bool isReachable(const std::vector<bool> &stateVector) = 0;

         /**
          * This function computes the distance from the initial state to a specified state.
          * i.e., minimum cycles it takes the FSM to transition to the specified state
          * Example: Initial state s0 = 0; s1 = 0
          *          Transition functions: s0' = !s1; s1' = s0
          *          FSM transitions:
          *             {False, False}
          *             {True, False}
          *             {True, True}
          *             {False, True}
          *             {False, False}
          *             ...
          *          stateDistance return values:
          *             {False, False} -> 0
          *             {True, False}  -> 1
          *             {True, True}   -> 2
          *             {False, True}  -> 3
          *             {False, False} -> 0
          * @param stateVector provides the assignment for each state bit
          * @return the shortest distance to the initial state, -1 if unreachable
	      * @throws std::runtime_error if size does not match with number of state bits
          */
        virtual int stateDistance(const std::vector<bool> &stateVector) = 0;

        /**
         * Each state variable has a transition function.
         * The transition function specifies the value of the state after the transition.
         * The transition functions can be composed of state variables and inputs if present.
         * Example: s0' = s0 XOR s1
         * Example: s1' = (s1 AND s0) OR i0
         * The next state for s0 is defined as XOR of the current values of the state bit s0 and s1
         * The next state for s1 is defined as the AND of the current values of s1 and s0 ORed with input i0
         * An exception is thrown, if
         *  - The number of given transition functions does not match the number of state bits
         *  - An unknown ID is provided
         *
         * @param transitionFunctions provide a transition function exactly for each state bit
         * @throws std::runtime_error
         */
        virtual void setTransitionFunctions(const std::vector<BDD_ID> &transitionFunctions) = 0;

        /**
         * Provides an initial state for the system as a vector of boolean values.
         * If the entry is true, the state bit is high. Otherwise, the bit is low.
         * E.g. initial state not(s0) and not(s1) is transformed into {false,false}.
         *
         * @param stateVector provides the assignment for each state bit
         * @throws std::runtime_error if size does not match with number of state bits
         */
        virtual void setInitState(const std::vector<bool> &stateVector) = 0;

    };
}

#endif

