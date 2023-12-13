#include "GraphRenderer.h"
#include <graphviz/gvc.h>
#include <vector>
#include <stack>

using namespace ClassProject;

GraphRenderer::GraphRenderer(const std::string &graphName)
{
    graph = agopen(const_cast<char*>(graphName.c_str()), Agdirected, nullptr);
}

GraphRenderer::GraphRenderer(const std::string &graphName, BDD_ID root, UniqueTable &uniqueTable)
: GraphRenderer(graphName)
{
    fillGraph(root, uniqueTable);
}

GraphRenderer::~GraphRenderer()
{
    agclose(graph);
}

template <typename T>
void GraphRenderer::setProperty(T *ref, const std::string &name, const std::string &value)
{
    std::vector<char> nameStr(name.c_str(), name.c_str() + name.size() + 1);
    std::vector<char> valueStr(value.c_str(), value.c_str() + value.size() + 1);
    agsafeset(ref, const_cast<char*>(nameStr.data()), const_cast<char*>(valueStr.data()), const_cast<char*>(""));
}

Agnode_t *GraphRenderer::createNodeIfAbsent(const std::string &nodeLabel)
{
    std::vector<char> labelStr(nodeLabel.c_str(), nodeLabel.c_str() + nodeLabel.size() + 1);
    Agnode_t *gNode = agnode(graph, labelStr.data(), FALSE);
    if (gNode == nullptr)
    {
        gNode = agnode(graph, labelStr.data(), TRUE);

        setProperty(gNode, "label", nodeLabel);
        setProperty(gNode, "shape", "circle");
        setProperty(gNode, "style", "filled");
        setProperty(gNode, "fillcolor", "#bedee8");
    }
    return gNode;

}

Agedge_t *GraphRenderer::createEdgeIfAbsent(Agnode_t *firstNode, Agnode_t *secondNode, const std::string &nodeLabel)
{
    std::vector<char> labelStr(nodeLabel.c_str(), nodeLabel.c_str() + nodeLabel.size() + 1);
    Agedge_t *gEdge = agedge(graph, firstNode, secondNode, labelStr.data(), FALSE);
    if (gEdge == nullptr)
    {
        gEdge = agedge(graph, firstNode, secondNode, labelStr.data(), TRUE);
    }
    return gEdge;
}

bool GraphRenderer::isLeaf(BDD_ID id) const
{
    return (id == TRUE_ID) || (id == FALSE_ID);
}

void GraphRenderer::fillGraph(BDD_ID function, UniqueTable &uniqueTable)
{
    setProperty(graph, "dpi", "400");
    setProperty(graph, "bgcolor", "transparent");

    auto functionNode = uniqueTable.findById(function);
    BDD_ID funcTopVar = functionNode->triple.topVariable;
    std::string topVarLabel = uniqueTable.findById(funcTopVar)->label;

    Agnode_t *rootNode = createNodeIfAbsent(topVarLabel);

    if (functionNode->complemented)
    {
        Agnode_t *dummyGNode = createNodeIfAbsent(functionNode->label);
        Agedge_t *dummyGEdge = createEdgeIfAbsent(dummyGNode, rootNode, "-1");

        setProperty(dummyGNode, "color", "transparent");
        setProperty(dummyGNode, "fillcolor", "transparent");
        setProperty(dummyGEdge, "style", "dotted");
    }
    else
    {
        setProperty(rootNode, "xlabel", functionNode->label);
    }

    Agnode_t *trueGNode = createNodeIfAbsent(uniqueTable.findById(TRUE_ID)->label);

    setProperty(trueGNode, "label", "1");
    setProperty(trueGNode, "shape", "square");
    setProperty(trueGNode, "fillcolor", "#000000");
    setProperty(trueGNode, "fontcolor", "#ffffff");

    std::stack<BDD_ID> nodes;
    nodes.push(function);
    while (!nodes.empty())
    {
        auto topNode = uniqueTable.findById(nodes.top());
        BDD_ID topVarOfTopNode = topNode->triple.topVariable;
        std::string topVarLabel = uniqueTable.findById(topVarOfTopNode)->label;

        Agnode_t *topGNode = createNodeIfAbsent(topVarLabel);

        nodes.pop();

        if (!isLeaf(topNode->id))
        {
            auto highNode = uniqueTable.findById(topNode->triple.high);
            BDD_ID topVarOfHighNode = highNode->triple.topVariable;
            std::string highTopVarLabel = uniqueTable.findById(topVarOfHighNode)->label;

            auto lowNode = uniqueTable.findById(topNode->triple.low);
            BDD_ID topVarOfLowNode = lowNode->triple.topVariable;
            std::string lowTopVarLabel = uniqueTable.findById(topVarOfLowNode)->label;

            Agnode_t *highGNode = createNodeIfAbsent(highTopVarLabel);
            Agnode_t *lowGNode = createNodeIfAbsent(lowTopVarLabel);
            Agedge_t *highGEdge = createEdgeIfAbsent(topGNode, highGNode, "high");
            Agedge_t *lowGEdge = createEdgeIfAbsent(topGNode, lowGNode, "low");

            /*
            if (highNode->complemented)
            {
                setProperty(highGEdge, "style", "solid");
            }
            else
            {
                setProperty(highGEdge, "style", "bold");
            }
            */

            if (lowNode->complemented)
            {
                setProperty(lowGEdge, "style", "dotted");
            }
            else
            {
                setProperty(lowGEdge, "style", "dashed");
            }

            nodes.push(topNode->triple.high);
            nodes.push(topNode->triple.low);
        }
    }
}

void GraphRenderer::renderGraph(const std::string &filepath)
{
    GVC_t *gvc = gvContext();

    gvLayout(gvc, graph, "dot");
    gvRender(gvc, graph, "png", fopen(filepath.c_str(), "w"));

    gvFreeLayout(gvc, graph);
    gvFreeContext(gvc);
}