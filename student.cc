#include "student.h"
#include "vendingMachine.h"
#include "bottlingPlant.h"

Student::Student(Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
			 unsigned int id, unsigned int maxPurchases)
	: printer(prt), nameServer(nameServer), office(cardOffice), groupoff(groupoff), id(id), maxPurchases(maxPurchases) {}

void Student::main() {
	unsigned int numTimes = prng(maxPurchases) + 1;
	unsigned int favouriteFlavour = prng(4);
	printer.print(Printer::Kind::Student, 'S', favouriteFlavour, numTimes);
	WATCard::FWATCard watcard = office.create(id, 5);
	WATCard::FWATCard giftcard = groupoff.giftCard();
	WATCard::FWATCard *currentCard = nullptr;
	WATCard* card = nullptr;
	VendingMachine *currentVM = nameServer.getMachine(id);
	printer.print(Printer::Kind::Student, 'V', currentVM->getId());
	for (unsigned int i=0;i<numTimes;i++) {
		yield(prng(1, 11));
		for (;;) {
			_When(true) _Select(giftcard) { currentCard = &giftcard; }
			or _When(true) _Select(watcard) { currentCard = &watcard; }

			try {
				card = (*currentCard)();
				currentVM->buy(static_cast<BottlingPlant::Flavours>(favouriteFlavour), *card);
				printer.print(Printer::Kind::Student, (currentCard == &watcard? 'B':'G'),
							  favouriteFlavour, card->getBalance());
				if (currentCard == &giftcard) {
					currentCard->reset(); // giftcard is one time use only
				}
				break;
			}
			catch (WATCardOffice::Lost&) {
				printer.print(Printer::Kind::Student, 'L');
				// create a new card and try again
				watcard = office.create(id, 5);
			}
			catch (VendingMachine::Free&) { // free soda, just yield 4 times
				if (prng(2) == 0) { // watched advertisement
					printer.print(Printer::Kind::Student, (currentCard == &watcard? 'A':'a'),
								  favouriteFlavour, card->getBalance());
					yield(4);
				}
				else { // did not watch advertisement
					printer.print(Printer::Kind::Student, 'X');
				}
				break;
			}
			catch (VendingMachine::Funds&) { // insufficient funds, so transfer funds to watcard
				assert(currentCard == &watcard);
				office.transfer(id, 5+currentVM->cost(), card);
			}
			catch (VendingMachine::Stock&) { // get new vending machine and try again
				currentVM = nameServer.getMachine(id);
				printer.print(Printer::Kind::Student, 'V', currentVM->getId());
			}
		}
	}
	printer.print(Printer::Kind::Student, 'F');
}