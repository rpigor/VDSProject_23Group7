//
// Written by Carolina P. Nogueira 2016
// Refactored by Deutschmann 27.09.2021
//

#include "CircuitToBDD.hpp"

#include <utility>


CircuitToBDD::CircuitToBDD(shared_ptr<ClassProject::ManagerInterface> BDD_manager_p) {
    bdd_manager = std::move(BDD_manager_p);
}

CircuitToBDD::~CircuitToBDD() = default;

void CircuitToBDD::GenerateBDD(const list_of_circuit_t &circuit, const std::string& benchmark_file) {
    ClassProject::BDD_ID BDD_node;

    std::filesystem::path pathToBenchFile(benchmark_file);
    if (!pathToBenchFile.has_filename())
        throw std::runtime_error("circuit_to_BDD_manager::GenerateBDD: benchmark_file not specified");
    if (!std::filesystem::exists(benchmark_file))
        throw std::runtime_error("circuit_to_BDD_manager::GenerateBDD: benchmark_file doesn't exist");
    result_dir = "results_" + pathToBenchFile.stem().string();

    if (!(std::filesystem::exists(result_dir)) && !std::filesystem::create_directory(result_dir)) {
        throw std::runtime_error("Unable to create directory 'result' for the output!");
    }

    std::ofstream bdd_out_file(result_dir + "/BNode_BDD.csv");

    if (!bdd_out_file.is_open()) {
        throw std::runtime_error("Unable to open Log File!");
    }

    bdd_out_file << "BDD_ID,Bench Label" << std::endl;

    for (const auto &circuit_node : circuit) {
        if (circuit_node.gate_type == INPUT_GATE_T) {
            BDD_node = InputGate(circuit_node.label);
        } else if (circuit_node.gate_type == NOT_GATE_T) {
            BDD_node = NotGate(circuit_node.input_id_list);
        } else if (circuit_node.gate_type == AND_GATE_T) {
            BDD_node = AndGate(circuit_node.input_id_list);
        } else if (circuit_node.gate_type == OR_GATE_T) {
            BDD_node = OrGate(circuit_node.input_id_list);
        } else if (circuit_node.gate_type == NAND_GATE_T) {
            BDD_node = NandGate(circuit_node.input_id_list);
        } else if (circuit_node.gate_type == NOR_GATE_T) {
            BDD_node = NorGate(circuit_node.input_id_list);
        } else if (circuit_node.gate_type == XOR_GATE_T) {
            BDD_node = XorGate(circuit_node.input_id_list);
        } else if (circuit_node.gate_type == BUFFER_GATE_T) {
            BDD_node = findBddId(*circuit_node.input_id_list.begin());
        }

        /* OUTPUT or FLIP FLOP gates do not generate a BDD */
        if (!((circuit_node.gate_type == OUTPUT_GATE_T) | (circuit_node.gate_type == FLIP_FLOP_GATE_T))) {
            node_to_bdd_id.insert(std::pair<unique_ID_t, ClassProject::BDD_ID>(circuit_node.id, BDD_node));
            label_to_bdd_id.insert(std::pair<label_t, ClassProject::BDD_ID>(circuit_node.label, BDD_node));
            bdd_out_file << BDD_node << "," << circuit_node.label << std::endl;
        }
    }

    bdd_out_file.close();
}


ClassProject::BDD_ID CircuitToBDD::findBddId(unique_ID_t circuit_node) {

    auto bdd_id_it = node_to_bdd_id.find(circuit_node);

    if (bdd_id_it != node_to_bdd_id.end()) {
        return bdd_id_it->second;
    } else {
        throw std::runtime_error("Destination node ID is not part of the circuit graph!");
    }
}


ClassProject::BDD_ID CircuitToBDD::InputGate(const label_t &label) {
    return bdd_manager->createVar(label);
}


ClassProject::BDD_ID CircuitToBDD::NotGate(const set_of_circuit_t &inputNodes) {
    unique_ID_t node = *inputNodes.begin();
    return bdd_manager->neg(findBddId(node));
}


ClassProject::BDD_ID CircuitToBDD::AndGate(set_of_circuit_t inputNodes) {
    auto it = inputNodes.begin();
    ClassProject::BDD_ID first_op, second_op;

    /* Get the ClassProject::BDD_ID of first elements */
    first_op = findBddId(*it);
    inputNodes.erase(it);

    while (!inputNodes.empty()) {
        it = inputNodes.begin();
        second_op = findBddId(*it);
        inputNodes.erase(it);

        first_op = bdd_manager->and2(first_op, second_op);
    }

    /* Return the ClassProject::BDD_ID equivalent to the AND of all inputs */
    return first_op;
}


ClassProject::BDD_ID CircuitToBDD::OrGate(set_of_circuit_t inputNodes) {
    auto it = inputNodes.begin();
    ClassProject::BDD_ID first_op, second_op;

    /* Get the ClassProject::BDD_ID of first elements */
    first_op = findBddId(*it);
    inputNodes.erase(it);

    while (!inputNodes.empty()) {
        it = inputNodes.begin();
        second_op = findBddId(*it);
        inputNodes.erase(it);

        first_op = bdd_manager->or2(first_op, second_op);
    }

    /* Return the ClassProject::BDD_ID equivalent to the OR of all inputs */
    return first_op;
}

