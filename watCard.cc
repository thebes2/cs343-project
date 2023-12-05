#include "watCard.h"

WATCard::WATCard() : balance(0) {}

// deposit amount into watcard
void WATCard::deposit(unsigned int amount) {
    balance += amount;
}

// withdraw money from watcard
void WATCard::withdraw(unsigned int amount) {
    assert(amount <= balance);
    balance -= amount;
}

// get balance from watcard
unsigned int WATCard::getBalance() {
    return balance;
}
