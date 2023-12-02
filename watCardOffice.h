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
        Courier(WATCardOffice& office, Bank &bank);
    };
	struct jobNode : uSeqable {
		Job *job;
		jobNode(Job *job) : job{job} {}
	};
	struct Card: uSeqable {
		WATCard *card;
		Card(WATCard *card) : card{card} {}
	};

	uSequence<jobNode> jobs;
	uSequence<Card> cards;
	WATCard::FWATCard result;

	Printer &printer;
	Bank &bank;
	unsigned int numCouriers;
	Courier **courierPool;

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