#include "nameServer.h"
#include "vendingMachine.h"
#include "printer.h"


NameServer::NameServer(Printer & prt, unsigned int numVendingMachines, unsigned int numStudents) :
printer{prt}, numVendingMachines{numVendingMachines}, numStudents{numStudents}, numRegistered{0}, currVMId{0}, currStudentId{0}, registered{new VendingMachine*[numVendingMachines]} {
    assignments = new int[numStudents];
    int currMachine = 0;
    for(unsigned int i=0; i<numStudents; i++) {
        assignments[i] = (currMachine % numVendingMachines);
        currMachine += 1;
    }
}

void NameServer::VMregister(VendingMachine *vendingMachine) {
    registered[vendingMachine->getId()] = vendingMachine;
    currVMId = vendingMachine->getId();
    numRegistered ++;
}

VendingMachine * NameServer::getMachine(unsigned int id) {
    VendingMachine *gopro = registered[assignments[id]];
    currStudentId = id; currVMId = assignments[id];
    assignments[id] = (assignments[id] + 1) % numVendingMachines;
    return gopro;
}

VendingMachine ** NameServer::getMachineList() {
    return registered;
}


void NameServer::main() {
    printer.print(Printer::Kind::NameServer, 'S');
    for(;;) {
        _Accept(~NameServer) {
            break;
        } or _When(numRegistered < numVendingMachines) _Accept(VMregister) {
            printer.print(Printer::Kind::NameServer, 'R', currVMId);
        } or _When(numRegistered == numVendingMachines) _Accept(getMachine) {
            printer.print(Printer::Kind::NameServer, 'N', currStudentId, currVMId);
        } or _When(numRegistered == numVendingMachines) _Accept(getMachineList) {

        }
    }
    printer.print(Printer::Kind::NameServer, 'F');
}

NameServer::~NameServer() {}