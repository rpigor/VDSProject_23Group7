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

        NodeTriple(BDD_ID topVariable, BDD_ID low, BDD_ID high);

        bool operator==(const NodeTriple &triple) const;

    };

    std::size_t hash_value(const NodeTriple &triple);

    /**
     * @brief The ComputedNode class represents a node of the computed table.
     */
    class ComputedNode {
    public:

        BDD_ID result;

        ComputedNode(BDD_ID result);

    };

    /**
     * @brief The Node class represents a node of the BDD.
     */
    class Node {
    public:

        BDD_ID id;

        NodeTriple triple;

        bool complemented;

        std::string label;

        Node(BDD_ID id, const NodeTriple &triple, const std::string &label);

        Node(BDD_ID id, BDD_ID topVariable, BDD_ID low, BDD_ID high, const std::string &label);

        Node(BDD_ID id, const NodeTriple &triple, bool complemented, const std::string &label);

        Node(BDD_ID id, BDD_ID topVariable, BDD_ID low, BDD_ID high, bool complemented, const std::string &label);

        bool operator==(const Node &node) const;

    };

}

#endif