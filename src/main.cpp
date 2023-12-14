//
// Created by Carolina P. Nogueira 2016
//

#include <iostream>
#include <string>
#include "Manager.h"

using namespace ClassProject;

int main(int argc, char* argv[])
{
    Manager manager;

    BDD_ID x1VarId = manager.createVar("x1");
    BDD_ID x2VarId = manager.createVar("x2");
    BDD_ID x3VarId = manager.createVar("x3");
    BDD_ID negX1VarId = manager.neg(x1VarId);
    BDD_ID negX2VarId = manager.neg(x2VarId);
    BDD_ID negX3VarId = manager.neg(x3VarId);

    BDD_ID andNegsId = manager.and2(manager.and2(negX1VarId, negX2VarId), negX3VarId);
    BDD_ID andX1X2Id = manager.and2(x1VarId, x2VarId);
    BDD_ID andX2X3Id = manager.and2(x2VarId, x3VarId);
    BDD_ID orAllId = manager.or2(manager.or2(andNegsId, andX1X2Id), andX2X3Id);

    manager.debugUniqueTable();

    std::string diagramFilename = "complemented_edges.png";

    // Reference diagram:
    // https://en.wikipedia.org/wiki/Binary_decision_diagram#/media/File:BDD_diagram_with_complemented_edges.png
    manager.visualizeBDD(diagramFilename, orAllId);

    std::cout << "Generated example BDD diagram: " << diagramFilename << " file." << std::endl;
}
