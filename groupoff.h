#ifndef _GROUPOFF_H_
#define _GROUPOFF_H_

#include "printer.h"
#include "watCard.h"

_Task Groupoff {
    Printer &printer;
    unsigned int numStudents, sodaCost, groupoffDelay, counter;
    struct FWATCardNode : uSeqable {
        WATCard::FWATCard card; 
        bool delivered = false;
        FWATCardNode(WATCard::FWATCard card) : card{card} {}
    };
    WATCard::FWATCard currentCard;
    uSequence<FWATCardNode> futures;
    //WATCard::FWATCard **futures;
	  void main();
  public:
	  Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
	  ~Groupoff();
    WATCard::FWATCard giftCard();
};

#endif