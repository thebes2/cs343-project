#include <algorithm>
#include "groupoff.h"
using namespace std;

Groupoff::Groupoff(Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay)
    : printer(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay), counter(0),
      order(new unsigned int[numStudents]) {
    // set order that the gift cards will be filled
    for (unsigned int i=0;i<numStudents;i++) { order[i] = i; }
    for (unsigned int i=0;i<numStudents;i++) {
        swap(order[i], order[prng(numStudents)]);        
    }
}

Groupoff::~Groupoff() {
    delete[] order;
    // delete[] futures; // incomplete futures are the problem of the client now
    uSeqIter<FWATCardNode> seqIterJob;
    FWATCardNode *curr;
    for(seqIterJob.over(futures); seqIterJob>>curr;) {
        futures.remove(curr);
        delete curr;
    }
}

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
            unsigned int index = 0;
            for(seqIterJob.over(futures); seqIterJob>>curr;) {
                if(index==order[i]) {
                    curr->card.delivery(card);
                    break;
                }
                index++;
            }
        }
    }
    printer.print(Printer::Kind::Groupoff, 'F');
}

