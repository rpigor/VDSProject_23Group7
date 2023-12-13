//
// Written by Carolina P. Nogueira 2016
// Refactored by Deutschmann 28.09.2021
//

#include "BenchParser.hpp"

BenchParser::BenchParser(const std::string &bench_file) {

    id_counter = 0;

    if (parseFile(bench_file)) {
        /* Based on the list of output labels, generate the corresponding circuit */
        std::cout << "- Creating circuit from bench nodes... ";
        createCircuitFromOutputList();
        std::cout << "Done!" << std::endl;

        /* Sort the circuit */
        std::cout << "- Topologically sorting the circuit... ";
        TopologicalSortKahnsAlgorithm();
        std::cout << "Done!" << std::endl;

        label_to_node.clear();
        labels_to_id.clear();
        id_to_circuit_node.clear();
        output_labels.clear();
        ff_labels.clear();
        output_circuits.clear();
        input_circuits.clear();
    } else {
        throw std::runtime_error("Please check bench file syntax!");
    }
}

BenchParser::~BenchParser() = default;

/* --------------- 
 * Print Functions 
 * ---------------
 */
void BenchParser::PrintLabelsTable() {
    std::unordered_map<label_t, bench_node_t>::const_iterator it_label;

    std::cout << "============ [BEGIN] Table of Labels and Nodes ============" << std::endl;
    std::cout << std::endl << "-----" << std::endl;
    for (it_label = label_to_node.begin(); it_label != label_to_node.end(); it_label++) {
        std::cout << std::endl << "Label: " << it_label->first << std::endl;
        std::cout << "Node Information:" << std::endl;
        std::cout << "\tNode Label: " << it_label->second.label << std::endl;
        std::cout << "\tGate Type: " << it_label->second.gate_type << std::endl;
        std::cout << "\tInputs: ";
        for (const auto &i : it_label->second.input_node_list)
            std::cout << i << ' ';
        std::cout << std::endl << "-----" << std::endl;
    }
    std::cout << "============ [END] Table of Labels and Nodes ============" << std::endl;
}

void BenchParser::PrintUUIDCircuitTable() {
    std::unordered_map<unique_ID_t, circuit_node_t>::const_iterator it_uuid;

    std::cout << "============ [BEGIN] Table of Labels and Nodes ============" << std::endl;
    std::cout << std::endl << "-----" << std::endl;
    for (it_uuid = id_to_circuit_node.begin(); it_uuid != id_to_circuit_node.end(); it_uuid++) {
        std::cout << std::endl << "UUID: " << it_uuid->first << std::endl;
        std::cout << "Node Information:" << std::endl;
        std::cout << "\tNode Circuit Label: " << it_uuid->second.label << std::endl;
        std::cout << "\tGate Type: " << it_uuid->second.gate_type << std::endl;
        std::cout << "\tInputs: ";
        for (unsigned long i : it_uuid->second.input_id_list)
            std::cout << i << ' ';
        std::cout << std::endl << "-----" << std::endl;
        std::cout << "\tOutputs: ";
        for (unsigned long i : it_uuid->second.output_id_list)
            std::cout << i << ' ';
        std::cout << std::endl << "-----" << std::endl;
    }
    std::cout << "============ [END] Table of Labels and Nodes ============" << std::endl;
}

void BenchParser::PrintOutputList() {
    std::set<label_t>::const_iterator it;

    std::cout << std::endl << "============ [BEGIN] List of Outputs ============" << std::endl << std::endl;
    std::cout << std::endl << "List of output labels: ";
    for (it = output_labels.begin(); it != output_labels.end(); it++) {
        std::cout << (*it) << " -> ";
    }
    std::cout << "end;" << std::endl;
    std::cout << std::endl << "============ [END] List of Outputs ============" << std::endl;
}

void BenchParser::PrintLabels2UUIDTable() {
    std::unordered_map<label_t, unique_ID_t>::const_iterator it_label;

    std::cout << "============ [BEGIN] Table of Labels and UUIDs ============" << std::endl;
    std::cout << std::endl << "-----" << std::endl;
    for (it_label = labels_to_id.begin(); it_label != labels_to_id.end(); it_label++) {
        std::cout << std::endl << "Label: " << it_label->first << std::endl;
        std::cout << std::endl << "UUID: " << it_label->second << std::endl;
        std::cout << std::endl << "------------------------------" << std::endl;
    }
    std::cout << "============ [END] Table of Labels and UUIDs ============" << std::endl;
}

