#include "watCardOffice.h"
#include "printer.h"
#include "bank.h"

#include <iostream>

WATCardOffice::Courier::Courier(Printer &printer, WATCardOffice& office, Bank& bank, unsigned int id)
    : printer{printer}, office(office), bank(bank), id{id} {}

void WATCardOffice::Courier::main() {
    // _Else below might be a really bad way of doing this
    // Maybe requestWork can return nullptr to indicate no work
    printer.print(Printer::Kind::Courier, id, 'S');
    for (;;) {
        _Accept(~Courier) {break;}
        _Else {
            // not busy waiting since requestWork blocks if no work
            Job* request = office.requestWork();
            unsigned int studentId = request->args.id, amount = request->args.amount;
            printer.print(Printer::Kind::Courier, id, 't', studentId, amount);
            bank.withdraw(studentId, amount);
            if (prng(6) == 0) { // lost WATCard
                printer.print(Printer::Kind::Courier, id, 'L', studentId);
                request->result.delivery( new Lost{} );

            }
            else {
                request->args.card->deposit(amount);
                request->result.delivery(request->args.card);
                printer.print(Printer::Kind::Courier, id, 'T', studentId, amount);
            }
            delete request;
        }
    }
    printer.print(Printer::Kind::Courier, id, 'F');
}

// to-do: make functions below as lean as possible by moving work to main function
WATCard::FWATCard WATCardOffice::create(unsigned int sid, unsigned int amount) {
    WATCard *newCard = new WATCard;
    cards.add(new Card{newCard});
    Job *newCardJob = new Job{Args{sid, amount, newCard}};
    jobs.add(new jobNode{newCardJob});
    return newCardJob->result;
}

WATCard::FWATCard WATCardOffice::transfer(unsigned int sid, unsigned int amount, WATCard * card) {
    Job *transferJob = new Job{Args{sid, amount, card}};
    jobs.add(new jobNode{transferJob});
    return transferJob->result;
}

WATCardOffice::Job * WATCardOffice::requestWork() {
    return jobs.head()->job;
}

void WATCardOffice::main() {
    for(unsigned int i=0; i<numCouriers; i++) {
        courierPool[i] = new Courier{printer, *this, bank, i};
    }
    for(;;) {
        _Accept(~WATCardOffice) {
            break;
        } or _When(!jobs.empty()) _Accept(requestWork) {
            jobNode * curr = jobs.dropHead();
            delete curr;
        } or _Accept(transfer) {

        } or _Accept(create) {

        }
    }
}

WATCardOffice::WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers )
    : printer{prt}, bank{bank}, numCouriers{numCouriers}, courierPool{new Courier*[numCouriers]} {}

WATCardOffice::~WATCardOffice() {
    // delete courier pool
    for(unsigned int i=0; i<numCouriers; i++) {
        delete courierPool[i];
    }
    delete[] courierPool;

    // delete watcards
    uSeqIter<Card> seqIterCard;
    Card * cp;
    for(seqIterCard.over(cards); seqIterCard>>cp;) {
        delete cp->card;
        cards.remove(cp);
        delete cp;
    }
    
    //delete outstanding jobs?
    uSeqIter<jobNode> seqIterJob;
    jobNode * jp;
    for(seqIterJob.over(jobs); seqIterJob>>jp;) {
        delete jp->job;
        jobs.remove(jp);
        delete jp;
    }
}
