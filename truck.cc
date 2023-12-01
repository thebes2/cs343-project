#include "truck.h"
#include "nameServer.h"
#include "vendingMachine.h"
#include "uPRNG.h"
#include "bottlingPlant.h"
using namespace std;

Truck::Truck(Printer & prt, NameServer & nameServer, BottlingPlant & plant, unsigned int numVendingMachines, unsigned int maxStockPerFlavour) : 
printer{prt}, nameServer{nameServer}, plant{plant}, numVendingMachines{numVendingMachines}, maxStockPerFlavour{maxStockPerFlavour} {}

bool Truck::empty(unsigned int *arr) {
    for(int i=0; i<4; i++) {
        if(arr[i]!=0) return false;
    }
    return true;
}

void Truck::main() {
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
            for(unsigned int i=((lastMachine + 1) % numVendingMachines); i<numVendingMachines; i++) {
                unsigned int *inventory = machines[i]->inventory();
                for(int j=0; j<4; j++) {
                    unsigned int amtAdd = min(shipment[j], maxStockPerFlavour-inventory[j]);
                    inventory[j] += amtAdd;
                    shipment[j] -= amtAdd;
                }
                machines[i]->restocked();
                if(empty(shipment)) break;
            }
            if(prng(100)==0) yield(10);
        }
    }
}


