#ifndef _WATCARDOFFICE_H_
#define _WATCARDOFFICE_H_

#include "watcard.h"
#include "bank.h"
#include "printer.h"

_Task WATCardOffice {
    struct Args {
        unsigned int id, amount;
    };
	struct Job {							// marshalled arguments and return future
		Args args;							// call arguments (YOU DEFINE "Args")
		WATCard::FWATCard result;			// return future
		Job( Args args ) : args( args ) {}
	};
	_Task Courier {                         // communicates with bank
        WATCardOffice &office;
        Bank &bank;
        void main();
    public:
        Courier(WATCardOffice& office, Bank &bank) = default;
    };

    uCondition courierPool;

	void main();
  public:
	_Event Lost {};							// lost WATCard
	WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers );
	WATCard::FWATCard create( unsigned int sid, unsigned int amount ) __attribute__(( warn_unused_result ));
	WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card ) __attribute__(( warn_unused_result ));
	Job * requestWork() __attribute__(( warn_unused_result ));
};

#endif