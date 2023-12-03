#include "Manager.h"
#include <graphviz/gvc.h>
#include <vector>
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

Agnode_t *Manager::createNodeIfAbsent(Agraph_t *graph, const std::string &nodeLabel)
{
    std::vector<char> labelStr(nodeLabel.c_str(), nodeLabel.c_str() + nodeLabel.size() + 1);
    Agnode_t *gNode = agnode(graph, labelStr.data(), FALSE);
    if (gNode == NULL)
    {
        gNode = agnode(graph, labelStr.data(), TRUE);
        setGProperty(gNode, "label", nodeLabel);
        setGProperty(gNode, "shape", "circle");
        setGProperty(gNode, "style", "filled");
        setGProperty(gNode, "fillcolor", "#bedee8");
    }
    return gNode;
}

Agedge_t *Manager::createEdgeIfAbsent(Agraph_t *graph, Agnode_t *firstNode, Agnode_t *secondNode, const std::string &nodeLabel)
{
    std::vector<char> labelStr(nodeLabel.c_str(), nodeLabel.c_str() + nodeLabel.size() + 1);
    Agedge_t *gEdge = agedge(graph, firstNode, secondNode, labelStr.data(), FALSE);
    if (gEdge == NULL)
    {
        gEdge = agedge(graph, firstNode, secondNode, labelStr.data(), TRUE);
    }
    return gEdge;
}

template <typename T>
void Manager::setGProperty(T *ref, std::string name, std::string value)
{
    std::vector<char> nameStr(name.c_str(), name.c_str() + name.size() + 1);
    std::vector<char> valueStr(value.c_str(), value.c_str() + value.size() + 1);
    agsafeset(ref, const_cast<char*>(nameStr.data()), const_cast<char*>(valueStr.data()), const_cast<char*>(""));
}

Manager::Manager()
{
    NodeTriple falseTriple{FALSE_ID, FALSE_ID, FALSE_ID};
    Node falseNode{FALSE_ID, falseTriple, "False"};
    NodeTriple trueTriple{TRUE_ID, TRUE_ID, TRUE_ID};
    Node trueNode{TRUE_ID, trueTriple, "True"};

    uniqueTable.insert(falseNode);
    uniqueTable.insert(trueNode);
}