ClassProject::BDD_ID CircuitToBDD::NandGate(set_of_circuit_t inputNodes) {
    auto it = inputNodes.begin();
    ClassProject::BDD_ID first_op, second_op;

    /* Get the ClassProject::BDD_ID of first elements */
    it = inputNodes.begin();
    first_op = findBddId(*it);
    inputNodes.erase(it);

    if (inputNodes.size() == 1) {
        it = inputNodes.begin();
        second_op = findBddId(*it);
        inputNodes.erase(it);

        /* Create the NAND BDD node for the first two elements */
        first_op = bdd_manager->nand2(first_op, second_op);
    } else {
        /* AND of all inputs, to use as the second operator of the NAND gate */
        second_op = AndGate(inputNodes);
        /* Create the NAND BDD node */
        first_op = bdd_manager->nand2(first_op, second_op);
    }

    /* Return the ClassProject::BDD_ID equivalent to the NAND of all inputs */
    return first_op;
}

ClassProject::BDD_ID CircuitToBDD::NorGate(set_of_circuit_t inputNodes) {
    auto it = inputNodes.begin();
    ClassProject::BDD_ID first_op, second_op;

    /* Get the ClassProject::BDD_ID of first elements */
    it = inputNodes.begin();
    first_op = findBddId(*it);
    inputNodes.erase(it);

    if (inputNodes.size() == 1) {
        it = inputNodes.begin();
        second_op = findBddId(*it);
        inputNodes.erase(it);

        /* Create the NOR BDD node for the first two elements */
        first_op = bdd_manager->nor2(first_op, second_op);
    } else {
        /* OR of all inputs, to use as the second operator of the NAND gate */
        second_op = OrGate(inputNodes);
        /* Create the NOR BDD node */
        first_op = bdd_manager->nor2(first_op, second_op);
    }

    /* Return the ClassProject::BDD_ID equivalent to the NOR of all inputs */
    return first_op;
}

ClassProject::BDD_ID CircuitToBDD::XorGate(set_of_circuit_t inputNodes) {
    auto it = inputNodes.begin();
    ClassProject::BDD_ID first_op, second_op;

    /* Get the ClassProject::BDD_ID of first elements */
    first_op = findBddId(*it);
    inputNodes.erase(it);

    while (!inputNodes.empty()) {
        it = inputNodes.begin();
        second_op = findBddId(*it);
        inputNodes.erase(it);

        first_op = bdd_manager->xor2(first_op, second_op);
    }

    /* Return the ClassProject::BDD_ID equivalent to the XOR of all inputs */
    return first_op;
}

void CircuitToBDD::PrintBDD(const std::set<label_t> &output_labels) {

    if ((!(std::filesystem::exists(result_dir + "/txt")) &
         !(std::filesystem::create_directory(result_dir + "/txt")))
        & (!(std::filesystem::exists(result_dir + "/dot")) &
           !(std::filesystem::create_directory(result_dir + "/dot")))) {
        throw std::runtime_error("Unable to create directories 'txt' and 'dot' for the output!");
    }

    for (const auto &output_label : output_labels) {

        auto output_id_it = label_to_bdd_id.find(output_label);

        if (output_id_it != label_to_bdd_id.end()) {

            std::string dot_file_name = result_dir + "/dot/" + std::string(output_label) + ".dot";
            std::string txt_file_name = result_dir + "/txt/" + std::string(output_label) + ".txt";

            std::ofstream bdd_out_dot_file(dot_file_name);
            std::ofstream bdd_out_txt_file(txt_file_name);

            if (!bdd_out_dot_file.is_open() | !bdd_out_txt_file.is_open()) {
                throw std::runtime_error("Unable to open Log File!");
            }

            output_nodes.clear();
            output_vars.clear();
            bdd_manager->findNodes(output_id_it->second, output_nodes);
            bdd_manager->findVars(output_id_it->second, output_vars);

            dumpBddText(bdd_out_txt_file);
            dumpBddDot(bdd_out_dot_file);

            bdd_out_dot_file.close();
            bdd_out_txt_file.close();

        } else {
            throw std::runtime_error("Destination node UUID is not part of the circuit graph!");
        }
    }
}

void CircuitToBDD::dumpBddText(std::ostream &out) {
    for (auto it = output_nodes.rbegin(); it != output_nodes.rend(); ++it) {
        if (bdd_manager->isConstant(*it)) {
            out << "Terminal Node: " << (*it) << "\n";
        } else {
            out << "Variable Node: " << (*it)
                << " Top Var Id: " << bdd_manager->topVar(*it)
                << " Top Var Name: " << bdd_manager->getTopVarName(bdd_manager->topVar(*it))
                << " Low: " << bdd_manager->coFactorFalse(*it)
                << " High: " << bdd_manager->coFactorTrue(*it) << "\n";
        }
    }
}

void CircuitToBDD::dumpBddDot(std::ostream &out) {
    out << "digraph BDD {\n";
    out << "center = true;\n";
    out << "{ rank = same; { node [style=invis]; \"T\" };\n";
    out << " { node [shape=box,fontsize=12]; \"0\"; }\n";
    out << "  { node [shape=box,fontsize=12]; \"1\"; }\n}\n";
    for (const auto var : output_vars) {
        out << R"({ rank=same; { node [shape=plaintext,fontname="Times Italic",fontsize=12] ")"
            << bdd_manager->getTopVarName(var) << "\" };";
        for (unsigned long node : output_nodes) {
            if (bdd_manager->topVar(node) == var) {
                out << "\"" << node << "\";";
            }
        }
        out << "}\n";
    }
    out << "edge [style = invis]; {";
    for (const auto var : output_vars) {
        out << "\"" << bdd_manager->getTopVarName(var) << "\" -> ";
    }
    out << "\"T\"; }\n";
    for (const auto node : output_nodes) {
        if (!bdd_manager->isConstant(node)) {
            out << "\"" << node << "\" -> \"" << bdd_manager->coFactorTrue(node)
                << "\" [style=solid,arrowsize=\".75\"];\n";
            out << "\"" << node << "\" -> \"" << bdd_manager->coFactorFalse(node)
                << "\" [style=dashed,arrowsize=\".75\"];\n";
        }
    }
    out << "}\n";
}


