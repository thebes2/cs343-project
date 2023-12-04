#include "bottlingPlant.h"
#include "truck.h"
using namespace std;

void BottlingPlant::getShipment(unsigned int cargo[]) {
    loadingCargo = cargo;
    // sync call, go to sleep
    bench.wait((uintptr_t)(void*)&uThisTask());
}

BottlingPlant::BottlingPlant(Printer & prt, NameServer & nameServer, unsigned int numVendingMachines, unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments) :
printer{prt}, nameServer{nameServer}, numVendingMachines{numVendingMachines}, maxShippedPerFlavour{maxShippedPerFlavour}, maxStockPerFlavour{maxStockPerFlavour}, timeBetweenShipments{timeBetweenShipments}, 
randomQuantity{0}, numFlavours{4} {}

void BottlingPlant::main() {
    printer.print(Printer::Kind::BottlingPlant, 'S');
    // create truck
    Truck driver(printer, nameServer, *this, numVendingMachines, maxStockPerFlavour);
    for(;;) {
        if(randomQuantity==0) {
            // perform production run and yield
            yield(timeBetweenShipments);
            for(unsigned int i=0; i<numFlavours; i++) {
                currentStock[i] = prng(0, maxShippedPerFlavour);
                // printf("bottling plant stock run: %d\n", currentStock[i]);
                randomQuantity += currentStock[i];
            }
            printer.print(Printer::Kind::BottlingPlant, 'G', randomQuantity);
        }
        _Accept(~BottlingPlant) {
            _Accept(getShipment) {
                _Resume Shutdown{} _At *(Truck*)(void*)bench.front();
                bench.signalBlock();
            }
            break;
        } or _Accept(getShipment) {
            for(unsigned int i=0; i<numFlavours; i++) {
                loadingCargo[i] = currentStock[i];
                randomQuantity -= loadingCargo[i];
                currentStock[i] = 0;
            }
            bench.signalBlock();
            printer.print(Printer::Kind::BottlingPlant, 'P');
        }
    }
    printer.print(Printer::Kind::BottlingPlant, 'F');
}
