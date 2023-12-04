#ifndef _STUDENT_H_
#define _STUDENT_H_

#include "printer.h"
#include "nameServer.h"
#include "watCardOffice.h"
#include "groupoff.h"

_Task Student {
	Printer &printer;
	NameServer &nameServer;
	WATCardOffice &office;
	Groupoff &groupoff;
	unsigned int id, maxPurchases, numTimes, favouriteFlavour;
	WATCard::FWATCard watcard;
	WATCard::FWATCard giftcard;
	VendingMachine *currentVM;
	unsigned int sodas = 0;
	void main();
  public:
	Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
			 unsigned int id, unsigned int maxPurchases );
};

#endif