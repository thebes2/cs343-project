#include "truck.h"
#include "nameServer.h"
#include "vendingMachine.h"
#include "uPRNG.h"
#include "bottlingPlant.h"
using namespace std;

Truck::Truck(Printer & prt, NameServer & nameServer, BottlingPlant & plant, unsigned int numVendingMachines, unsigned int maxStockPerFlavour) : 
printer{prt}, nameServer{nameServer}, plant{plant}, numVendingMachines{numVendingMachines}, maxStockPerFlavour{maxStockPerFlavour} {}

bool Truck::total(unsigned int *arr) {
    unsigned int tot = 0;
    for(int i=0; i<4; i++) { tot += arr[i]; }
    return tot;
}

void Truck::main() {
    printer.print(Printer::Kind::Truck, 'S');
    int lastMachine = -1;
    VendingMachine **machines;
    machines = nameServer.getMachineList();
    unsigned int shipment[numVendingMachines];
    for(;;) {
        yield(prng(1, 11));
        _Accept(~Truck) {
            break;
        }
        _Else{
            try {
                plant.getShipment(shipment);
            } catch(const BottlingPlant::Shutdown &) {
                break;
            }
            printer.print(Printer::Kind::Truck, 'P', total(shipment));
            for(unsigned int i=1; i<=numVendingMachines; i++) {
                unsigned int currMachine = (lastMachine + i) % numVendingMachines;
                printer.print(Printer::Kind::Truck, 'd', currMachine, total(shipment));
                unsigned int *inventory = machines[currMachine]->inventory();
                unsigned int unfilledStock = 0;
                for(int j=0; j<4; j++) {
                    unsigned int amtAdd = min(shipment[j], maxStockPerFlavour-inventory[j]);
                    inventory[j] += amtAdd;
                    shipment[j] -= amtAdd;
                    unfilledStock += maxStockPerFlavour - inventory[j];
                }
                if (unfilledStock > 0) {
                    printer.print(Printer::Kind::Truck, 'U', currMachine, unfilledStock);
                }
                machines[i]->restocked();
                printer.print(Printer::Kind::Truck, 'D', currMachine, total(shipment));
                if(total(shipment) == 0) {
                    lastMachine = currMachine;
                    break;
                }
            }
            if(prng(100)==0) {
                printer.print(Printer::Kind::Truck, 'W');
                yield(10);
            }
        }
    }
    printer.print(Printer::Kind::Truck, 'F');
}


