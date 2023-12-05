#include "watCardOffice.h"
#include "printer.h"
#include "bank.h"

// ctor for Courier
WATCardOffice::Courier::Courier(Printer &printer, WATCardOffice& office, Bank& bank, unsigned int id)
    : printer{printer}, office(office), bank(bank), id{id} {}

// main method for Courier to request and do work
void WATCardOffice::Courier::main() {
    printer.print(Printer::Kind::Courier, id, 'S');
    for (;;) {
        _Accept(~Courier) {break;}
        _Else {// not busy waiting since requestWork below blocks if no work
            Job * request;
            try { _Enable {
                request = office.requestWork();
                }
            } catch (const WATCardOffice::Empty &) { // break and terminate if Empty exception is returned
                break;
            }
            // parse arguments from current Job and complete it
            unsigned int studentId = request->args.id, amount = request->args.amount;
            printer.print(Printer::Kind::Courier, id, 't', studentId, amount);
            // withdraw money from bank and deposit into student's card
            bank.withdraw(studentId, amount);
            request->args.card->deposit(amount);
            if (prng(0,5)==0) { // lost WATCard
                printer.print(Printer::Kind::Courier, id, 'L', studentId);
                request->result.delivery( new Lost{} ); // delivered lost exception
            }
            else {
                request->result.delivery(request->args.card); // deliver updated WATCard or giftcard to student
                printer.print(Printer::Kind::Courier, id, 'T', studentId, amount);
            }
            delete request; // delete this job request from the heap
        }
    }
    printer.print(Printer::Kind::Courier, id, 'F');
}

// public mutex member to create a watcard; returns a future
WATCard::FWATCard WATCardOffice::create(unsigned int sid, unsigned int amount) {
    currSID = sid; currAmount = amount; // save function arguemnts to local stack
    return jobs.tail()->result;
}

// public mutex member to transfer funds into a watcard; returns a future
WATCard::FWATCard WATCardOffice::transfer(unsigned int sid, unsigned int amount, WATCard * card) {
    currSID = sid; currAmount = amount; currCard = card; // save function arguments to local stack
    return jobs.tail()->result;
}

// public mutex member for couriers to request work; returns a pointer to a Job struct
WATCardOffice::Job * WATCardOffice::requestWork() {
    bench.wait((uintptr_t)(void*)&uThisTask()); // go to sleep to determine if exception needs to be thrown
    return jobs.dropHead(); // return the job at the head of the list (one that has been waiting the most)
}

void WATCardOffice::main() {
    jobs.add(new Job{}); // add a node to the Jobs list so that the first call to create/transfer has a future ready to return
    printer.print(Printer::Kind::WATCardOffice, 'S');
    for(;;) {
        _Accept(~WATCardOffice) {
            // delete outstanding jobs on the jobs list if they remain since they can't be worked on
            uQueueIter<Job> qIterJob;
            Job * jp;
            for(qIterJob.over(jobs); qIterJob>>jp;) {
                jobs.remove(jp);
                delete jp;
            }
            // if there are active couriers, accept their calls to request work and Resume empty exception at them
            while(activeCouriers>0) {
                _Accept(requestWork) {
                    _Resume Empty{} _At *(WATCardOffice::Courier *)(void *)bench.front();
                    bench.signalBlock(); // allow courier to return with exception
                }
                activeCouriers--;
            }
            break;
        } or _When(jobCounter>0) _Accept(requestWork) { // if there are jobs to work on, accept calls by courier to requestWork
            jobCounter--;
            bench.signalBlock(); // signal block to allow couriers to return
            printer.print(Printer::Kind::WATCardOffice, 'W');
        } or _Accept(transfer) {
            // transfer function has already returned a future stored in the tail of jobs list
            // we now need to populate that node on the list with the correct arguments for this job
            // and then add a new node to the list for the next call of transfer/create
            jobCounter++;
            jobs.tail()->args = Args{currSID, currAmount, currCard}; // populate correct arguments for Job node whose future was returned
            jobs.add(new Job{}); // add new node to jobs for next call
            printer.print(Printer::Kind::WATCardOffice, 'T', currSID, currAmount);
        } or _Accept(create) {
             // create function has already returned a future stored in the tail of jobs list
            // we now need to populate that node on the list with the correct arguments for this job
            // and then add a new node to the list for the next call of transfer/create
            jobCounter++;
            WATCard *newCard = new WATCard;
            jobs.tail()->args = Args{currSID, currAmount, newCard}; // populate correct arguments for node whose future was returned
            cards.add(new Card{newCard}); // add this newly created WATCard to list of WATCards for memory management
            jobs.add(new Job{}); // create a new node on jobs for next call
            printer.print(Printer::Kind::WATCardOffice, 'C', currSID, currAmount);
        }
    }
    printer.print(Printer::Kind::WATCardOffice, 'F');
}

// ctor for WATCardOffice
WATCardOffice::WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers )
  : printer{prt}, bank{bank}, numCouriers{numCouriers}, activeCouriers{0}, jobCounter{0}, courierPool{new Courier*[numCouriers]} {
    for(unsigned int i=0; i<numCouriers; i++) { // loop to generate all couriers on heap
        courierPool[i] = new Courier{printer, *this, bank, i}; // store couriers in courierPool array
        activeCouriers++;
    }
}

WATCardOffice::~WATCardOffice() {
    // delete courier pool
    for(unsigned int i=0; i<numCouriers; i++) {
        delete courierPool[i];
    }
    delete[] courierPool;

    // delete all watcards that were generated and their corresponding storage on uQueue list
    uQueueIter<Card> qIterJob;
    Card * cp;
    for(qIterJob.over(cards); qIterJob>>cp;) {
        delete cp->card; // delete WATCard
        cards.remove(cp);
        delete cp; // delete node itself used to store WATCard on list
    }

}
