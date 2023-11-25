#include "Manager.h"
#include <boost/functional/hash.hpp>

using namespace ClassProject;

bool NodeTriple::operator==(const NodeTriple &triple) const
{
    return (this->topVariable == triple.topVariable) && (this->low == triple.low) && (this->high == triple.high);
}

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

    uniqueTable.insert(falseNode);
    uniqueTable.insert(trueNode);
}

BDD_ID Manager::createVar(const std::string &label)
{
    BDD_ID varId = uniqueTable.size();
    NodeTriple varNodeTriple {varId, TRUE_ID, FALSE_ID};
    Node varNode {varId, varNodeTriple, label};

    uniqueTable.insert(varNode);
    return varId;
}

const BDD_ID &Manager::True()
{
    return TRUE_ID; // equivalent to uniqueTable.get<UniqueTableHashMapTags::ById>().find(TRUE_ID)->id;
}

const BDD_ID &Manager::False()
{
    return FALSE_ID; // equivalent to uniqueTable.get<UniqueTableHashMapTags::ById>().find(FALSE_ID)->id;
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

std::size_t Manager::uniqueTableSize()
{
    return uniqueTable.size();
}

void Manager::visualizeBDD(std::string filepath, BDD_ID &root)
{

}