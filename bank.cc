#include "bank.h"

// ctor for Bank; initalize balance of each student to 0 and create uCondition variables for waiting
Bank::Bank(unsigned int numStudents)
    : numStudents{numStudents},  balance{new unsigned int[numStudents]}, 
      waiters{new uCondition[numStudents]} {
    for (unsigned int i=0;i<numStudents;i++) { balance[i] = 0; }
}

// dtor for Bank; delete all uCondition variables
Bank::~Bank() {
    delete[] balance;
    // wake all remaining waiting threads
    for (unsigned int i=0;i<numStudents;i++) { waiters[i].signal(); }
    delete[] waiters;
}

// deposit function for Bank; wake up any Tasks waiting for amount of money in account to change
void Bank::deposit(unsigned int id, unsigned int amount) {
    balance[id] += amount;
    waiters[id].signal();
}

// attempt to withdraw money from account, if there isn't enough then wait on that uCondition
void Bank::withdraw(unsigned int id, unsigned int amount) {
    while (balance[id] < amount) { waiters[id].wait(); }
    balance[id] -= amount;
}