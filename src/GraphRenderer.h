#ifndef VDSPROJECT_GRAPH_RENDERER_H
#define VDSPROJECT_GRAPH_RENDERER_H

#include "ManagerInterface.h"
#include "UniqueTable.h"
#include <graphviz/cgraph.h>
#include <string>

namespace ClassProject {

    class GraphRenderer {
    private:

        Agraph_t *graph;

        template <typename T>
        static void setProperty(T *ref, const std::string &name, const std::string &value);

        Agnode_t *createNodeIfAbsent(const std::string &nodeLabel);

        Agedge_t *createEdgeIfAbsent(Agnode_t *firstNode, Agnode_t *secondNode, const std::string &nodeLabel);

    public:

        GraphRenderer(const std::string &graphName);

        GraphRenderer(const std::string &graphName, BDD_ID root, UniqueTable &uniqueTable);

        ~GraphRenderer();

        void fillGraph(BDD_ID function, UniqueTable &uniqueTable);

        void renderGraph(const std::string &filepath);

    };

}

#endif