void BenchParser::PrintCircuit(unique_ID_t circuit_ID, int indent) {
    std::unordered_map<unique_ID_t, circuit_node_t>::const_iterator got;
    circuit_node_t node;

    got = (id_to_circuit_node).find(circuit_ID);
    if (got != id_to_circuit_node.end()) {
        node = got->second;
        std::cout << std::string(indent, ' ') << "Node ID: " << node.id << std::endl;
        std::cout << std::string(indent, ' ') << "Label: " << node.label << std::endl;
        std::cout << std::string(indent, ' ') << "Type: " << node.gate_type << std::endl;

        std::cout << std::string(indent, ' ') << "Input List: " << std::endl;
        for (unsigned long i : node.input_id_list)
            std::cout << std::string(indent, ' ') << "    " << i << std::endl;

        std::cout << std::string(indent, ' ') << "Output List: " << std::endl;
        for (unsigned long i : node.output_id_list)
            std::cout << std::string(indent, ' ') << "    " << i << std::endl;

        indent = indent + 4;
        std::cout << std::string(indent, ' ') << "--------------" << std::endl;
        for (unsigned long i : node.input_id_list)
            PrintCircuit(i, indent);
        std::cout << std::string(indent, ' ') << "--------------" << std::endl;
    }
}

void BenchParser::PrintCircuitByLabel(const label_t &node_label) {
    std::unordered_map<label_t, unique_ID_t>::const_iterator got;

    got = (labels_to_id).find(node_label);
    if (got != labels_to_id.end()) {
        PrintCircuit(got->second, 0);
    } else {
        throw std::runtime_error("There is no mapping from this label to a circuit node.");
    }

}


void BenchParser::PrintCircuitsOfOutputSet() {
    for (unsigned long it : output_circuits) {
        PrintCircuit(it, 0);
    }
}

void BenchParser::PrintSortedCircuitList() {
    list_of_circuit_t::const_iterator it;

    std::cout << std::endl << "============ [BEGIN] List of Sorted Circuit Nodes ============" << std::endl
              << std::endl;
    std::cout << std::endl << "List of Sorted Circuit Nodes labels: ";

    list_of_circuit_t ordered_circuit = this->GetSortedCircuit();


    for (it = ordered_circuit.begin(); it != ordered_circuit.end(); it++) {
        std::cout << (*it).id << " -> ";
    }
    std::cout << "end;" << std::endl;
    std::cout << std::endl << "============ [END] List of Sorted Circuit Nodes ============" << std::endl;
}

/* ---------------- 
 * Get functions
 * ----------------
 */

set_of_circuit_t BenchParser::GetListOfCircuitsInputs() {
    return input_circuits;
}


set_of_circuit_t BenchParser::GetListOfCircuitsOutputs() {
    return output_circuits;
}

std::set<label_t> BenchParser::GetListOfOutputLabels() {
    return outputs;
}


circuit_node_t BenchParser::GetCircuitNode(unique_ID_t circuit_node_uuid) {
    /* Iterator for the uuid2circuitNode_table table */
    std::unordered_map<unique_ID_t, circuit_node_t>::const_iterator got;

    /* Search the node and add the UUID of the current node to this node OUTPUT_LIST */
    got = id_to_circuit_node.find(circuit_node_uuid);

    if (got != id_to_circuit_node.end()) {
        return got->second;
    }//if
    else {
        throw std::runtime_error("There is no mapping from this unique_ID_t to a circuit_node_type.");
    }
}

std::unordered_map<unique_ID_t, circuit_node_t> BenchParser::GetCircuit() {
    return id_to_circuit_node;
}

list_of_circuit_t BenchParser::GetSortedCircuit() {
    return sorted_circuit;
}

/* --------------- 
 * Read File Functions 
 * ---------------
 */
