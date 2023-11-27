#ifndef VDSPROJECT_UNIQUE_TABLE_H
#define VDSPROJECT_UNIQUE_TABLE_H

#include "ManagerInterface.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/tag.hpp>

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

    /**
     * @brief The UniqueTableHashMapTags namespace contains the tags representing
     * the indexing mode of the unique table.
     */
    namespace UniqueTableHashMapTags {

        struct ById {

        };

        struct ByTriple {

        };

    }

    using UniqueTableHashMap = boost::multi_index_container<
        Node,
        boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<
                boost::multi_index::tag<UniqueTableHashMapTags::ById>,
                boost::multi_index::member<Node, BDD_ID, &Node::id>
            >,
            boost::multi_index::hashed_unique<
                boost::multi_index::tag<UniqueTableHashMapTags::ByTriple>,
                boost::multi_index::member<Node, NodeTriple, &Node::triple>,
                NodeTripleHash
            >
        >
    >;

}

#endif