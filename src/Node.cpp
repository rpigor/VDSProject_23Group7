#include "Node.h"
#include <boost/functional/hash.hpp>

using namespace ClassProject;

NodeTriple::NodeTriple(BDD_ID topVariable, BDD_ID low, BDD_ID high)
: topVariable(topVariable), low(low), high(high)
{

}

bool NodeTriple::operator==(const NodeTriple &triple) const
{
    return (this->topVariable == triple.topVariable) && (this->low == triple.low) && (this->high == triple.high);
}

std::size_t ClassProject::hash_value(const NodeTriple &triple)
{
    std::size_t hash = 0;
    boost::hash_combine(hash, boost::hash_value(triple.topVariable));
    boost::hash_combine(hash, boost::hash_value(triple.low));
    boost::hash_combine(hash, boost::hash_value(triple.high));
    return hash;
}

ComputedNode::ComputedNode(BDD_ID result)
: result(result)
{

}

Node::Node(BDD_ID id, const NodeTriple &triple, bool complemented, const std::string &label)
: id(id), triple(triple), complemented(complemented), label(label)
{

}

Node::Node(BDD_ID id, BDD_ID topVariable, BDD_ID low, BDD_ID high, bool complemented, const std::string &label)
: id(id), triple(NodeTriple{topVariable, low, high}), complemented(complemented), label(label)
{

}

Node::Node(BDD_ID id, const NodeTriple &triple, const std::string &label)
: Node(id, triple, false, label)
{

}

Node::Node(BDD_ID id, BDD_ID topVariable, BDD_ID low, BDD_ID high, const std::string &label)
: Node(id, topVariable, low, high, false, label)
{

}

bool Node::operator==(const Node &node) const
{
    return (this->id == node.id);
}