bool BenchParser::parseFile(const std::string &bench_file) {

    std::cout << std::endl << "- Reading bench format file... ";
    std::ifstream in(bench_file);
    if (!in.is_open()) {
        throw std::runtime_error("Could not open file: " + bench_file);
    }
    in.unsetf(std::ios::skipws);
    std::cout << "Done!" << std::endl;

    /* Declaring file iterators */
    boost::spirit::istream_iterator first(in), last;

    /* Declaring skip parser to be used */
    skip_p::skip_grammar<boost::spirit::istream_iterator> skip;

    /* Declaring bench format grammar */
    bench_format::bench_parser<boost::spirit::istream_iterator> bench_grammar_parser;

    /* Stored result after parsing a file line */
    bench_format::bench_node_type parsed_bench_node;

    /* Effectively parsing the file. The parser returns a benchnode to be added to the labels table */
    std::cout << "- Parsing input file '" << bench_file << "'... ";
    bool successfully_parsed;
    do {
        successfully_parsed = phrase_parse(first, last, bench_grammar_parser, skip, parsed_bench_node);
        if (!successfully_parsed) {
            std::cout << "Failed parsing input file at: " << std::string(first, last) << std::endl;
            return false;
        } else {
            /* Add the successfully read bench node into the labels table */
            addToLabelTable(parsed_bench_node);
        }
        parsed_bench_node = bench_format::bench_node_type();
    } while (first != last); /* Operations are performed until it is EOF */
    std::cout << "Done!" << std::endl;

    return true;
}


bool BenchParser::addToLabelTable(bench_node_t bench_node) {
    bool new_node_added;
    label_t search_label;
    /*
     * Output nodes have to be handle a bit different, since they'll have
     *  the same label as another gate. Mappings require unique element
     *  identifiers. By convention, OUTPUT gates will be referenced by its
     *  original label concatenated with the string "OUTPUT" to differentiate
     *  from the other gate that has the same label.
    */
    if (bench_node.gate_type == OUTPUT_GATE_T) {
        /* If the gate is an output gate, it must be included into the set_of_output_labels */
        output_labels.insert(bench_node.label);
        search_label = bench_node.label + bench_node.gate_type;
    } else {
        search_label = bench_node.label;
    }

    if (label_to_node.find(search_label) != label_to_node.end()) {
        new_node_added = false;
    } else {
        /*
         * Otherwise add bench_node to the labels_table
         */
        if (bench_node.gate_type == FLIP_FLOP_GATE_T) {

            /* If it is a flip flop, we have to add two nodes:
               One that will be the output node;
               Another one as input node.
               For the searching part it is not necessary to differentiate it,
               since for each existing flip flop in the circuit we will add
               to the labels table one node that is an INPUT with the same
               label as the flip flop, and another one with the "FLIP_FLOP_GATE_T"
               string concatenated. So if we search for one of them, it is
               enough to check whether the node exists or not. */
            ff_labels.insert(search_label);
            label_to_node.insert(std::pair<label_t, bench_node_t>(search_label + FLIP_FLOP_GATE_T, bench_node));
            bench_node.gate_type = INPUT_GATE_T;
            bench_node.input_node_list.clear();
        }
        label_to_node.insert(std::pair<label_t, bench_node_t>(search_label, bench_node));
        new_node_added = true;
    }
    return new_node_added;
}

unique_ID_t BenchParser::findOrAddToCircuit(const bench_node_t &bench_node) {

    unique_ID_t CircuitNodeID;
    std::unordered_map<label_t, unique_ID_t>::const_iterator got;
    circuit_node_t new_circuit_node;
    label_t search_label;

    if ((bench_node.gate_type == OUTPUT_GATE_T) || (bench_node.gate_type == FLIP_FLOP_GATE_T)) {
        search_label = bench_node.label + bench_node.gate_type;
    } else {
        search_label = bench_node.label;
    }

    got = labels_to_id.find(search_label);

    if (got != labels_to_id.end()) {
        /* If the iterator is not pointing to the end of the labels_table,
           means that already exist another node with the same label. Then
           simply returns the unique ID from the node. */
        CircuitNodeID = (got->second);
    } else {
        /*
         * Otherwise add bench_node to the labels_table and returns its UUID
         */
        new_circuit_node = benchNodeToCircuitNode(bench_node);
        CircuitNodeID = new_circuit_node.id;
        labels_to_id.insert(std::pair<label_t, unique_ID_t>(search_label, CircuitNodeID));
        id_to_circuit_node.insert(std::pair<unique_ID_t, circuit_node_t>(CircuitNodeID, new_circuit_node));
    }
    return CircuitNodeID;
}


unique_ID_t BenchParser::findOrAddToCircuitByLabel(const label_t &node_label) {

    auto label = (label_to_node).find(node_label);

    if (label != label_to_node.end()) {
        return (findOrAddToCircuit(label->second));
    } else {
        throw std::runtime_error("There is no mapping from this label to a node.");
    }
}

