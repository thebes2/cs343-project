#include "bottlingPlant.h"
#include "truck.h"
using namespace std;

// public method called by Truck to get shipment of sodas
// to improve concurrency it goes to sleep on a bench momentarily for Bottling Plant main to copy over sodas to cargo array
void BottlingPlant::getShipment(unsigned int cargo[]) {
    loadingCargo = cargo; //save reference to cargo array on Bottling Plant stack for main to access
    bench.wait((uintptr_t)(void*)&uThisTask()); // sync call, go to sleep with address of client in case of exceptions
}

// ctor
BottlingPlant::BottlingPlant(Printer & prt, NameServer & nameServer, unsigned int numVendingMachines, unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments) :
printer{prt}, nameServer{nameServer}, numVendingMachines{numVendingMachines}, maxShippedPerFlavour{maxShippedPerFlavour}, maxStockPerFlavour{maxStockPerFlavour}, timeBetweenShipments{timeBetweenShipments}, 
randomQuantity{0}, numFlavours{4} {}


void BottlingPlant::main() {
    printer.print(Printer::Kind::BottlingPlant, 'S');
    Truck driver(printer, nameServer, *this, numVendingMachines, maxStockPerFlavour); // create truck
    for(;;) {
        if(randomQuantity==0) { // perform production run and yield
            yield(timeBetweenShipments);
            for(unsigned int i=0; i<numFlavours; i++) {
                // generate random number of sodas for each flavour
                currentStock[i] = prng(0, maxShippedPerFlavour);
                randomQuantity += currentStock[i];
            }
            printer.print(Printer::Kind::BottlingPlant, 'G', randomQuantity);
        }
        _Accept(~BottlingPlant) {
            // if outstanding call to getShiment, accept the truck and throw shutdown exception at it
            _Accept(getShipment) {
                _Resume Shutdown{} _At *(Truck*)(void*)bench.front();
                bench.signalBlock(); // signalBlock to wake up Truck and let it return with an exception
            }
            break;
        } or _Accept(getShipment) { // if not dtor, accept call to getShipment from truck
            // copy over cargo from current stock to Truck's cargo array
            for(unsigned int i=0; i<numFlavours; i++) {
                loadingCargo[i] = currentStock[i];
                randomQuantity -= loadingCargo[i];
                currentStock[i] = 0;
            }
            bench.signalBlock(); // signal block to allow Truck to return with its new cargo
            printer.print(Printer::Kind::BottlingPlant, 'P');
        }
    }
    printer.print(Printer::Kind::BottlingPlant, 'F');
}
