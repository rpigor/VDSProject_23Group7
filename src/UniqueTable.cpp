#include "UniqueTable.h"
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