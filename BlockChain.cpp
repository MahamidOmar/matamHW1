#include "BlockChain.h"

int BlockChainGetSize(const BlockChain& blockChain){
    return blockChain.size;
}

int BlockChainPersonalBalance(const BlockChain& blockChain, const string& name){
    //  the total of which "name" has sent
    int payed_amount = 0;
    //  the total of which "name" has received
    int received_amount = 0;
    //  iterator for moving over all the given blockchain nodes
    Node* iterator = blockChain.head;
    while(iterator != nullptr){
        if(name == iterator->transaction.sender){
            payed_amount += iterator->transaction.value;
        }else if(name == iterator->transaction.receiver){
            received_amount += iterator->transaction.value;
        }
        iterator = iterator->next;
    }
    return received_amount - payed_amount;
}

void BlockChainAppendTransaction(
        BlockChain& blockChain,
        unsigned int value,
        const string& sender,
        const string& receiver,
        const string& timestamp
);

void BlockChainAppendTransaction(
        BlockChain& blockChain,
        const Transaction& transaction,
        const string& timestamp
);

BlockChain BlockChainLoad(ifstream& file);

void BlockChainDump(const BlockChain& blockChain, ofstream& file);

void BlockChainDumpHashed(const BlockChain& blockChain, ofstream& file);

bool BlockChainVerifyFile(const BlockChain& blockChain, std::ifstream& file);

void BlockChainCompress(BlockChain& blockChain);

void BlockChainTransform(BlockChain& blockChain, updateFunction function);





