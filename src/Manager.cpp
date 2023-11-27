#include "Manager.h"
#include <stack>
#include <utility>

using namespace ClassProject;

UniqueTableHashMap::index<UniqueTableHashMapTags::ById>::type &Manager::uniqueTableById()
{
    return uniqueTable.get<UniqueTableHashMapTags::ById>();
}

UniqueTableHashMap::index<UniqueTableHashMapTags::ByTriple>::type &Manager::uniqueTableByTriple()
{
    return uniqueTable.get<UniqueTableHashMapTags::ByTriple>();
}

std::string Manager::nodeToString(BDD_ID i, BDD_ID t, BDD_ID e)
{
    if (i == TRUE_ID)
    {
        return "True";
    }

    if (i == FALSE_ID)
    {
        return "False";
    }

    if ((t == TRUE_ID) && (e == TRUE_ID))
    {
        return "True";
    }

    if ((t == FALSE_ID) && (t == FALSE_ID))
    {
        return "False";
    }

    std::string iLabel = uniqueTableById().find(i)->label;
    if ((t == TRUE_ID) && (e == FALSE_ID))
    {
        return iLabel;
    }

    if ((t == FALSE_ID) && (e == TRUE_ID))
    {
        return "neg(" + iLabel + ")";
    }

    std::string eLabel = uniqueTableById().find(e)->label;
    if (t == TRUE_ID)
    {
        return "or2(" + iLabel + ", " + eLabel + ")";
    }

    if (t == FALSE_ID)
    {
        return "and2(neg(" + iLabel + "), " + eLabel + ")";
    }

    std::string tLabel = uniqueTableById().find(t)->label;
    if (e == TRUE_ID)
    {
        return "or2(neg(" + iLabel + "), " + tLabel + ")";
    }

    if (e == FALSE_ID)
    {
        return "and2(" + iLabel + ", " + tLabel + ")";
    }

    return "ite( " + iLabel + ", " + tLabel + ", " + eLabel + ")";
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
    NodeTriple varNodeTriple {varId, FALSE_ID, TRUE_ID};
    Node varNode {varId, varNodeTriple, label};

    uniqueTable.insert(varNode);
    return varId;
}

const BDD_ID &Manager::True()
{
    return TRUE_ID;
}

const BDD_ID &Manager::False()
{
    return FALSE_ID;
}

bool Manager::isConstant(BDD_ID f)
{
    return (f == TRUE_ID) || (f == FALSE_ID);
}

bool Manager::isVariable(BDD_ID x)
{
    auto xNode = uniqueTableById().find(x);
    return (xNode->triple.high == TRUE_ID) && (xNode->triple.low == FALSE_ID);
}

BDD_ID Manager::topVar(BDD_ID f)
{
    auto fNode = uniqueTableById().find(f);
    return fNode->triple.topVariable;
}

BDD_ID Manager::ite(BDD_ID i, BDD_ID t, BDD_ID e)
{
    if ((i == TRUE_ID) || (t == e))
    {
        return t;
    }

    if (i == FALSE_ID)
    {
        return e;
    }

    NodeTriple iteTriple {i, t, e};
    auto computedTableResult = computedTable.find(iteTriple);
    if (computedTableResult != computedTable.end())
    {
        return computedTableResult->second.result;
    }

    BDD_ID xTopVar = std::min({topVar(i), topVar(t), topVar(e)}, [&](const BDD_ID &i1, const BDD_ID &i2) {
        if (isConstant(i1))
        {
            return false;
        }
        else if (isConstant(i2))
        {
            return true;
        }

        return i1 < i2;
    });

    BDD_ID rHigh = ite(coFactorTrue(i, xTopVar), coFactorTrue(t, xTopVar), coFactorTrue(e, xTopVar));
    BDD_ID rLow = ite(coFactorFalse(i, xTopVar), coFactorFalse(t, xTopVar), coFactorFalse(e, xTopVar));
    if (rHigh == rLow)
    {
        return rHigh;
    }

    BDD_ID rId;
    NodeTriple rTriple {xTopVar, rLow, rHigh};
    auto uniqueTableResult = uniqueTableByTriple().find(rTriple);
    if (uniqueTableResult == uniqueTableByTriple().end())
    {
        rId = uniqueTable.size();
        Node rNode {rId, rTriple, nodeToString(xTopVar, rHigh, rLow)};
        uniqueTableByTriple().insert(rNode);
    }
    else
    {
        rId = uniqueTableResult->id;
    }

    std::string compComment = "ite(" + std::to_string(i) + ", " + std::to_string(t) + ", " + std::to_string(e) + ")";
    ComputedNode compNode {rId, compComment};
    computedTable.insert(std::make_pair(rTriple, compNode));

    return rId;
}

BDD_ID Manager::coFactorTrue(BDD_ID f, BDD_ID x)
{
    if (isConstant(f) || isConstant(x) || topVar(f) > x)
    {
        return f;
    }

    auto fNode = uniqueTableById().find(f);
    if (topVar(f) == x)
    {
        return fNode->triple.high;
    }

    BDD_ID trueCoFactId = coFactorTrue(fNode->triple.high, x);
    BDD_ID falseCoFactId = coFactorTrue(fNode->triple.low, x);

    return ite(topVar(f), trueCoFactId, falseCoFactId);
}

BDD_ID Manager::coFactorFalse(BDD_ID f, BDD_ID x)
{
    if (isConstant(f) || isConstant(x) || topVar(f) > x)
    {
        return f;
    }

    auto fNode = uniqueTableById().find(f);
    if (topVar(f) == x)
    {
        return fNode->triple.low;
    }

    BDD_ID trueCoFactId = coFactorFalse(fNode->triple.high, x);
    BDD_ID falseCoFactId = coFactorFalse(fNode->triple.low, x);

    return ite(topVar(f), trueCoFactId, falseCoFactId);
}

BDD_ID Manager::coFactorTrue(BDD_ID f)
{
    return coFactorTrue(f, topVar(f));
}

BDD_ID Manager::coFactorFalse(BDD_ID f)
{
    return coFactorFalse(f, topVar(f));
}

BDD_ID Manager::and2(BDD_ID a, BDD_ID b)
{
    return ite(a, b, FALSE_ID);

}

BDD_ID Manager::or2(BDD_ID a, BDD_ID b)
{
    return ite(a, TRUE_ID, b);
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
    auto topVarNode = uniqueTableById().find(topVar(root));
    return topVarNode->label;
}

void Manager::findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root)
{
    std::stack<BDD_ID> nodes;
    nodes.push(root);
    while (!nodes.empty())
    {
        auto topNode = uniqueTableById().find(nodes.top());
        nodes_of_root.insert(nodes.top());
        nodes.pop();

        if (!isConstant(topNode->id))
        {
            nodes.push(topNode->triple.high);
            nodes.push(topNode->triple.low);
        }
    }
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