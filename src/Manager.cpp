#include "Manager.h"
#include <boost/functional/hash.hpp>

using namespace ClassProject;

std::size_t NodeTripleHash::operator()(const NodeTriple &triple) const
{
    std::size_t hash = 0;
    boost::hash_combine(hash, boost::hash_value(triple.topVariable));
    boost::hash_combine(hash, boost::hash_value(triple.low));
    boost::hash_combine(hash, boost::hash_value(triple.high));
    return hash;
}

Manager::Manager()
{
    NodeTriple falseTriple {FALSE_ID, FALSE_ID, FALSE_ID};
    Node falseNode {FALSE_ID, falseTriple, "False"};
    NodeTriple trueTriple {TRUE_ID, TRUE_ID, TRUE_ID};
    Node trueNode {TRUE_ID, trueTriple, "True"};

    uniqueTable.emplace(falseTriple, falseNode);
    uniqueTable.emplace(trueTriple, trueNode);
}

BDD_ID Manager::createVar(const std::string &label)
{
    BDD_ID varId = uniqueTable.size();
    NodeTriple varNodeTriple {varId, TRUE_ID, FALSE_ID};
    Node varNode {varId, varNodeTriple, label};

    uniqueTable.emplace(varNodeTriple, varNode);
    return varId;
}

const BDD_ID &Manager::True()
{
    return uniqueTable.at(NodeTriple {TRUE_ID, TRUE_ID, TRUE_ID}).id; // when optimizing: simply return TRUE_ID
}

const BDD_ID &Manager::False()
{
    return uniqueTable.at(NodeTriple {FALSE_ID, FALSE_ID, FALSE_ID}).id; // when optimizing: simply return FALSE_ID
}

bool Manager::isConstant(BDD_ID f)
{

}

bool Manager::isVariable(BDD_ID x)
{

}

BDD_ID Manager::topVar(BDD_ID f)
{

}

BDD_ID Manager::ite(BDD_ID i, BDD_ID t, BDD_ID e)
{

}

BDD_ID Manager::coFactorTrue(BDD_ID f, BDD_ID x)
{

}

BDD_ID Manager::coFactorFalse(BDD_ID f, BDD_ID x)
{

}

BDD_ID Manager::coFactorTrue(BDD_ID f)
{

}

BDD_ID Manager::coFactorFalse(BDD_ID f)
{

}

BDD_ID Manager::and2(BDD_ID a, BDD_ID b)
{

}

BDD_ID Manager::or2(BDD_ID a, BDD_ID b)
{

}

BDD_ID Manager::xor2(BDD_ID a, BDD_ID b)
{

}

BDD_ID Manager::neg(BDD_ID a)
{

}

BDD_ID Manager::nand2(BDD_ID a, BDD_ID b)
{

}

BDD_ID Manager::nor2(BDD_ID a, BDD_ID b)
{

}

BDD_ID Manager::xnor2(BDD_ID a, BDD_ID b)
{

}

std::string Manager::getTopVarName(const BDD_ID &root)
{

}

void Manager::findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root)
{

}

void Manager::findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root)
{

}

size_t Manager::uniqueTableSize()
{

}

void Manager::visualizeBDD(std::string filepath, BDD_ID &root)
{

}