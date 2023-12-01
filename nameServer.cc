#include "nameServer.h"
#include "vendingMachine.h"


NameServer::NameServer(Printer & prt, unsigned int numVendingMachines, unsigned int numStudents) :
printer{prt}, numVendingMachines{numVendingMachines}, numStudents{numStudents}, numRegistered{0}, registered{new VendingMachine*[numVendingMachines]} {
    assignments = new int[numStudents];
    int currMachine = 0;
    for(unsigned int i=0; i<numStudents; i++) {
        assignments[i] = (currMachine % numVendingMachines);
        currMachine += 1;
    }
}

void NameServer::VMregister(VendingMachine *vendingMachine) {
    registered[vendingMachine->getId()] = vendingMachine;
}

VendingMachine * NameServer::getMachine(unsigned int id) {
    VendingMachine *gopro = registered[assignments[id]];
    assignments[id] = (assignments[id] + 1) % numVendingMachines;
    return gopro;
}

VendingMachine ** NameServer::getMachineList() {
    return registered;
}

void NameServer::main() {
    for(;;) {
        _Accept(~NameServer) {

        } or _When(numRegistered < numVendingMachines) _Accept(VMregister) {

        } or _When(numRegistered == numVendingMachines) _Accept(getMachine || getMachineList) {

        }
    }
}

