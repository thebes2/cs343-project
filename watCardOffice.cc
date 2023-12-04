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
            Job * request;
            try { _Enable {
                request = office.requestWork();
                }
            } catch (const WATCardOffice::Empty &) {
                break;
            }
            unsigned int studentId = request->args.id, amount = request->args.amount;
            printer.print(Printer::Kind::Courier, id, 't', studentId, amount);
            bank.withdraw(studentId, amount);
            request->args.card->deposit(amount);
            if (prng(0,5)==0) { // lost WATCard
                printer.print(Printer::Kind::Courier, id, 'L', studentId);
                request->result.delivery( new Lost{} );

            }
            else {
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
    currSID = sid;
    currAmount = amount;
    WATCard *newCard = new WATCard;
    cards.add(new Card{newCard});
    Job *newCardJob = new Job{Args{sid, amount, newCard}};
    jobs.add(newCardJob);
    return newCardJob->result;
}

WATCard::FWATCard WATCardOffice::transfer(unsigned int sid, unsigned int amount, WATCard * card) {
    currSID = sid;
    currAmount = amount;
    Job *transferJob = new Job{Args{sid, amount, card}};
    jobs.add(transferJob);
    return transferJob->result;
}

WATCardOffice::Job * WATCardOffice::requestWork() {
    uRendezvousAcceptor();
    if(jobs.empty()) _Throw Empty{};
    return jobs.dropHead();
}

void WATCardOffice::main() {

    printer.print(Printer::Kind::WATCardOffice, 'S');
    for(;;) {
        _Accept(~WATCardOffice) {
            for(;;) {
                _Accept(requestWork) {}
                _Else {break;}
            }
            break;
        } or _When(!jobs.empty()) _Accept(requestWork) {
            printer.print(Printer::Kind::WATCardOffice, 'W');
        } or _Accept(transfer) {
            printer.print(Printer::Kind::WATCardOffice, 'T', currSID, currAmount);
        } or _Accept(create) {
            printer.print(Printer::Kind::WATCardOffice, 'C', currSID, currAmount);
        }
    }
    printer.print(Printer::Kind::WATCardOffice, 'F');
}

WATCardOffice::WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers )
  : printer{prt}, bank{bank}, numCouriers{numCouriers}, courierPool{new Courier*[numCouriers]} {
    for(unsigned int i=0; i<numCouriers; i++) {
        courierPool[i] = new Courier{printer, *this, bank, i};
    }
}

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
    uSeqIter<Job> seqIterJob;
    Job * jp;
    for(seqIterJob.over(jobs); seqIterJob>>jp;) {
        jobs.remove(jp);
        delete jp;
    }
}
