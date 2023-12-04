#ifndef VDSPROJECT_UNIQUE_TABLE_H
#define VDSPROJECT_UNIQUE_TABLE_H

#include "ManagerInterface.h"
#include "Table.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/tag.hpp>

namespace ClassProject {

    class UniqueTable {
    private:

        struct ById {

        };

        struct ByTriple {

        };

        typedef boost::multi_index_container<
            Node,
            boost::multi_index::indexed_by<
                boost::multi_index::hashed_unique<
                    boost::multi_index::tag<ById>,
                    boost::multi_index::member<Node, BDD_ID, &Node::id>
                >,
                boost::multi_index::hashed_unique<
                    boost::multi_index::tag<ByTriple>,
                    boost::multi_index::member<Node, NodeTriple, &Node::triple>,
                    NodeTripleHash
                >
            >
        > unordered_bimap;

        unordered_bimap table;

        unordered_bimap::index<ById>::type &tableById();

        unordered_bimap::index<ByTriple>::type &tableByTriple();

    public:
        bool insert(const Node& node);

        unordered_bimap::iterator findById(const BDD_ID &id);

        unordered_bimap::iterator findByTriple(const NodeTriple &triple);

        std::size_t size() const;

        unordered_bimap::iterator begin();

        unordered_bimap::iterator end();

        unordered_bimap::const_iterator begin() const;

        unordered_bimap::const_iterator end() const;

        unordered_bimap::const_iterator cbegin() const;

        unordered_bimap::const_iterator cend() const;

    };

}

#endif