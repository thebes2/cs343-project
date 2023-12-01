#include "watCardOffice.h"

WATCardOffice::Job* WATCardOffice::requestWork() {
    // the main thread of WATCardOffice will _Accept requestWork once there are jobs
}

using WATCardOffice::Courier

Courier::Courier(WATCardOffice& office, Bank& bank)
    : office(office), bank(bank) {}

void Courier::main() {
    for (;;) {
        Job* request = office.requestWork();
        unsigned int id = request->args.id, amount = request->args.amount;
        bank.withdraw(id, amount);
        if (prng(6) == 0) { // lost WATCard
            request->result.delivery( new Lost{} );
        }
        else {
            WATCard *res = new WATCard;
            res->deposit(amount);
            request->result.delivery( res );
        }
    }
}

/*

The WATCardOffice is an administrator task used by a student to 
transfer funds from their bank account to their WATCard to buy a soda. 
Initially, the WATCard office creates a fixed-sized courier pool with 
numCouriers courier tasks to communicate with the bank. (Additional couriers
 may not be created after the WATCardOffice begins.) A student performs 
 an asynchronous call to create to create a “real” WATCard with an initial 
 balance. A future WATCard is returned and sufficient funds are subsequently
  obtained from the bank (see Parent task) via a courier to satisfy the create
   request. 
   
   A student performs an asynchronous call to transfer when their WATCard
    indicates there is insufficient funds to buy a soda. Again, a future WATCard is
     returned and sufficient funds are subsequently obtained from the bank as for
      create. The WATCard office is empowered to transfer funds from a student’s
       bank-account to its WATCard by sending a request through a courier to the
        bank. Each courier task calls requestWork, blocks until a Job request is
         ready, and then receives the next Job request as the result of the call.
          As soon as the request is satisfied (i.e., money is obtained from the bank),
          the courier updates the student’s WATCard. There is a 1 in 6 chance, 
          prng( 6 ) == 0, a courier loses a student’s WATCard after the update. 
          When the card is lost, the exception WATCardOffice::Lost is inserted 
          into the future, rather than making the future available, and the 
          current WATCard is deleted.


*/