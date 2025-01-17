#ifndef VDSPROJECT_UNIQUE_TABLE_H
#define VDSPROJECT_UNIQUE_TABLE_H

#include "ManagerInterface.h"
#include "Node.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
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

        struct ByTripleCompl {

        };

        typedef boost::multi_index_container<
            Node,
            boost::multi_index::indexed_by<
                boost::multi_index::hashed_unique<
                    boost::multi_index::tag<ById>,
                    boost::multi_index::member<Node, BDD_ID, &Node::id>
                >,
                boost::multi_index::hashed_unique<
                    boost::multi_index::tag<ByTripleCompl>,
                    boost::multi_index::composite_key<
                        Node,
                        boost::multi_index::member<Node, NodeTriple, &Node::triple>,
                        boost::multi_index::member<Node, bool, &Node::complemented>
                    >
                >
            >
        > unordered_bimap;

        typedef unordered_bimap::iterator unique_table_iterator;

        typedef unordered_bimap::const_iterator unique_table_const_iterator;

        unordered_bimap table;

        unordered_bimap::index<ById>::type &tableById();

        unordered_bimap::index<ByTripleCompl>::type &tableByTripleCompl();

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
        unique_table_const_iterator findById(const BDD_ID &id);

        /**
         * @brief Finds a node by its triple.
         *
         * @param triple Triple of the node.
         * @return Constant iterator to node.
         */
        unique_table_const_iterator findByTripleAndComplemented(const NodeTriple &triple, bool complemented);

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
        unique_table_iterator begin() const;

        /**
         * @brief Gets iterator to the end of the table.
         *
         * @return Iterator to the end of the table.
         */
        unique_table_iterator end() const;

        /**
         * @brief Gets constant iterator to the first node of the table.
         *
         * @return Constant iterator to the first node of the table.
         */
        unique_table_const_iterator cbegin() const;

        /**
         * @brief Gets constant iterator to the end of the table.
         *
         * @return Constant iterator to the end of the table.
         */
        unique_table_const_iterator cend() const;

    };

}

#endif