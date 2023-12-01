#ifndef _STUDENT_H_
#define _STUDENT_H_

#include "printer.h"
#include "nameServer.h"
#include "watCardOffice.h"
#include "groupoff.h"

_Task Student {
	void main();
  public:
	Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
			 unsigned int id, unsigned int maxPurchases );
};

#endif