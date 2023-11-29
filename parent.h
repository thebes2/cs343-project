#ifndef _PARENT_H_
#define _PARENT_H_

#include "bank.h"
#include "printer.h"

_Task Parent {
    Printer &printer;
    Bank& bank;
    unsigned int numStudents, parentalDelay;
	void main();
  public:
	Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay );
};

#endif