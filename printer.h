
#ifndef __PRINTER_H__
#define __PRINTER_H__


// Monitor to handle outputting various states and votes
_Monitor Printer {
    enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };

    unsigned int numStudents;
    unsigned int numVendingMachines;
    unsigned int numCouriers;
    unsigned int numColumns;           // Total number of columns (fixed + variable instances)


    struct BufferEntry {                    // Buffer to temporarily store vote and state informationn
        char state;                         // State to print
        bool empty = true;                  // Whether this buffer slot is empty
        int value1 = -1; 
        int value2 = -1;
    };

    vector<Buffer> buffer;           // Vector of buffers for storing each voter's state

    void flush();                    // Function to output and clear buffer content
    unsigned int determineColumn(Kind kind, unsigned int lid = 0) const;

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

#endif // __PRINTER_H__