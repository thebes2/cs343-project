#ifndef _PRINTER_H_
#define _PRINTER_H_

_Monitor Printer {
  //printState is a struct for storing printing information
  struct printState {
	char state;
	unsigned int value1;
	unsigned int value2;
    unsigned int lid;
    bool empty = true;
  };

  //buffer is a dynamically allocated array of printStates for storing printing information for each voter
  printState *buffer;
  unsigned int numStudents, numVendingMachines, numCouriers; 
  void flush_buffer();
  public:
	enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
	Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
	void print( Kind kind, char state );
	void print( Kind kind, char state, unsigned int value1 );
	void print( Kind kind, char state, unsigned int value1, unsigned int value2 );
	void print( Kind kind, unsigned int lid, char state );
	void print( Kind kind, unsigned int lid, char state, unsigned int value1 );
	void print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 );
};

#endif