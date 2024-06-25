#include "BlockChain.h"

void destroyBlockChain(BlockChain& blockChain);

int BlockChainGetSize(const BlockChain& blockChain){
    return (int)blockChain.size;
}

int BlockChainPersonalBalance(const BlockChain& blockChain, const string& name){
    ////  the total of which "name" has sent
    int payed_amount = 0;
    ////  the total of which "name" has received
    int received_amount = 0;
    ////  iterator for moving over all the given blockchain nodes
    Node* iterator = blockChain.head;
    while(iterator != nullptr){
        if(name == iterator->transaction->sender){
            payed_amount += (int)iterator->transaction->value;
        }else if(name == iterator->transaction->receiver){
            received_amount += (int)iterator->transaction->value;
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
    Transaction* toAppend = new Transaction();
    toAppend->value = value;
    toAppend->sender = sender;
    toAppend->receiver = receiver;
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

////    may need to check if the file is open, and if I have to close it
BlockChain BlockChainLoad(ifstream& file){
    string line;
    while(std::getline(file, line)){
        string sender, receiver, timestamp;
        unsigned int value;
        ////    find the index in which every word in the line ends (positions of the spaces)
        int sender_position = line.find_first_of(' ');
        int receiver_position = line.find_first_of(' ', sender_position + 1);
        int value_position = line.find_first_of(' ', receiver_position + 1);
//        int timestamp_position = line.find_first_of(' ', value_position + 1);

        ////    parse the line
        sender = line.substr(0, sender_position);
        receiver = line.substr(sender_position + 1, receiver_position - sender_position - 1);
        ////    string to save the value before transforming it to an int
        string value_string = line.substr(receiver_position + 1, value_position - receiver_position - 1);
        value = std::stoi(value_string);
        timestamp = line.substr(value_position + 1);

        //// maybe "new" ?
        Transaction transaction = {value, sender, receiver};

    }
}

void BlockChainDump(const BlockChain& blockChain, ofstream& file){
    ////    blockchain is currently empty, nothing to print
    if(blockChain.size == 0){
        return;
    }
    file << "BlockChain info:" << std::endl;
    Node* iterator = blockChain.head;
    for(int i = 1 ; i <= blockChain.size ; ++i){
        file << i << "." << std::endl;
        TransactionDumpInfo(iterator->transaction, file);

        iterator = iterator->next;
    }
}

void BlockChainDumpHashed(const BlockChain& blockChain, ofstream& file){
    ////    blockchain is currently empty, nothing to print
    if(blockChain.size == 0){
        return;
    }
    Node* iterator = blockChain.head;
    ////    run through 1 -> n-1 transactions, and the last one print without new line
    for(int i = 1 ; i < blockChain.size ; ++i){
        file << TransactionHashedMessage(iterator->transaction) << std::endl;

        iterator = iterator->next;
    }
    file << TransactionHashedMessage(iterator->transaction);
}

bool BlockChainVerifyFile(const BlockChain& blockChain, std::ifstream& file){
    if(blockChain.size == 0){
        return true;
    }
    Node* iterator = blockChain.head;
    string line;
    while(std::getline(file, line)){
        ////    use the verify method of Transaction to check if the hashing is correct
        if(!TransactionVerifyHashedMessage(iterator->transaction, line)){
            return false;
        }
        iterator = iterator->next;
    }
    return true;
}

void BlockChainCompress(BlockChain& blockChain);

void BlockChainTransform(BlockChain& blockChain, updateFunction function);





