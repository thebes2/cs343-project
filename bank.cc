#include "bank.h"

Bank::Bank(unsigned int numStudents)
    : numStudents{numStudents},  balance{new unsigned int[numStudents]}, 
      waiters{new uCondition[numStudents]} {
    for (unsigned int i=0;i<numStudents;i++) { balance[i] = 0; }
}

Bank::~Bank() {
    delete[] balance;
    // wake all remaining waiting threads
    for (unsigned int i=0;i<numStudents;i++) { waiters[i].signal(); }
    delete[] waiters;
}

void Bank::deposit(unsigned int id, unsigned int amount) {
    balance[id] += amount;
    waiters[id].signal();
}

void Bank::withdraw(unsigned int id, unsigned int amount) {
    while (balance[id] < amount) { waiters[id].wait(); }
    balance[id] -= amount;
}