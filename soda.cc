// _Task Student {
// 	void main();
//   public:
// 	Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
// 			 unsigned int id, unsigned int maxPurchases );
// };

// _Task VendingMachine {
// 	void main();
//   public:
// 	_Event Free {};						// free, advertisement
// 	_Event Funds {};					// insufficient funds
// 	_Event Stock {};					// flavour out of stock
// 	VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost );
// 	void buy( BottlingPlant::Flavours flavour, WATCard & card );
// 	unsigned int * inventory() __attribute__(( warn_unused_result ));
// 	void restocked();
// 	_Nomutex unsigned int cost() const;
// 	_Nomutex unsigned int getId() const;
// };

// _Task NameServer {
// 	void main();
//   public:
// 	NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents );
// 	void VMregister( VendingMachine * vendingmachine );
// 	VendingMachine * getMachine( unsigned int id ) __attribute__(( warn_unused_result ));
// 	VendingMachine ** getMachineList() __attribute__(( warn_unused_result ));
// };

// _Task Truck {
// 	void main();
//   public:
// 	Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
// 		   unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
// };

#include <iostream>
#include <string>
#include "config.h"
#include "printer.h"
#include "bank.h"
#include "parent.h"
#include "groupoff.h"
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

	// basic test where we will launch the parent and bank and see if they compile
	Printer printer(config.numStudents, config.numVendingMachines, config.numCouriers);
	Bank bank(config.numStudents);
	Parent parent(printer, bank, config.numStudents, config.parentalDelay);
	Groupoff groupoff(printer, config.numStudents, config.sodaCost, config.groupoffDelay);
	// simulate all students calling groupoff
	for (unsigned int i=0;i<config.numStudents;i++) {
		groupoff.giftCard();
	}
	float a = 2.0;
	// spin for some time...
	for (int i=0;i<1000000000;i++) {
		a = a*a - 2.0;
	}
	return 0;
}