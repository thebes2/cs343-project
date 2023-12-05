#ifndef _WATCARDOFFICE_H_
#define _WATCARDOFFICE_H_

#include "watCard.h"
#include "bank.h"
#include "printer.h"
#include <uFuture.h>

_Task WATCardOffice {
	struct Args {
		unsigned int id, amount;
		WATCard *card;
		Args(unsigned int id, unsigned int amount, WATCard *card) : id{id}, amount{amount}, card{card} {}
		Args():id{0}, amount{0}, card{nullptr} {}
	};
	// inherit from uColable to use in uQueue list to manage jobs
	struct Job : uColable {							// marshalled arguments and return future
		Args args;							// call arguments (YOU DEFINE "Args")
		WATCard::FWATCard result;			// return future
		Job( Args args ) : args( args ) {}
		Job() : args{} {}
	};
	_Task Courier {                         // communicates with bank
		Printer &printer;
        WATCardOffice &office;
        Bank &bank;
        void main();
		unsigned int id;
      public:
        Courier(Printer & printer, WATCardOffice& office, Bank &bank, unsigned int id);
    };
	// Card is a wrapped for WATcard to allow it to be stored in a uQueue list to manage termination at the end
	struct Card: uColable {
		WATCard *card;
		Card(WATCard *card) : card{card} {}
	};

	uQueue<Job> jobs; // manage list of jobs for couriers
	uQueue<Card> cards; // manage list of all WATCards
	WATCard::FWATCard result;

	Printer &printer;
	Bank &bank;
	unsigned int numCouriers, currSID, currAmount, activeCouriers, jobCounter;
	WATCard *currCard;
	Courier **courierPool; // array of pointers to Courier Task objects
	uCondition bench; // bench used to momentarily put to sleep other Task's accessing public mutex members

	_Event Empty{}; // Empty exception used to let couriers know there are no more jobs to work and let them terminate

	void main();
  public:
	_Event Lost {};							// lost WATCard
	WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers );
	~WATCardOffice();
	WATCard::FWATCard create( unsigned int sid, unsigned int amount ) __attribute__(( warn_unused_result ));
	WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card ) __attribute__(( warn_unused_result ));
	Job * requestWork() __attribute__(( warn_unused_result ));
};

#endif