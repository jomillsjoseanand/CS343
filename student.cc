#include "soda.h"

Student::Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
			 unsigned int id, unsigned int maxPurchases) : prt(prt), nameServer(nameServer), 
             cardOffice(cardOffice), groupoff(groupoff), id(id), maxPurchases(maxPurchases) {
                lostCard = false;
                    // Select a random number of bottles to purchase [1, MaxPurchases]
                    numPurchases = prng(1, maxPurchases);

                    // Select a random favourite flavour [0, 3]
                    favouriteFlavour = static_cast<BottlingPlant::Flavours>(prng(0, 3));

                    prt.print(Printer::Student, id, 'S', favouriteFlavour, numPurchases); // Start of student
             }

// Destructor
Student::~Student() {

    try {
        if (watCard.available()) {
            delete watCard();
        } else {
            _Select(watCard){
                delete watCard();
            }
        }

        // if (giftCard.available()) {
        //     delete giftCard();
        // }
    } catch( WATCardOffice::Lost &lost ) {
        // if (giftCard.available()) {
        //     delete giftCard();
        // }
    }
    
}


void Student::main() {
    // Create a WATCard from the WATCardOffice and initialize balance with $5
    watCard = cardOffice.create(id, 5);
    // cout << "after watcard created" <<endl;

    // Create a gift card from Groupoff with a value of $SodaCost
    giftCard = groupoff.giftCard(id);
    // cout << "after giftcard created" <<endl;

    // Obtain the location of a vending machine from the name server
    // cout <<"before getting machine "<<endl;
    vendingMachine = nameServer.getMachine(id);
    // cout <<"after getting machine "<<endl;
    // cout << "vendingMachine: " << vendingMachine->getId();


    // Print selecting vending machine
    prt.print(Printer::Student, id, 'V', vendingMachine->getId());
        
    unsigned int totalFreeSodas = 0;
    unsigned int totalSodasDrank = 0;
    unsigned int bottlesBought = 0;

    while (bottlesBought < numPurchases) {
    buy: {
        if (!lostCard) {
            yield(prng(1, 10)); // Yield before attempting to purchase
        }
        lostCard = false;
        
        // Attempt to purchase a soda  
        try {
            _Enable {
                // If both cards have money, use the gift card first
                _Select (giftCard) {
                    unsigned int currentBalance = giftCard()->getBalance();
                    vendingMachine->buy(favouriteFlavour, *giftCard());
                    if (currentBalance != giftCard()->getBalance()) {
                        bottlesBought++;
                        totalSodasDrank++;
                        prt.print(Printer::Student, id, 'G', favouriteFlavour, giftCard()->getBalance());
                        giftCard.reset();
                    }                   
                } or _Select (watCard) {
                    unsigned int currentBalance = watCard()->getBalance();
                    vendingMachine->buy(favouriteFlavour, *watCard());
                    if (currentBalance != watCard()->getBalance()) {
                        bottlesBought++;
                        totalSodasDrank++;
                        prt.print(Printer::Student, id, 'B', favouriteFlavour, watCard()->getBalance());
                    }
                } 
            }
        } catch( WATCardOffice::Lost &lost ) {
            lostCard = true;
            prt.print(Printer::Student, id, 'L'); // Lost card
            watCard = cardOffice.create(id, 5);
            goto buy;
            
        }  catch ( VendingMachine::Funds &funds ) {
            // 
            // If the vending machine indicates
            // insufficient funds, a student transfers the current vending-machine soda-cost plus $5 to their WATCard via the
            // WATCard office and attempts another purchase.
            // 

            // If vending machine indicates insufficient funds, transfer the current vending-machine soda-cost plus $5 to their WATCard via the WATCard office and attempt another purchase
            watCard = cardOffice.transfer(id, vendingMachine->cost() + 5, watCard());
            goto buy;
        } catch( VendingMachine::Stock &stock ) {
            // 
            // If the vending machine is out of the student’s favourite flavour,
            // the student must obtain a new vending machine from the name server and 
            // attempt another purchase.
            // a student may perform a busy wait among vending machines until their 
            // specific soda appears from the bottling plant. 
            // 

            // If vending machine is out of the student’s favourite flavour, obtain a new vending machine from the name server and attempt another purchase
            vendingMachine = nameServer.getMachine(id);
            prt.print(Printer::Student, id, 'V', vendingMachine->getId());
            goto buy;
        } catch( VendingMachine::Free &free ) {
            // 
            // there is a 50% chance the student watches an advertisement 
            // associated with it by yielding 4 times (not random) 
            // while drinking the free soda and then attempts another purchase.
            //  
            prt.print(Printer::Student, id, 'A', favouriteFlavour); // Advertisement
            totalFreeSodas++;
            totalSodasDrank++;
            // If vending machine delivers a free bottle of soda, there is a 50% chance the student watches an advertisement associated with it by yielding 4 times (not random) while drinking the free soda and then attempts another purchase
            if (prng(0, 1)) {
                yield(4);
            } else {
                prt.print(Printer::Student, id, 'X'); // No advertisement
            }
            goto buy;
        }
    }
    }

    prt.print(Printer::Student, id, 'F', totalSodasDrank, totalFreeSodas); // Finished
}

// Student context:
// - A Student’s function is to periodically buy some of their favourite soda 
// // - Each student is passed an id in the range [0, NumStudents) for
// //identification.

// - A student begins by selecting a random number of bottles to purchase [1, MaxPurchases]
// - pick a random favourite flavour [0, 3] 
// // creates a WATCard from the WATCardOffice and intialize balance with $5 
// - creates a gift card from Groupoff with a value of $SodaCost
// - obtains the location of a vending machine from the name server.
// - A student terminates after purchasing their selected random number of bottles of their favourite flavour.

// // - Before each attempt to purchase a soda (including the first), a 
// //student yields a random number of times in the range [1, 10]. 
// -  then wants to buy a soda but may need to wait for money to be 
// transferred either from the WATCardOffice to their WATCard or from Groupoff 
// to their gift card. 
//     - Once money is available on either card, the student can attempt to 
//     buy a bottle of soda from their current vending machine; 
//         - if both cards have money, use the gift card first. 
//         - Note, a giftcard future can only be used once, so it must be 
//         reset after use to prevent any further usage.

// - If a courier has lost a student’s WATCard during a transfer (see WATCardOffice::Courier),
// the exception WATCardOffice::Lost is raised when the future value is accessed.
// - In this case, the students watcard was lost the first time it must 
// create a new WATCard from the WATCardOffice with a $5 balance, and re-attempt to purchase a soda without
// yielding as a purchase has not occurred. 
// -  a courier can lose a student’s WATCard during each transfer
// for the new WATCard so this issue can occur repeatedly.

// - Once money is available, a purchase begins with a student calling a vending machine to begin a transaction.
// - If the vending machine debits the WATCard and delivers a bottle of soda, the student drinks it and attempts
// another purchase. 
// // -  If the vending machine delivers a free bottle of soda (which does not count as a purchased
// // soda), there is a 50% chance the student watches an advertisement 
// // associated with it by yielding 4 times (not random) 
// // while drinking the free soda and then attempts another purchase.
// // - If the vending machine indicates
// // insufficient funds, a student transfers the current vending-machine soda-cost plus $5 to their WATCard via the
// // WATCard office and attempts another purchase.

// // - If the vending machine is out of the student’s favourite flavour,
// //the student must obtain a new vending machine from the name server and 
// // attempt another purchase.
// // - a student may perform a busy wait among vending machines until their 
// // specific soda appears from the bottling plant. 

// *** NOTE: Watch out for the case of a student who only buys one soda using the gift card.
