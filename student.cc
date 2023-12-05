#include "student.h"
#include "vendingMachine.h"
#include "bottlingPlant.h"

// ctor
Student::Student(Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
			 unsigned int id, unsigned int maxPurchases)
  : printer(prt), nameServer(nameServer), office(cardOffice), groupoff(groupoff), id(id), maxPurchases(maxPurchases) {}

void Student::main() {
	// choose randomly number of times to purchase and favourite flavor; print start
	numTimes = prng(1, maxPurchases);
	favouriteFlavour = prng(0, 3);
	printer.print(Printer::Kind::Student, id, 'S', favouriteFlavour, numTimes);
	
	// create giftcard and watcard
	WATCard::FWATCard watcard = office.create(id, 5);
	WATCard::FWATCard giftcard = groupoff.giftCard();
	WATCard::FWATCard *currentCard = nullptr;
	WATCard* card = nullptr;

	// get current vending machine to buy soda
	VendingMachine *currentVM = nameServer.getMachine(id);
	printer.print(Printer::Kind::Student, id, 'V', currentVM->getId());
	unsigned int sodas = 0;
	bool usedGiftcard = false;


	while (sodas < numTimes) { // only iterate enough times to purchase required number of sodas
		yield(prng(1, 10));
		for (;;) {
			// wait for giftcard or watcard to become available; giftcard has higher priority
			_Select(giftcard) { currentCard = &giftcard; }
			or _Select(watcard) { currentCard = &watcard; }
			// try purchasing a soda
			try {
				_Enable {
					card = (*currentCard)();
					currentVM->buy(static_cast<BottlingPlant::Flavours>(favouriteFlavour), *card);
					printer.print(Printer::Kind::Student, id, (currentCard == &watcard? 'B':'G'), favouriteFlavour, card->getBalance());
					if (currentCard == &giftcard) {
						usedGiftcard = true;
						delete (*currentCard)(); // free memory since groupoff doesn't delete used giftcards
						currentCard->reset(); // giftcard is one time use only
					}
					sodas ++; // increment number of purchased sodas
					break;
				}
			}
			catch (WATCardOffice::Lost&) { // card was lost by courier in transit
				printer.print(Printer::Kind::Student, id, 'L');
				// create a new card and try again without yielding again
				watcard = office.create(id, 5);
			}
			catch (VendingMachine::Free&) { // free soda, just yield 4 times if ad is watched
				printer.print(Printer::Kind::Student, id, (currentCard == &watcard? 'A':'a'), favouriteFlavour, card->getBalance());
				if (prng(2) == 0) { // watched advertisement
					yield(4); 
				}
				else { // did not watch advertisement
					printer.print(Printer::Kind::Student, id, 'X');
				}
				break;
			}
			catch (VendingMachine::Funds&) { // insufficient funds, so transfer funds to watcard
				assert(currentCard == &watcard);
				watcard = office.transfer(id, 5+currentVM->cost(), card); // transfer funds and try again
				break;
			}
			catch (VendingMachine::Stock&) { // get new vending machine and try again
				currentVM = nameServer.getMachine(id); // new vending machine
				printer.print(Printer::Kind::Student, id, 'V', currentVM->getId());
				break; // try again
			}
		}
	}
	_Select(watcard); // for the special case where student only buys one soda with a giftcard; wait for watcard to be ready from courier
	if (!usedGiftcard) { // also have to cleanup giftcard
		if (!giftcard.available()) { // cancel if giftcard not available
			giftcard.cancel();
		}
		try { _Enable{ delete giftcard(); } }
		catch (...) {}
	}
	printer.print(Printer::Kind::Student, id, 'F');
}