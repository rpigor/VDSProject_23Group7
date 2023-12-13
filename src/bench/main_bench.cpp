//
// Written by Carolina P. Nogueira 2016
// Refactored by Deutschmann 28.09.2021
//

#include <iostream>
#include <string>

#include "Manager.h"
#include "BenchParser.hpp"
#include "CircuitToBDD.hpp"
#include "BenchmarkLib.h"

int main(int argc, char *argv[]) {

    if (2 > argc) {
        std::cout << "Must specify a filename!" << std::endl;
        return -1;
    }

    std::string bench_file = argv[1];

    /* Parse the circuit from file and generate topological sorted circuit */
    BenchParser parsed_circuit(bench_file);

    auto BDD_manager = make_shared<ClassProject::Manager>();
    auto circuit2BDD = make_unique<CircuitToBDD>(BDD_manager);

    double user_time, vm1, rss1, vm2, rss2;

    std::cout << "- Generating BDD from circuit...";
    process_mem_usage(vm1, rss1);
    user_time = userTime();
    circuit2BDD->GenerateBDD(parsed_circuit.GetSortedCircuit(), bench_file);
    user_time = userTime() - user_time;
    std::cout << " BDD generated successfully!" << std::endl << std::endl;

    circuit2BDD->PrintBDD(parsed_circuit.GetListOfOutputLabels());

    std::cout << "**** Performance ****" << std::endl;
    std::cout << " Runtime: " << user_time << std::endl;
    process_mem_usage(vm2, rss2);
    std::cout << " VM: " << vm2 - vm1 << "; RSS: " << rss2 - rss1 << endl << endl;

    return 0;
}
