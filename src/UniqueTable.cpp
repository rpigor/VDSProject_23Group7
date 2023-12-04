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

UniqueTable::unordered_bimap::iterator UniqueTable::insert(const Node& node)
{
    table.insert(node);
}

UniqueTable::unordered_bimap::iterator UniqueTable::findById(const BDD_ID &id)
{
    return table.iterator_to(*tableById().find(id));
}

UniqueTable::unordered_bimap::iterator UniqueTable::findByTriple(const NodeTriple &triple)
{
    return table.iterator_to(*tableByTriple().find(triple));
}

std::size_t UniqueTable::size() const
{
    return table.size();
}

UniqueTable::unordered_bimap::iterator UniqueTable::begin()
{

}

UniqueTable::unordered_bimap::iterator UniqueTable::end()
{

}

UniqueTable::unordered_bimap::const_iterator UniqueTable::begin() const
{

}

UniqueTable::unordered_bimap::const_iterator UniqueTable::end() const
{

}

UniqueTable::unordered_bimap::const_iterator UniqueTable::cbegin() const
{

}

UniqueTable::unordered_bimap::const_iterator UniqueTable::cend() const
{

}