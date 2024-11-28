#ifndef __BANK_H__
#define __BANK_H__

using namespace std;

_Monitor Bank {
    unsigned int numStudents;
    vector<unsigned int> accounts; // Store balances for each student

  public:
	Bank( unsigned int numStudents );
	void deposit( unsigned int id, unsigned int amount );
	void withdraw( unsigned int id, unsigned int amount );
};

#endif