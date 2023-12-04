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
    currSID = sid; currAmount = amount;
    return jobs.tail()->result;
}

WATCard::FWATCard WATCardOffice::transfer(unsigned int sid, unsigned int amount, WATCard * card) {
    currSID = sid; currAmount = amount; currCard = card;
    return jobs.tail()->result;
}

WATCardOffice::Job * WATCardOffice::requestWork() {
    bench.wait((uintptr_t)(void*)&uThisTask());
    return jobs.dropHead();
}

void WATCardOffice::main() {
    jobs.add(new Job{});
    printer.print(Printer::Kind::WATCardOffice, 'S');
    for(;;) {
        _Accept(~WATCardOffice) {
            //delete outstanding jobs
            uSeqIter<Job> seqIterJob;
            Job * jp;
            for(seqIterJob.over(jobs); seqIterJob>>jp;) {
                jobs.remove(jp);
                delete jp;
            }
            while(activeCouriers>0) {
                _Accept(requestWork) {
                    _Resume Empty{} _At *(WATCardOffice::Courier *)(void *)bench.front();
                    bench.signalBlock();
                }
                activeCouriers--;
            }
            break;
        } or _When(jobCounter>0) _Accept(requestWork) {
            jobCounter--;
            bench.signalBlock();
            printer.print(Printer::Kind::WATCardOffice, 'W');
        } or _Accept(transfer) {
            jobCounter++;
            jobs.tail()->args = Args{currSID, currAmount, currCard};
            jobs.add(new Job{});
            printer.print(Printer::Kind::WATCardOffice, 'T', currSID, currAmount);
        } or _Accept(create) {
            jobCounter++;
            WATCard *newCard = new WATCard;
            jobs.tail()->args = Args{currSID, currAmount, newCard};
            cards.add(new Card{newCard});
            jobs.add(new Job{});
            printer.print(Printer::Kind::WATCardOffice, 'C', currSID, currAmount);
        }
    }
    printer.print(Printer::Kind::WATCardOffice, 'F');
}

WATCardOffice::WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers )
  : printer{prt}, bank{bank}, numCouriers{numCouriers}, activeCouriers{0}, jobCounter{0}, courierPool{new Courier*[numCouriers]} {
    for(unsigned int i=0; i<numCouriers; i++) {
        courierPool[i] = new Courier{printer, *this, bank, i};
        activeCouriers++;
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

}
