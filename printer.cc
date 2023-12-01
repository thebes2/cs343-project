#include <iostream>
#include "printer.h"
using namespace std;

void Printer::print(Kind kind, char state) {
    unsigned int idx = getColumn(kind);
    if(buffer[idx].defined) flushBuffer();
    buffer[idx].state = state;
    buffer[idx].defined = true;
}

void Printer::print(Kind kind, char state, unsigned int value1) {
    unsigned int idx = getColumn(kind);
    if(buffer[idx].defined) flushBuffer();
    buffer[idx].state = state;
    buffer[idx].value1 = value1;
    buffer[idx].defined = true;
    buffer[idx].def1 = true;
}

void Printer::print(Kind kind, char state, unsigned int value1, unsigned int value2) {
    unsigned int idx = getColumn(kind);
    if(buffer[idx].defined) flushBuffer();
    buffer[idx].state = state;
    buffer[idx].value1 = value1;
    buffer[idx].value2 = value2;
    buffer[idx].defined = true;
    buffer[idx].def1 = true;
    buffer[idx].def2 = true;
}

void Printer::print(Kind kind, unsigned int lid, char state) {
    unsigned int idx = getColumn(kind, lid);
    if(buffer[idx].defined) flushBuffer();
    buffer[idx].state = state;
    buffer[idx].defined = true;
}

void Printer::print(Kind kind, unsigned int lid, char state, unsigned int value1) {
    unsigned int idx = getColumn(kind, lid);
    if(buffer[idx].defined) flushBuffer();
    buffer[idx].state = state;
    buffer[idx].value1 = value1;
    buffer[idx].defined = true;
    buffer[idx].def1 = true;
}

void Printer::print(Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2) {
    unsigned int idx = getColumn(kind, lid);
    if(buffer[idx].defined) flushBuffer();
    buffer[idx].state = state;
    buffer[idx].value1 = value1;
    buffer[idx].value2 = value2;
    buffer[idx].defined = true;
    buffer[idx].def1 = true;
    buffer[idx].def2 = true;
}

Printer::Printer(unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers)
  : numStudents(numStudents), numVendingMachines(numVendingMachines), numCouriers(numCouriers) {
    totalCols = singletons + numStudents + numVendingMachines + numCouriers;
    buffer = new PrintState[totalCols];
    // print header
    for (unsigned int i=0;i<singletons;i++) { cout << classNames[i] << "\t"; }
    // print students
    for (unsigned int i=0;i<numStudents;i++) { cout << classNames[singletons] << i << "\t"; }
    // print machines
    for (unsigned int i=0;i<numVendingMachines;i++) { cout << classNames[singletons+1] << i << "\t"; }
    // print couriers
    for (unsigned int i=0;i<numCouriers;i++) {
        cout << classNames[singletons+2] << i << (i+1 == numCouriers? "\n":"\t");
    }

    for (unsigned int i=0;i<totalCols;i++) {
        cout << "*******" << (i+1 == totalCols? "\n":"\t");
    }
}

Printer::~Printer() {
    cout << "***********************" << endl;
    delete[] buffer;
}

unsigned int Printer::getColumn(Kind kind) {
    unsigned int idx = 0;
    switch (kind) {
      case Parent: idx = 0; break;
      case Groupoff: idx = 1; break;
      case WATCardOffice: idx = 2; break;
      case NameServer: idx = 3; break;
      case Truck: idx = 4; break;
      case BottlingPlant: idx = 5; break;
      default:
        assert(false); // invalid type
    }
    return idx;
}

unsigned int Printer::getColumn(Kind kind, unsigned int lid) {
    unsigned int idx = 0;
    switch (kind) {
      case Student:
        idx = singletons + lid; break;
      case Vending:
        idx = singletons + numStudents + lid; break;
      case Courier:
        idx = singletons + numStudents + numVendingMachines + lid; break;
      default:
        assert(false); // invalid type
    }
    return idx;
}

void Printer::displayState(PrintState &s, char suf) {
    cout << s.state;
    if (s.def1) { cout << to_string(s.value1); }
    if (s.def2) {
        assert(s.def1);
        cout << "," << to_string(s.value2);
    }
    cout << suf;
}

void Printer::flushBuffer() {
    unsigned int lastDefined = 0;
    for (unsigned int i=0;i<totalCols;i++) {
        if (buffer[i].defined) { lastDefined = i+1; }
    }
    for (unsigned int i=0;i<lastDefined;i++) {
        displayState(buffer[i], (i+1 == lastDefined? '\n':'\t'));
        buffer[i].defined = false; // clear state
        buffer[i].def1 = false;
        buffer[i].def2 = false;
    }
}