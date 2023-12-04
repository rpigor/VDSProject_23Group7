#include "UniqueTable.h"

using namespace ClassProject;

UniqueTable::unordered_bimap::index<UniqueTable::ById>::type &UniqueTable::tableById()
{
    return table.get<UniqueTable::ById>();
}

UniqueTable::unordered_bimap::index<UniqueTable::ByTriple>::type &UniqueTable::tableByTriple()
{
    return table.get<UniqueTable::ByTriple>();
}

void UniqueTable::insert(const Node& node)
{
    table.insert(node);
}

UniqueTable::unordered_bimap::const_iterator UniqueTable::findById(const BDD_ID &id)
{
    return table.iterator_to(*(tableById().find(id)));
}

UniqueTable::unordered_bimap::const_iterator UniqueTable::findByTriple(const NodeTriple &triple)
{
    return table.iterator_to(*(tableByTriple().find(triple)));
}

std::size_t UniqueTable::size() const
{
    return table.size();
}

UniqueTable::unordered_bimap::iterator UniqueTable::begin() const
{
    return table.begin();
}

UniqueTable::unordered_bimap::iterator UniqueTable::end() const
{
    return table.end();
}

UniqueTable::unordered_bimap::const_iterator UniqueTable::cbegin() const
{
    return table.cbegin();
}

UniqueTable::unordered_bimap::const_iterator UniqueTable::cend() const
{
    return table.cend();
}