// A minimalistic BDD library, following Wolfgang Kunz lecture slides
//
// Created by Markus Wedler 2014

#ifndef VDSPROJECT_MANAGER_H
#define VDSPROJECT_MANAGER_H

#include "ManagerInterface.h"
#include "UniqueTable.h"
#include "Node.h"
#include <boost/unordered_map.hpp>
#include <string>
#include <set>

/**
 * @brief The ClassProject namespace contains everything related to the library.
 */
namespace ClassProject {

    /**
     * @brief The Manager class represents the manager of a binary decision diagram (BDD).
     * Each Manager object has its own context and can be used to manipulate a BDD.
     */
    class Manager : public ManagerInterface {
    private:

        UniqueTable uniqueTable;

        boost::unordered_map<NodeTriple, ComputedNode> computedTable;

        std::string nodeToString(BDD_ID i, BDD_ID t, BDD_ID e);

    public:

        /**
         * @brief Default constructor of the Manager object.
         */
        Manager();

        /**
         * @brief Creates a new variable with the given label.
         *
         * @param label Label of the variable.
         * @return ID of the variable node.
         */
        BDD_ID createVar(const std::string &label) override;

        /**
         * @brief Gets the True node.
         *
         * @return ID of the True node.
         */
        const BDD_ID &True() override;

        /**
         * @brief Gets the False node.
         *
         * @return ID of the False node.
         */
        const BDD_ID &False() override;

        /**
         * @brief Checks if the given ID represents a leaf node.
         *
         * @param node ID of the node.
         * @return True if the given ID represents a leaf node.
         */
        bool isConstant(BDD_ID node) override;

        /**
         * @brief Checks if the given ID represents a variable.
         *
         * @param node ID of the node.
         * @return True if the given ID represents a variable.
         */
        bool isVariable(BDD_ID node) override;

        /**
         * @brief Gets the top variable of the given node.
         *
         * @param node ID of the node.
         * @return ID of the top variable node.
         */
        BDD_ID topVar(BDD_ID node) override;

        /**
         * @brief Implements the if-then-else algorithm, which most of the following
         * functions are based on.
         *
         * @param i ID of the 'if' node.
         * @param t ID of the 'then' node.
         * @param e ID of the 'else' node.
         * @return ID of the existing or new node that represents the given expression.
         */
        BDD_ID ite(BDD_ID i, BDD_ID t, BDD_ID e) override;

        /**
         * @brief Returns the positive co-factor of the function represented by the
         * given node.
         *
         * @param function ID of the node representing the function.
         * @param var ID of the variable node.
         * @return ID of the node representing the positive co-factor.
         */
        BDD_ID coFactorTrue(BDD_ID function, BDD_ID var) override;

        /**
         * @brief Returns the negative co-factor of the function represented by the
         * given node.
         *
         * @param function ID of the node representing the function.
         * @param var ID of the variable node.
         * @return ID of the node representing the negative co-factor.
         */
        BDD_ID coFactorFalse(BDD_ID function, BDD_ID var) override;

        /**
         * @brief Returns the positive co-factor of the function represented by the
         * given node.
         *
         * @param function ID of the node representing the function.
         * @return ID of the node representing the positive co-factor.
         */
        BDD_ID coFactorTrue(BDD_ID function) override;

        /**
         * @brief Returns the negative co-factor of the function represented by the
         * given node.
         *
         * @param function ID of the node representing the function.
         * @return ID of the node representing the negative co-factor.
         */
        BDD_ID coFactorFalse(BDD_ID function) override;

        /**
         * @brief Gets the node representing the resulting function of a ∗ b.
         *
         * @param a ID of the first node.
         * @param b ID of the second node.
         * @return ID of the node representing the a * b function.
         */
        BDD_ID and2(BDD_ID a, BDD_ID b) override;

        /**
         * @brief Gets the node representing the resulting function of a + b.
         *
         * @param a ID of the first node.
         * @param b ID of the second node.
         * @return ID of the node representing the a + b function.
         */
        BDD_ID or2(BDD_ID a, BDD_ID b) override;

        /**
         * @brief Gets the node representing the resulting function of a xor b.
         *
         * @param a ID of the first node.
         * @param b ID of the second node.
         * @return ID of the node representing the a xor b function.
         */
        BDD_ID xor2(BDD_ID a, BDD_ID b) override;

        /**
         * @brief Gets the node representing the negation of the given function.
         *
         * @param a ID of the node to be negated.
         * @return ID of the node representing the negated node.
         */
        BDD_ID neg(BDD_ID a) override;

        /**
         * @brief Gets the node representing the resulting function of a nand b.
         *
         * @param a ID of the first node.
         * @param b ID of the second node.
         * @return ID of the node representing the a nand b function.
         */
        BDD_ID nand2(BDD_ID a, BDD_ID b) override;

        /**
         * @brief Gets the node representing the resulting function of a nor b.
         *
         * @param a ID of the first node.
         * @param b ID of the second node.
         * @return ID of the node representing the a nor b function.
         */
        BDD_ID nor2(BDD_ID a, BDD_ID b) override;

        /**
         * @brief Gets the node representing the resulting function of a xnor b.
         *
         * @param a ID of the first node.
         * @param b ID of the second node.
         * @return ID of the node representing the a xnor b function.
         */
        BDD_ID xnor2(BDD_ID a, BDD_ID b) override;

        /**
         * @brief Returns the label of the top variable of root.
         *
         * @param root ID of the root node.
         * @return Label of the top variable of root.
         */
        std::string getTopVarName(const BDD_ID &root) override;

        /**
         * @brief Gets the set of all nodes which are reachable from root including
         * itself.
         *
         * @param root ID of the root node.
         * @param rootNodes Set of nodes.
         */
        void findNodes(const BDD_ID &root, std::set<BDD_ID> &rootNodes) override;

        /**
         * @brief Gets the set of all variables which are reachable from root including
         * itself.
         *
         * @param root ID of the root node.
         * @param rootVars Set of variable nodes.
         */
        void findVars(const BDD_ID &root, std::set<BDD_ID> &rootVars) override;

        /**
         * @brief Gets the number of nodes currently existing in the unique table.
         *
         * @return Number of nodes in the unique table.
         */
        std::size_t uniqueTableSize() override;

        /**
         * @brief Creates a file (PNG format) that contains a visual representation of the
         * BDD represented by the root node.
         *
         * @param filepath Absolute path of the file.
         * @param root ID of the root node.
         */
        void visualizeBDD(std::string filepath, BDD_ID &root) override;

        void debugUniqueTable();

    };

}

#endif
