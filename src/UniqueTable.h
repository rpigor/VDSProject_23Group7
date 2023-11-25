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

    class NodeTriple {
    public:
        BDD_ID topVariable;
        BDD_ID low;
        BDD_ID high;

        bool operator==(const NodeTriple &triple) const;
    };

    class NodeTripleHash {
    public:
        std::size_t operator()(const NodeTriple &triple) const;
    };

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