#include "watCardOffice.h"
#include "printer.h"
#include "bank.h"

WATCardOffice::Courier::Courier(WATCardOffice& office, Bank& bank)
    : office(office), bank(bank) {}

void WATCardOffice::Courier::main() {
    // _Else below might be a really bad way of doing this
    // Maybe requestWork can return nullptr to indicate no work
    for (;;) {
        _Accept(~Courier) {break;}
        _Else {
            Job* request = office.requestWork();
            unsigned int id = request->args.id, amount = request->args.amount;
            bank.withdraw(id, amount);
            if (prng(6) == 0) { // lost WATCard
                request->result.delivery( new Lost{} );
            }
            else {
                request->args.card->deposit(amount);
                request->result.delivery(request->args.card);
            }
        }
    }
}


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
        courierPool[i] = new Courier{*this, bank};
    }
    for(;;) {
        _Accept(~WATCardOffice) {
            break;
        } or _When(!jobs.empty()) _Accept(requestWork) {
            jobNode * curr = jobs.dropHead();
            delete curr->job;
            delete curr;
        } or _Accept(transfer) {

        } or _Accept(create) {

        }
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
    uSeqIter<jobNode> seqIterJob;
    jobNode * jp;
    for(seqIterJob.over(jobs); seqIterJob>>jp;) {
        delete jp->job;
        jobs.remove(jp);
        delete jp;
    }
}
