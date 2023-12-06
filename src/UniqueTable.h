#ifndef VDSPROJECT_UNIQUE_TABLE_H
#define VDSPROJECT_UNIQUE_TABLE_H

#include "ManagerInterface.h"
#include "Node.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/tag.hpp>

namespace ClassProject {

    /**
     * @brief The UniqueTable class represents the unique table
     * of the BDD.
     */
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

        /**
         * @brief Inserts a new node to the table.
         *
         * @param node Node to be inserted.
         */
        void insert(const Node& node);

        /**
         * @brief Finds a node by its ID.
         *
         * @param id ID of the node.
         * @return Constant iterator to node.
         */
        unordered_bimap::const_iterator findById(const BDD_ID &id);

        /**
         * @brief Finds a node by its triple.
         *
         * @param triple Triple of the node.
         * @return Constant iterator to node.
         */
        unordered_bimap::const_iterator findByTriple(const NodeTriple &triple);

        /**
         * @brief Gets the size of the table.
         *
         * @return Size of the table.
         */
        std::size_t size() const;

        /**
         * @brief Gets iterator to the first node of the table.
         *
         * @return Iterator to the first node of the table.
         */
        unordered_bimap::iterator begin() const;

        /**
         * @brief Gets iterator to the end of the table.
         *
         * @return Iterator to the end of the table.
         */
        unordered_bimap::iterator end() const;

        /**
         * @brief Gets constant iterator to the first node of the table.
         *
         * @return Constant iterator to the first node of the table.
         */
        unordered_bimap::const_iterator cbegin() const;

        /**
         * @brief Gets constant iterator to the end of the table.
         *
         * @return Constant iterator to the end of the table.
         */
        unordered_bimap::const_iterator cend() const;

    };

}

#endif