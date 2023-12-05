#include <algorithm>
#include "groupoff.h"
#include <iostream>
using namespace std;

// ctor
Groupoff::Groupoff(Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay)
    : printer(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay), counter(0) { }

// public mutex member to give Giftcard future to student
WATCard::FWATCard Groupoff::giftCard() {
    WATCard::FWATCard newCard;
    currentCard = newCard;
    return currentCard;
}

void Groupoff::main() {
    printer.print(Printer::Kind::Groupoff, 'S');
    // let all students call and request a giftcard
    for(counter=0; counter<numStudents; counter++) {
        _Accept(giftCard) {
            futures.add(new FWATCardNode{currentCard}); // add giftcard to uQueue list
        }
    }
    for(unsigned int i=0; i<numStudents; i++) {
        _Accept(~Groupoff) {
            break;
        } _Else {  // this Else is not busy waiting since it is bounded by number of students in the for loop encapsulating it
            yield(groupoffDelay);
            // actual card in the giftcard future is on the heap
            WATCard *card = new WATCard;
            // deposit money into the giftcard
            card->deposit(sodaCost);
            // choose random student to give loaded giftcard to
            uQueueIter<FWATCardNode> qIterJob;
            FWATCardNode *curr;
            unsigned int i = 0;
            unsigned int cnt = prng(counter);
            // iterate over list to give random giftcard to student
            for(qIterJob.over(futures); qIterJob>>curr;) {
                if(i==cnt) {
                    // only deliver the future giftcard if hasn't been cancelled
                    if (!curr->card.cancelled()) {
                        curr->card.delivery(card);
                    }
                    else {
                        // otherwise delete the card since the student that wanted to use it has been terminated
                        delete card;
                    }
                    printer.print(Printer::Kind::Groupoff, 'D', sodaCost);
                    futures.remove(curr);
                    delete curr;
                    counter--;
                    break;
                }
                i++;
            }
        }
    }
    printer.print(Printer::Kind::Groupoff, 'F');
}


Groupoff::~Groupoff() {
    // iterate all the giftcard nodes in uQueue list and delete the nodes
    uQueueIter<FWATCardNode> qIterJob;
    FWATCardNode * fp;
    for(qIterJob.over(futures); qIterJob>>fp;) {
        futures.remove(fp); // remove from list
        delete fp; // delete the removed node
    }
}
