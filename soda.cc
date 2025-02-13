#include <iostream>
#include <string>
#include <uPRNG.h>
#include "printer.h"
#include "watCardOffice.h"
#include "config.h"
#include "groupoff.h"
#include "student.h"
#include "parent.h"
#include "vendingMachine.h"
using namespace std;

int main(int argc, char *argv[]) {
	string configFile = "soda.config";
	unsigned int seed = 0, processors = 1; // seed is 0 if we are using default seed
	ConfigParms c;
	struct cmd_error{};
	try {
		switch(argc) {
			case 4:
				if (strcmp(argv[3], "d") != 0) { // parse number of processors
                    processors = convert(argv[3]);
                    if (processors <= 0) throw cmd_error{};
                }
			case 3:
				if (strcmp(argv[2], "d") != 0) { // parse random seed
					seed = convert(argv[2]);
					if(seed<=0) throw cmd_error{};
				}
			case 2:
				if (strcmp(argv[1], "d") != 0) { // config file
					configFile = argv[1];
				}
			case 1: break;
			default: throw cmd_error{};
		}
		processConfigFile(configFile.c_str(), c);
	}
	catch ( ... ) {
		cerr << "Usage: " << argv[0] << " [ config-file | 'd' [ seed (> 0) | 'd' [ processors (> 0) | 'd' ] ] ]" << endl;
		exit(EXIT_FAILURE);
	}

	if (seed != 0) { set_seed(seed); } // if seed is provided, use it

	uProcessor p[processors - 1]; // number of kernel threads
	{
		// launch tasks
		Printer printer(c.numStudents, c.numVendingMachines, c.numCouriers);
		Bank bank(c.numStudents);
		Parent parent(printer, bank, c.numStudents, c.parentalDelay);
		WATCardOffice office(printer, bank, c.numCouriers);
		Groupoff g(printer, c.numStudents, c.sodaCost, c.groupoffDelay);
		NameServer nameServer(printer, c.numVendingMachines, c.numStudents);
		VendingMachine *machines[c.numVendingMachines];
		for(unsigned int i=0; i<c.numVendingMachines; i++) { // launch all vending machines
			machines[i] = new VendingMachine(printer, nameServer, i, c.sodaCost);
		}
		{
			BottlingPlant bottlingPlant(printer, nameServer, c.numVendingMachines, c.maxShippedPerFlavour, c.maxStockPerFlavour, c.timeBetweenShipments);
			Student *students[c.numStudents];

			for(unsigned int i=0; i<c.numStudents; i++) {
				students[i] = new Student(printer, nameServer, office, g, i, c.maxPurchases);
			}
			for(unsigned int i=0; i<c.numStudents; i++) { // wait for all students to complete
				delete students[i];
			}
		} // we use a nested block here to force bottlingPlant to shut down after student shuts down
		for(unsigned int i=0; i<c.numVendingMachines; i++) { // wait for vending machines to shut down
			delete machines[i];
		} // wait for all other tasks to complete
	}
}