BDD_ID Manager::createVar(const std::string &label)
{
    BDD_ID varId = uniqueTable.size();
    NodeTriple varNodeTriple{varId, FALSE_ID, TRUE_ID};
    Node varNode{varId, varNodeTriple, label};

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

bool Manager::isConstant(BDD_ID node)
{
    return (node == TRUE_ID) || (node == FALSE_ID);
}

bool Manager::isVariable(BDD_ID node)
{
    auto nodeRef = uniqueTableById().find(node);
    return (nodeRef->triple.high == TRUE_ID) && (nodeRef->triple.low == FALSE_ID);
}

BDD_ID Manager::topVar(BDD_ID node)
{
    auto nodeRef = uniqueTableById().find(node);
    return nodeRef->triple.topVariable;
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

    if ((t == TRUE_ID) && (e == FALSE_ID))
    {
        return i;
    }

    NodeTriple iteTriple{i, t, e};
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
    NodeTriple rTriple{xTopVar, rLow, rHigh};
    auto uniqueTableResult = uniqueTableByTriple().find(rTriple);
    if (uniqueTableResult == uniqueTableByTriple().end())
    {
        rId = uniqueTable.size();
        Node rNode{rId, rTriple, nodeToString(xTopVar, rHigh, rLow)};
        uniqueTableByTriple().insert(rNode);
    }
    else
    {
        rId = uniqueTableResult->id;
    }

    std::string compComment = "ite(" + std::to_string(i) + ", " + std::to_string(t) + ", " + std::to_string(e) + ")";
    NodeTriple compTriple{i, t, e};
    ComputedNode compNode{rId, compComment};
    computedTable.insert(std::make_pair(compTriple, compNode));

    return rId;
}

BDD_ID Manager::coFactorTrue(BDD_ID function, BDD_ID var)
{
    if (isConstant(function) || isConstant(var) || topVar(function) > var)
    {
        return function;
    }

    auto nodeRef = uniqueTableById().find(function);
    if (topVar(function) == var)
    {
        return nodeRef->triple.high;
    }

    BDD_ID trueCoFactId = coFactorTrue(nodeRef->triple.high, var);
    BDD_ID falseCoFactId = coFactorTrue(nodeRef->triple.low, var);

    return ite(topVar(function), trueCoFactId, falseCoFactId);
}

BDD_ID Manager::coFactorFalse(BDD_ID function, BDD_ID var)
{
    if (isConstant(function) || isConstant(var) || topVar(function) > var)
    {
        return function;
    }

    auto nodeRef = uniqueTableById().find(function);
    if (topVar(function) == var)
    {
        return nodeRef->triple.low;
    }

    BDD_ID trueCoFactId = coFactorFalse(nodeRef->triple.high, var);
    BDD_ID falseCoFactId = coFactorFalse(nodeRef->triple.low, var);

    return ite(topVar(function), trueCoFactId, falseCoFactId);
}

BDD_ID Manager::coFactorTrue(BDD_ID function)
{
    return coFactorTrue(function, topVar(function));
}

BDD_ID Manager::coFactorFalse(BDD_ID function)
{
    return coFactorFalse(function, topVar(function));
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
    return ite(a, neg(b), b);
}

BDD_ID Manager::neg(BDD_ID a)
{
    return ite(a, FALSE_ID, TRUE_ID);
}

BDD_ID Manager::nand2(BDD_ID a, BDD_ID b)
{
    return ite(a, neg(b), TRUE_ID);
}

BDD_ID Manager::nor2(BDD_ID a, BDD_ID b)
{
    return ite(a, FALSE_ID, neg(b));
}

BDD_ID Manager::xnor2(BDD_ID a, BDD_ID b)
{
    return ite(a, b, neg(b));
}

std::string Manager::getTopVarName(const BDD_ID &root)
{
    auto topVarNode = uniqueTableById().find(topVar(root));
    return topVarNode->label;
}

void Manager::findNodes(const BDD_ID &root, std::set<BDD_ID> &rootNodes)
{
    std::stack<BDD_ID> nodes;
    nodes.push(root);
    while (!nodes.empty())
    {
        auto topNode = uniqueTableById().find(nodes.top());
        rootNodes.insert(nodes.top());
        nodes.pop();

        if (!isConstant(topNode->id))
        {
            nodes.push(topNode->triple.high);
            nodes.push(topNode->triple.low);
        }
    }
}

void Manager::findVars(const BDD_ID &root, std::set<BDD_ID> &rootVars)
{
    std::set<BDD_ID> nodes;
    findNodes(root, nodes);
    for (BDD_ID nodeId : nodes)
    {
        if (!isConstant(nodeId))
        {
            rootVars.insert(topVar(nodeId));
        }
    }
}

std::size_t Manager::uniqueTableSize()
{
    return uniqueTable.size();
}

void Manager::visualizeBDD(std::string filepath, BDD_ID &root)
{
    Agraph_t *g = agopen(const_cast<char*>("G"), Agdirected, NULL);
    setGProperty(g, "dpi", "400");

    Agnode_t *rootGNode = createNodeIfAbsent(g, getTopVarName(root));
    Agnode_t *trueGNode = createNodeIfAbsent(g, uniqueTableById().find(TRUE_ID)->label);
    Agnode_t *falseGNode = createNodeIfAbsent(g, uniqueTableById().find(FALSE_ID)->label);
    setGProperty(trueGNode, "label", "1");
    setGProperty(falseGNode, "label", "0");
    setGProperty(trueGNode, "shape", "square");
    setGProperty(falseGNode, "shape", "square");
    setGProperty(trueGNode, "fillcolor", "#000000");
    setGProperty(falseGNode, "fillcolor", "#000000");
    setGProperty(trueGNode, "fontcolor", "#ffffff");
    setGProperty(falseGNode, "fontcolor", "#ffffff");

    std::stack<BDD_ID> nodes;
    nodes.push(root);
    while (!nodes.empty())
    {
        auto topNode = uniqueTableById().find(nodes.top());
        Agnode_t *topGNode = createNodeIfAbsent(g, getTopVarName(topNode->id));
        nodes.pop();

        if (!isConstant(topNode->id))
        {
            auto highNode = uniqueTableById().find(topNode->triple.high);
            auto lowNode = uniqueTableById().find(topNode->triple.low);
            Agnode_t *highGNode = createNodeIfAbsent(g, getTopVarName(highNode->id));
            Agnode_t *lowGNode = createNodeIfAbsent(g, getTopVarName(lowNode->id));
            Agedge_t *highGEdge = createEdgeIfAbsent(g, topGNode, highGNode, "high");
            Agedge_t *lowGEdge = createEdgeIfAbsent(g, topGNode, lowGNode, "low");
            setGProperty(lowGEdge, "style", "dotted");
            nodes.push(topNode->triple.high);
            nodes.push(topNode->triple.low);
        }
    }

    GVC_t *gvc = gvContext();
    gvLayout(gvc, g, "dot");
    gvRender(gvc, g, "png", fopen(filepath.c_str(), "w"));
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);
}