#ifndef VDSPROJECT_REACHABILITY_H
#define VDSPROJECT_REACHABILITY_H

#include "ReachabilityInterface.h"
#include <vector>

namespace ClassProject {

    class Reachability : public ReachabilityInterface {
    private:

        std::vector<BDD_ID> states;

        std::vector<BDD_ID> inputs;

        std::vector<BDD_ID> transitionFunctions;

        std::vector<bool> initState;

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
