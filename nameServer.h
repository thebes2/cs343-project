#ifndef _NAME_SERVER_H
#define _NAME_SERVER_H

#include "printer.h"

_Task VendingMachine;

_Task NameServer {
    void main();
  public:
    NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents );
    void VMregister( VendingMachine * vendingmachine );
    VendingMachine *getMachine( unsigned int id ) __attribute__(( warn_unused_result ));
    VendingMachine **getMachineList() __attribute__(( warn_unused_result ));
};

#endif