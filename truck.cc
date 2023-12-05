#include "truck.h"
#include "nameServer.h"
#include "vendingMachine.h"
#include "bottlingPlant.h"
using namespace std;

// ctor
Truck::Truck(Printer & prt, NameServer & nameServer, BottlingPlant & plant, unsigned int numVendingMachines, unsigned int maxStockPerFlavour) : 
printer{prt}, nameServer{nameServer}, plant{plant}, numVendingMachines{numVendingMachines}, maxStockPerFlavour{maxStockPerFlavour} {}

// function to sum the total number of sodas in Truck's cargo array
unsigned int Truck::total(unsigned int *arr) {
    unsigned int tot = 0;
    for(int i=0; i<4; i++) { tot += arr[i]; }
    return tot;
}

void Truck::main() {
    printer.print(Printer::Kind::Truck, 'S');
    int lastMachine = -1;
    VendingMachine **machines;
    machines = nameServer.getMachineList(); // begin by getting list of all vending machines from NameServer
    unsigned int shipment[numVendingMachines];
    for(;;) {
        _Accept(~Truck) {
            break;
        }
        _Else { // this Else is not busy waiting since getShipment is a blocking call if no shipment is ready from Bottling plant
            yield(prng(1, 10)); // get coffee from Tom Hortons
            try { _Enable {
                    plant.getShipment(shipment); // get shipment from Bottling Plant
                }
            } catch(const BottlingPlant::Shutdown &) {
                break;
            }
            printer.print(Printer::Kind::Truck, 'P', total(shipment));
            // cycle through all vending machines not including the last one that was serviced
            for(unsigned int i=1; i<=numVendingMachines && total(shipment)!=0; i++) {
                unsigned int currMachine = (lastMachine + i) % numVendingMachines;
                printer.print(Printer::Kind::Truck, 'd', currMachine, total(shipment));
                unsigned int *inventory = machines[currMachine]->inventory();
                unsigned int unfilledStock = 0;
                // fill machine up with new inventory
                for(int j=0; j<4; j++) {
                    unsigned int amtAdd = min(shipment[j], maxStockPerFlavour-inventory[j]);
                    inventory[j] += amtAdd;
                    shipment[j] -= amtAdd;
                    unfilledStock += maxStockPerFlavour - inventory[j];
                }
                if (unfilledStock > 0) { // unfilled stock message
                    printer.print(Printer::Kind::Truck, 'U', currMachine, unfilledStock);
                }
                printer.print(Printer::Kind::Truck, 'D', currMachine, total(shipment));
                machines[currMachine]->restocked();
                if(total(shipment) == 0) { // no more soda left on the truck, set last machine and break
                    lastMachine = currMachine;
                    break;
                }
            }
            // 1 in 100 chance of a flat tire, yield and fix
            if(prng(100)==0) {
                printer.print(Printer::Kind::Truck, 'W');
                yield(10);
            }
        }
    }
    printer.print(Printer::Kind::Truck, 'F');
}


