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
	// refactor Job below to directly inherit from uSerqable
	struct Job : uSeqable {							// marshalled arguments and return future
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
	
	struct Card: uSeqable {
		WATCard *card;
		Card(WATCard *card) : card{card} {}
	};

	uSequence<Job> jobs;
	uSequence<Card> cards;
	WATCard::FWATCard result;

	Printer &printer;
	Bank &bank;
	unsigned int numCouriers, currSID, currAmount, activeCouriers, jobCounter;
	WATCard *currCard;
	Courier **courierPool;
	uCondition bench;

	_Event Empty{};

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