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
            // actual card in the giftcard future is on the heap
            WATCard *card = new WATCard;
            card->deposit(sodaCost);
            uSeqIter<FWATCardNode> seqIterJob;
            FWATCardNode *curr;
            unsigned int i = 0;
            unsigned int cnt = prng(counter);
            // printf("cnt prng for giving gc: %d\n", cnt);
            for(seqIterJob.over(futures); seqIterJob>>curr;) {
                if(i==cnt) {
                    curr->card.delivery(card);
                    printer.print(Printer::Kind::Groupoff, 'D', sodaCost);
                    futures.remove(curr);
                    delete curr;
                    counter--;
                    break;
                }
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
