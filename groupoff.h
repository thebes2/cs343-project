#ifndef _GROUPOFF_H_
#define _GROUPOFF_H_

#include "printer.h"
#include "watCard.h"

_Task Groupoff {
    Printer &printer;
    unsigned int numStudents, sodaCost, groupoffDelay, counter;
    // wrapper for future giftcard that inherits from uColable to allow it to be stored in a uQueue list
    struct FWATCardNode : uColable {
        WATCard::FWATCard card; 
        bool delivered = false;
        FWATCardNode(WATCard::FWATCard card) : card{card} {}
    };
    WATCard::FWATCard currentCard; // temporary variable to store current card
    uQueue<FWATCardNode> futures; // uQueue list to store futures of giftcards created by Groupoff
	  void main();
  public:
	  Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
	  ~Groupoff();
    WATCard::FWATCard giftCard();
};

#endif