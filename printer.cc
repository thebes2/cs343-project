#include "printer.h"

Printer::print(Kind kind, char state) {
    if(!buffer[kind].empty) flush_buffer();
    buffer[kind].state = state;
}

Printer::print(Kind kind, char state, unsigned int value1) {
    if(!buffer[kind].empty) flush_buffer();
    buffer[kind].state = state;
    buffer[kind].value1 = value1;
}

Printer::print(Kind kind, char state, unsigned int value1, unsigned int value2) {
    if(!buffer[kind].empty) flush_buffer();
    buffer[kind].state = state;
    buffer[kind].value1 = value1;
    buffer[kind].value2 = value2;
}

Printer::print(Kind kind, unsigned int lid, char state) {
    if(!buffer[kind].empty) flush_buffer();
    buffer[kind].lid = lid;
    buffer[kind].state = state;
}

Printer::print(Kind kind, unsigned int lid, char state, unsigned int value1) {
    if(!buffer[kind].empty) flush_buffer();
    buffer[kind].lid = lid;
    buffer[kind].state = state;
    buffer[kind].value1 = value1;
}

Printer::print(Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2) {
    if(!buffer[kind].empty) flush_buffer();
    buffer[kind].lid = lid;
    buffer[kind].state = state;
    buffer[kind].value1 = value1;
    buffer[kind].value2 = value2;
}

Printer::flush_buffer() {
    
}