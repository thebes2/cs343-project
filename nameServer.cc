#include "nameServer.h"
#include "vendingMachine.h"
#include "printer.h"

// ctor; initialize array of vending machines that have yet to be registered and assign a vending machine to each student
NameServer::NameServer(Printer & prt, unsigned int numVendingMachines, unsigned int numStudents) :
printer{prt}, numVendingMachines{numVendingMachines}, numStudents{numStudents}, numRegistered{0}, currVMId{0}, currStudentId{0}, registered{new VendingMachine*[numVendingMachines]} {
    assignments = new int[numStudents];
    int currMachine = 0;
    // assign vending machine in round robin fashion
    for(unsigned int i=0; i<numStudents; i++) {
        assignments[i] = (currMachine % numVendingMachines);
        currMachine += 1;
    }
}

// public mutex member to register vending machine
void NameServer::VMregister(VendingMachine *vendingMachine) {
    currRegistering = vendingMachine;
}

// public mutex member to return an assigned machine to the student with a given id
VendingMachine * NameServer::getMachine(unsigned int id) {
    currStudentId = id; 
    return registered[assignments[id]];
}

// public mutex member to return array of all registered machines
VendingMachine ** NameServer::getMachineList() {
    return registered;
}

// server's main
void NameServer::main() {
    printer.print(Printer::Kind::NameServer, 'S');
    for(;;) {
        _Accept(~NameServer) {
            break;
        } or _When(numRegistered < numVendingMachines) _Accept(VMregister) { // prioritize calls to register a vending machine
            // do most of the work after accepting the call
            registered[currRegistering->getId()] = currRegistering;
            currVMId = currRegistering->getId();
            numRegistered ++;
            printer.print(Printer::Kind::NameServer, 'R', currVMId);
        } or _When(numRegistered == numVendingMachines) _Accept(getMachineList) {
            // nothing to be done for returning a list of all machines to truck
        } or _When(numRegistered == numVendingMachines) _Accept(getMachine) {
            // for the current student who was given a machine, change their assignment to a new vending machine based on modulo incrementing
            currVMId = assignments[currStudentId];
            assignments[currStudentId] = (assignments[currStudentId] + 1) % numVendingMachines;
            printer.print(Printer::Kind::NameServer, 'N', currStudentId, currVMId);
        }
    }
    printer.print(Printer::Kind::NameServer, 'F');
}

NameServer::~NameServer() {
    // free memory for heap arrays
    delete[] registered;
    delete[] assignments;
}