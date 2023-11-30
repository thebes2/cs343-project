#ifndef _BANK_H_
#define _BANK_H_

_Monitor Bank {
	unsigned int *balance;
	uCondition *waiters;
  public:
	Bank( unsigned int numStudents );
	~Bank();
	void deposit( unsigned int id, unsigned int amount );
	void withdraw( unsigned int id, unsigned int amount );
};

#endif