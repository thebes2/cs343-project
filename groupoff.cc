#include <algorithm>
#include "groupoff.h"
using namespace std;

Groupoff::Groupoff(Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay)
    : printer(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay), counter(0),
      order(new unsigned int[numStudents]), futures(new WATCard::FWATCard*[numStudents]) {
    // set order that the gift cards will be filled
    for (unsigned int i=0;i<numStudents;i++) { order[i] = i; }
    for (unsigned int i=0;i<numStudents;i++) {
        swap(order[i], order[prng(numStudents)]);        
    }
}

Groupoff::~Groupoff() {
    delete[] order;
    delete[] futures; // incomplete futures are the problem of the client now
}

void Groupoff::main() {
    for (counter=0;counter<numStudents;counter++) { _Accept(giftCard); }
    for (unsigned int i=0;i<numStudents;i++) {
        for (unsigned int j=0;j<groupoffDelay;j++) {
            _Accept(~Groupoff) { goto shutdown; }
            _Else { yield(1); }
        }
        WATCard card = new WATCard;
        card.deposit(sodaCost);
        futures[order[i]]->result.delivery(card);
    } shutdown:
}

WATCard::FWATCard giftCard() {
    WATCard::FWATCard *result = new WATCard::FWATCard;
    futures[counter] = result;
    return result;
}