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
    currRegistering = vendingMachine;
}

VendingMachine * NameServer::getMachine(unsigned int id) {
    currStudentId = id; 
    return registered[assignments[id]];
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
            registered[currRegistering->getId()] = currRegistering;
            currVMId = currRegistering->getId();
            numRegistered ++;
            printer.print(Printer::Kind::NameServer, 'R', currVMId);
        } or _When(numRegistered == numVendingMachines) _Accept(getMachine) {
            currVMId = assignments[currStudentId];
            assignments[currStudentId] = (assignments[currStudentId] + 1) % numVendingMachines;
            printer.print(Printer::Kind::NameServer, 'N', currStudentId, currVMId);
        } or _When(numRegistered == numVendingMachines) _Accept(getMachineList) {

        }
    }
    printer.print(Printer::Kind::NameServer, 'F');
}

NameServer::~NameServer() {
    delete[] registered;
    delete[] assignments;
}