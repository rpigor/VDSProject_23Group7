#ifndef VDSPROJECT_REACHABILITY_H
#define VDSPROJECT_REACHABILITY_H

#include "ReachabilityInterface.h"
#include <vector>

namespace ClassProject {

    class Reachability : public ReachabilityInterface {
    private:

        std::vector<BDD_ID> currStateBits;

        std::vector<BDD_ID> nextStateBits;

        std::vector<BDD_ID> inputBits;

        BDD_ID transitionRelation;

        BDD_ID characteristicInitState;

        BDD_ID boolToBDD(bool boolean);

        BDD_ID exists(BDD_ID function, BDD_ID var);

        BDD_ID computeImage(BDD_ID characteristicState);

        bool isInStateSet(std::vector<bool> state, BDD_ID stateSet);

        BDD_ID unionStatesWithImage(BDD_ID characteristicStateSet);

    public:

        Reachability(unsigned int stateSize, unsigned int inputSize);

        Reachability(unsigned int stateSize);

        const std::vector<BDD_ID> &getStates() const override;

        const std::vector<BDD_ID> &getInputs() const override;

        bool isReachable(const std::vector<bool> &stateVector) override;

        int stateDistance(const std::vector<bool> &stateVector) override;

        void setTransitionFunctions(const std::vector<BDD_ID> &transitionFunctions) override;

        void setInitState(const std::vector<bool> &stateVector) override;

    };

}

#endif
