#ifndef VDSPROJECT_NODE_H
#define VDSPROJECT_NODE_H

#include "ManagerInterface.h"

namespace ClassProject {

    static const BDD_ID FALSE_ID = 0;
    static const BDD_ID TRUE_ID = 1;

    /**
     * @brief The NodeTriple class represents the triple that defines
     *  the identity of a node.
     */
    class NodeTriple {
    public:
        BDD_ID topVariable;
        BDD_ID low;
        BDD_ID high;

        bool operator==(const NodeTriple &triple) const;
    };

    /**
     * @brief The NodeTripleHash class represents the hash function of
     * the NodeTriple class.
     */
    class NodeTripleHash {
    public:
        std::size_t operator()(const NodeTriple &triple) const;
    };

    /**
     * @brief The ComputedNode class represents a node of the computed table.
     */
    class ComputedNode {
    public:
        BDD_ID result;
        std::string comment;
    };

    /**
     * @brief The Node class represents a node of the BDD.
     */
    class Node {
    public:
        BDD_ID id;
        NodeTriple triple;
        std::string label;

        inline Node(BDD_ID id, NodeTriple triple, std::string label)
        : id(id), triple(triple), label(label)
        {

        }

        inline Node(BDD_ID id, BDD_ID topVariable, BDD_ID low, BDD_ID high, std::string label)
        : id(id), triple(NodeTriple {topVariable, low, high}), label(label)
        {

        }
    };

}

#endif