circuit_node_t BenchParser::benchNodeToCircuitNode(const bench_node_t &bench_node) {

    circuit_node_t new_circuit_node;
    size_t input_id, new_id;

    /* Generate an unique identifier */
    new_circuit_node.id = id_counter;
    id_counter++;
    new_id = new_circuit_node.id;

    new_circuit_node.label = bench_node.label;
    new_circuit_node.gate_type = bench_node.gate_type;

    /* If it is not an INPUT gate, we have to recursively iterate to get the unique id of the inputs */
    if (!(new_circuit_node.gate_type == INPUT_GATE_T)) {

        if (new_circuit_node.gate_type == OUTPUT_GATE_T) {

            input_id = findOrAddToCircuitByLabel(bench_node.label);
            new_circuit_node.input_id_list.insert(input_id);

            auto node = id_to_circuit_node.find(input_id);
            if (node != id_to_circuit_node.end()) {
                node->second.output_id_list.insert(new_id);
            }

        } else {

            for (const auto &input_node : bench_node.input_node_list) {

                input_id = findOrAddToCircuitByLabel(input_node);
                new_circuit_node.input_id_list.insert(input_id);

                auto node = id_to_circuit_node.find(input_id);
                if (node != id_to_circuit_node.end()) {
                    node->second.output_id_list.insert(new_id);
                }
            }
        }
    } else {
        input_circuits.insert(new_id);
    }

    return new_circuit_node;
}


void BenchParser::createCircuitFromOutputList() {

    std::set<label_t>::const_iterator it;
    std::unordered_map<label_t, unique_ID_t>::const_iterator got_uid;
    std::unordered_map<unique_ID_t, circuit_node_t>::const_iterator got_node;

    for (const auto &output_label : output_labels) {
        createCircuitByLabel(output_label + OUTPUT_GATE_T);
    }
    for (const auto &ff_label : ff_labels) {
        createCircuitByLabel(ff_label + FLIP_FLOP_GATE_T);
    }
    for (const auto &ff_label : ff_labels) {
        auto ff_id = labels_to_id.find(ff_label + FLIP_FLOP_GATE_T);
        auto ff_node = id_to_circuit_node.find(ff_id->second);
        ff_node = id_to_circuit_node.find(*(ff_node->second).input_id_list.begin());
        outputs.insert(ff_node->second.label);
    }
    outputs.insert(output_labels.begin(), output_labels.end());
}

void BenchParser::createCircuitByLabel(const label_t &bnode_label) {
    unique_ID_t new_circuit;
    new_circuit = findOrAddToCircuitByLabel(bnode_label);
    output_circuits.insert(new_circuit);
}


/* -----------------------------
 * Topological Sort Algorithms
 * -----------------------------
 */
void BenchParser::TopologicalSortKahnsAlgorithm() {
    set_of_circuit_t nodes_without_outgoing_edges = GetListOfCircuitsOutputs();
    set_of_circuit_t::iterator it;

    circuit_node_t node, tmp_node;

    while (!nodes_without_outgoing_edges.empty()) {
        it = nodes_without_outgoing_edges.begin();
        /* Always pick the first element of the list of nodes without incoming edges */
        node = GetCircuitNode(*it);
        nodes_without_outgoing_edges.erase(it);

        /* Nodes are added always to the end of the list */
        sorted_circuit.push_front(node);
        for (auto it_input_nodes = node.input_id_list.begin();
             it_input_nodes != node.input_id_list.end(); it_input_nodes++) {
            if (IsUniqueOutgoingEdge(*it_input_nodes, node.id)) {
                nodes_without_outgoing_edges.insert(*it_input_nodes);
            }
        }
    }

    if (sorted_circuit.size() != id_to_circuit_node.size()) {
        throw std::runtime_error("The circuit must be cycle free!");
    }
}


/* -----------------------------
 * Auxiliary functions for
 * Topological Sort Algorithms
 * -----------------------------
 */

bool BenchParser::IsUniqueOutgoingEdge(unique_ID_t src_node, unique_ID_t dst_node) {
    std::unordered_map<unique_ID_t, circuit_node_t>::iterator got;

    /* Search source node in the circuit graph */
    got = id_to_circuit_node.find(src_node);

    if (got != id_to_circuit_node.end()) {
        /* Remove outgoing edge to source node */
        (got->second).output_id_list.erase((got->second).output_id_list.find(dst_node));
        return (got->second).output_id_list.empty();
    }//if
    else {
        throw std::runtime_error("Destination node UUID is not part of the circuit graph!");
    }
}
