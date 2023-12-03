#include <algorithm>
#include "groupoff.h"
#include <iostream>
using namespace std;

Groupoff::Groupoff(Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay)
    : printer(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay), counter(0) { }

WATCard::FWATCard Groupoff::giftCard() {
    WATCard::FWATCard newCard;
    currentCard = newCard;
    return currentCard;
}

void Groupoff::main() {
    printer.print(Printer::Kind::Groupoff, 'S');
    for(counter=0; counter<numStudents; counter++) {
        _Accept(giftCard) {
            futures.add(new FWATCardNode{currentCard});
        }
    }
    for(unsigned int i=0; i<numStudents; i++) {
        _Accept(~Groupoff) {
            break;
        } _Else {
            yield(groupoffDelay);
            printer.print(Printer::Kind::Groupoff, 'D', sodaCost);
            // actual card in the giftcard future is on the heap
            WATCard *card = new WATCard;
            card->deposit(sodaCost);
            uSeqIter<FWATCardNode> seqIterJob;
            FWATCardNode *curr;
            unsigned int cnt = prng(numStudents - i);
            for(seqIterJob.over(futures); seqIterJob>>curr;) {
                if (cnt == 0 && !curr->delivered) {
                    curr->delivered = true;
                    curr->card.delivery(card);
                    break;
                }
                if (!curr->delivered) { cnt --; }
            }
        }
    }
    printer.print(Printer::Kind::Groupoff, 'F');
}


Groupoff::~Groupoff() {
    uSeqIter<FWATCardNode> seqIterJob;
    FWATCardNode * fp;
    for(seqIterJob.over(futures); seqIterJob>>fp;) {
        futures.remove(fp);
        delete fp;
    }
}
