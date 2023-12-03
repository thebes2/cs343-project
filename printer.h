#ifndef _PRINTER_H_
#define _PRINTER_H_

_Monitor Printer {
  public:
	enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
  private:
 	// PrintState is a struct for storing printing information
	struct PrintState {
		char state;
		unsigned int value1;
		unsigned int value2;
		// flags for whether this state is defined or not
		// and whether value1 or value2 is populated
		bool defined = false, def1 = false, def2 = false;
	};

  	// buffer is a dynamically allocated array of PrintStates for storing printing information for each class
	unsigned int numStudents, numVendingMachines, numCouriers, totalCols;
	PrintState *buffer;

	// print a full line of the buffer and empty out all states
	void flushBuffer();
	// print a single state
	void displayState(PrintState &s);
	
	// helper functions to get which columns the print call should be displayed in
	unsigned int getColumn(Kind kind);
	unsigned int getColumn(Kind kind, unsigned int lid);

	const unsigned int singletons = 6; // number of classes that only have a single instance
	// display names of each class
	const char* classNames[9] = {"Parent", "Gropoff", "WATOff", "Names", "Truck", "Plant", "Stud", "Mach", "Cour"};
  public:
	Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
	~Printer();
	void print( Kind kind, char state );
	void print( Kind kind, char state, unsigned int value1 );
	void print( Kind kind, char state, unsigned int value1, unsigned int value2 );
	void print( Kind kind, unsigned int lid, char state );
	void print( Kind kind, unsigned int lid, char state, unsigned int value1 );
	void print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 );
};

#endif