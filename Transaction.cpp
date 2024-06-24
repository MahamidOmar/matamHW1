#include "Transaction.h"
#include "Utilities.h"

void TransactionDumpInfo(const Transaction& transaction, ofstream& file){
    file << "Sender Name: " << transaction.sender << std::endl;
    file << "Receiver Name: " << transaction.receiver << std::endl;

    //  maybe remove new line here
    file << "Transaction value: " << transaction.value << std::endl;
}

string TransactionHashedMessage(const Transaction& transaction){
    return hash(transaction.value, transaction.sender, transaction.receiver);
}

bool TransactionVerifyHashedMessage(
        const Transaction& transaction,
        string hashedMessage
){

}
