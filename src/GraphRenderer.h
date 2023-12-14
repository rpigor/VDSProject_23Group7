#ifndef VDSPROJECT_GRAPH_RENDERER_H
#define VDSPROJECT_GRAPH_RENDERER_H

#include "ManagerInterface.h"
#include "UniqueTable.h"
#include "Node.h"
#include <graphviz/cgraph.h>
#include <string>

namespace ClassProject {

    /**
     * @brief The GraphRenderer class represents a renderer for BDDs.
     */
    class GraphRenderer {
    private:

        Agraph_t *graph;

        template <typename T>
        static void setProperty(T *ref, const std::string &name, const std::string &value);

        Agnode_t *createNodeIfAbsent(Agraph_t *g, const std::string &nodeName, const std::string &nodeLabel);

        Agnode_t *createNodeIfAbsent(const std::string &nodeName, const std::string &nodeLabel);

        Agnode_t *createNodeIfAbsent(const std::string &nodeLabel);

        Agedge_t *createEdgeIfAbsent(Agnode_t *firstNode, Agnode_t *secondNode, const std::string &nodeLabel);

        Agraph_t *createSubGraphIfAbsent(const std::string &subGraphName);

        bool isLeaf(BDD_ID id) const;

        std::string nodeNameByTriple(const NodeTriple &triple);

    public:

        GraphRenderer(const std::string &graphName);

        GraphRenderer(const std::string &graphName, BDD_ID root, UniqueTable &uniqueTable);

        ~GraphRenderer();

        /**
         * @brief Fills the rendering graph with the BDD using the provided unique table.
         *
         * @param function The node ID of the function to be constructed.
         * @param uniqueTable The unique table to be used.
         */
        void fillGraph(BDD_ID function, UniqueTable &uniqueTable);

        /**
         * @brief Renders the BDD to the designed filepath.
         *
         * @param filepath The filepath to render the BDD.
         */
        void renderGraph(const std::string &filepath);

    };

}

#endif