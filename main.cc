#include <iostream>
#include "printer.h"
#include "watCardOffice.h"
#include "config.h"
#include "groupoff.h"
#include "student.h"
#include "vendingMachine.h"

using namespace std;

void uMain::main() {
    ConfigParms c;
    Printer printer(c.numStudents, c.numVendingMachines, c.numCouriers);
    Bank bank(c.numStudents);
    WATCardOffice office(printer, bank, c.numCouriers);
    Groupoff g(printer, c.numStudents, c.sodaCost, c.groupoffDelay);
    NameServer nameServer(printer, c.numVendingMachines, c.numStudents);
    BottlingPlant bottlingPlant(printer, nameServer, c.numVendingMachines, c.maxShippedPerFlavour, c.maxStockPerFlavour, c.timeBetweenShipments);

    VendingMachine *machines[c.numVendingMachines];
    Student *students[c.numStudents];
    for(unsigned int i=0; i<c.numVendingMachines; i++) {
        machines[i] = new VendingMachine(printer, nameServer, i, c.sodaCost);
    }
    for(unsigned int i=0; i<c.numStudents; i++) {
        students[i] = new Student(printer, nameServer, office, g, i, c.maxPurchases);
    }

    for(unsigned int i=0; i<c.numVendingMachines; i++) {
        delete machines[i];
    }
    for(unsigned int i=0; i<c.numStudents; i++) {
        delete students[i];
    }

}