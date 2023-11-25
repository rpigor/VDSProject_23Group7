// A minimalistic BDD library, following Wolfgang Kunz lecture slides
//
// Created by Markus Wedler 2014

#ifndef VDSPROJECT_MANAGER_H
#define VDSPROJECT_MANAGER_H

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

    class Manager : public ManagerInterface {
    private:
        UniqueTableHashMap uniqueTable;

    public:
        Manager();

        BDD_ID createVar(const std::string &label) override;

        const BDD_ID &True() override;

        const BDD_ID &False() override;

        bool isConstant(BDD_ID f) override;

        bool isVariable(BDD_ID x) override;

        BDD_ID topVar(BDD_ID f) override;

        BDD_ID ite(BDD_ID i, BDD_ID t, BDD_ID e) override;

        BDD_ID coFactorTrue(BDD_ID f, BDD_ID x) override;

        BDD_ID coFactorFalse(BDD_ID f, BDD_ID x) override;

        BDD_ID coFactorTrue(BDD_ID f) override;

        BDD_ID coFactorFalse(BDD_ID f) override;

        BDD_ID and2(BDD_ID a, BDD_ID b) override;

        BDD_ID or2(BDD_ID a, BDD_ID b) override;

        BDD_ID xor2(BDD_ID a, BDD_ID b) override;

        BDD_ID neg(BDD_ID a) override;

        BDD_ID nand2(BDD_ID a, BDD_ID b) override;

        BDD_ID nor2(BDD_ID a, BDD_ID b) override;

        BDD_ID xnor2(BDD_ID a, BDD_ID b) override;

        std::string getTopVarName(const BDD_ID &root) override;

        void findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root) override;

        void findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root) override;

        std::size_t uniqueTableSize() override;

        void visualizeBDD(std::string filepath, BDD_ID &root) override;
    };

}

#endif
