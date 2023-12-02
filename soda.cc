#include <iostream>
#include <string>
#include "printer.h"
#include "watCardOffice.h"
#include "config.h"
#include "groupoff.h"
#include "student.h"
#include "vendingMachine.h"
using namespace std;

int main(int argc, char *argv[]) {
	string configFile = "soda.config";
	int seed = -1, processors = 1;
	ConfigParms config;
	struct cmd_error{};
	try {
		switch(argc) {
			case 4:
				if (strcmp(argv[3], "d") != 0) {
                    processors = convert(argv[3]);
                    if (processors <= 0) throw cmd_error{};
                }
			case 3:
				if (strcmp(argv[2], "d") != 0) {
					seed = convert(argv[2]);
					if (seed <= 0) throw cmd_error{};
				}
			case 2:
				if (strcmp(argv[1], "d") != 0) {
					configFile = argv[1];
				}
			case 1: break;
			default: throw cmd_error{};
		}
		processConfigFile(configFile.c_str(), config);
	}
	catch ( ... ) {
		cerr << "Usage: " << argv[0] << " [ config-file | 'd' [ seed (> 0) | 'd' [ processors (> 0) | 'd' ] ] ]" << endl;
		exit(EXIT_FAILURE);
	}

    ConfigParms c;
    Printer printer(c.numStudents, c.numVendingMachines, c.numCouriers);
    Bank bank(c.numStudents);
    WATCardOffice office(printer, bank, c.numCouriers);
    Groupoff g(printer, c.numStudents, c.sodaCost, c.groupoffDelay);
    NameServer nameServer(printer, c.numVendingMachines, c.numStudents);
    BottlingPlant bottlingPlant(printer, nameServer, c.numVendingMachines, c.maxShippedPerFlavour, c.maxStockPerFlavour, c.timeBetweenShipments);

    VendingMachine *machines[c.numVendingMachines];
    Student *students[c.numStudents];
    for(unsigned int i=0; i<c.numVendingMachines; i++) {
        machines[i] = new VendingMachine(printer, nameServer, i, c.sodaCost);
    }
    for(unsigned int i=0; i<c.numStudents; i++) {
        students[i] = new Student(printer, nameServer, office, g, i, c.maxPurchases);
    }

    for(unsigned int i=0; i<c.numVendingMachines; i++) {
        delete machines[i];
    }
    for(unsigned int i=0; i<c.numStudents; i++) {
        delete students[i];
    }
}