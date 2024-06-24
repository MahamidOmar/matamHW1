#include "BlockChain.h"

int BlockChainGetSize(const BlockChain& blockChain){
    return blockChain.size;
}

int BlockChainPersonalBalance(const BlockChain& blockChain, const string& name){
    ////  the total of which "name" has sent
    int payed_amount = 0;
    ////  the total of which "name" has received
    int received_amount = 0;
    ////  iterator for moving over all the given blockchain nodes
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

////    the new node and the new transaction maybe change them to pointers and create with "new" and erase with "delete"
void BlockChainAppendTransaction(
        BlockChain& blockChain,
        unsigned int value,
        const string& sender,
        const string& receiver,
        const string& timestamp
){
    ////    Create a new transaction to be added to the blockchain
    Transaction toAppend;
    toAppend.value = value;
    toAppend.sender = sender;
    toAppend.receiver = receiver;
    ////    Create a new node to save the new transaction
    Node* newNode;
    newNode->transaction = toAppend;
    newNode->timeStamp = timestamp;
    newNode->next = nullptr;
    ////    change the head of the blockchain to the new node
    if(blockChain.size == 0){
        blockChain.head = newNode;
    }else{
        newNode->next = blockChain.head;
        blockChain.head = newNode;
    }

    ++blockChain.size;
}

////    Also need to check if the new node should be created and removed with "new" and "delete"
void BlockChainAppendTransaction(
        BlockChain& blockChain,
        const Transaction& transaction,
        const string& timestamp
){
    Node* newNode;
    newNode->transaction = transaction;
    newNode->timeStamp = timestamp;
    newNode->next = nullptr;
    if(blockChain.size == 0){
        blockChain.head = newNode;
    }else{
        newNode->next = blockChain.head;
        blockChain.head = newNode;
    }
    ++blockChain.size;
}

BlockChain BlockChainLoad(ifstream& file);

void BlockChainDump(const BlockChain& blockChain, ofstream& file);

void BlockChainDumpHashed(const BlockChain& blockChain, ofstream& file);

bool BlockChainVerifyFile(const BlockChain& blockChain, std::ifstream& file);

void BlockChainCompress(BlockChain& blockChain);

void BlockChainTransform(BlockChain& blockChain, updateFunction function);





