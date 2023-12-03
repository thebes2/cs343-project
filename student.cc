#include "student.h"
#include "vendingMachine.h"
#include "bottlingPlant.h"

Student::Student(Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
			 unsigned int id, unsigned int maxPurchases)
  : printer(prt), nameServer(nameServer), office(cardOffice), groupoff(groupoff), id(id), maxPurchases(maxPurchases) {
	numTimes = prng(maxPurchases) + 1;
	favouriteFlavour = prng(4);
	printer.print(Printer::Kind::Student, id, 'S', favouriteFlavour, numTimes);
}

void Student::main() {
	WATCard::FWATCard watcard = office.create(id, 5);
	WATCard::FWATCard giftcard = groupoff.giftCard();
	WATCard::FWATCard *currentCard = nullptr;
	WATCard* card = nullptr;
	VendingMachine *currentVM = nameServer.getMachine(id);
	printer.print(Printer::Kind::Student, id, 'V', currentVM->getId());
	unsigned int sodas = 0;
	while (sodas < numTimes) {
		yield(prng(1, 10));
		for (;;) {
			_Select(giftcard) { currentCard = &giftcard; }
			or _Select(watcard) { currentCard = &watcard; }

			try {
				_Enable {
					card = (*currentCard)();
					currentVM->buy(static_cast<BottlingPlant::Flavours>(favouriteFlavour), *card);
					printer.print(Printer::Kind::Student, id, (currentCard == &watcard? 'B':'G'), favouriteFlavour, card->getBalance());
					if (currentCard == &giftcard) {
						delete (*currentCard)();
						currentCard->reset(); // giftcard is one time use only
					}
					sodas ++;
					break;
				}
			}
			catch (WATCardOffice::Lost&) {
				printer.print(Printer::Kind::Student, id, 'L');
				// create a new card and try again
				watcard = office.create(id, 5);
			}
			catch (VendingMachine::Free&) { // free soda, just yield 4 times
				if (prng(2) == 0) { // watched advertisement
					printer.print(Printer::Kind::Student, id, (currentCard == &watcard? 'A':'a'),
								  favouriteFlavour, card->getBalance());
					yield(4);
				}
				else { // did not watch advertisement
					printer.print(Printer::Kind::Student, id, 'X');
				}
				sodas ++;
				break;
			}
			catch (VendingMachine::Funds&) { // insufficient funds, so transfer funds to watcard
				assert(currentCard == &watcard);
				watcard = office.transfer(id, 5+currentVM->cost(), card);
				break;
			}
			catch (VendingMachine::Stock&) { // get new vending machine and try again
				currentVM = nameServer.getMachine(id);
				printer.print(Printer::Kind::Student, id, 'V', currentVM->getId());
				break;
			}
		}
	}
	printer.print(Printer::Kind::Student, id, 'F');
}