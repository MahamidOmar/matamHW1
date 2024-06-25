#include "BlockChain.h"

void destroyBlockChain(BlockChain& blockChain){
    ////    blockchain is currently empty, nothing to delete
    if(blockChain.size == 0){
        return;
    }
    Node* iterator = blockChain.head;
    while(iterator != nullptr){
        Node* toDelete = iterator;
        iterator = iterator->next;
        delete toDelete->transaction;
        delete toDelete;
    }
    ////    return the blockChain to it's original / starting state
    blockChain.head = nullptr;
    blockChain.size = 0;
}

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
    Node* newNode = new Node();
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

void BlockChainAppendTransaction(
        BlockChain& blockChain,
        const Transaction& transaction,
        const string& timestamp
){
    ////    create a new node and new transaction to append to the blockchain
    Node* newNode = new Node();
    newNode->transaction = new Transaction();
    ////    copy the values of the given transaction to the new one (the one to be appended)
    newNode->transaction->sender = transaction.sender;
    newNode->transaction->receiver = transaction.receiver;
    newNode->transaction->value = transaction.value;
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
    ////    this holds the current line read from the file
    string line;
    ////    iterator for adding the nodes to the blockchain
    Node* iterator = nullptr;
    ////    pointer to the head of the new blockchain that is returned
    Node* head = nullptr;
    ////    holds the size of the blockchain
    unsigned int size = 0;
    while(std::getline(file, line)){
        string sender, receiver, timestamp;
        unsigned int value;
        ////    find the index in which every word in the line ends (positions of the spaces)
        int sender_position = line.find_first_of(' ');
        int receiver_position = line.find_first_of(' ', sender_position + 1);
        int value_position = line.find_first_of(' ', receiver_position + 1);

        ////    parse the line
        sender = line.substr(0, sender_position);
        receiver = line.substr(sender_position + 1, receiver_position - sender_position - 1);
        ////    string to save the value before transforming it to an int
        string value_string = line.substr(receiver_position + 1, value_position - receiver_position - 1);
        value = std::stoi(value_string);
        timestamp = line.substr(value_position + 1);

        //// create a new transaction to be added to the blockchain
        Transaction* transaction = new Transaction();
        transaction->sender = sender;
        transaction->receiver = receiver;
        transaction->value = value;

        ////    create a new node to include the new transaction
        Node* new_node = new Node();
        new_node->transaction = transaction;
        new_node->timeStamp = timestamp;

        ////    the case of which this is the first line read from the file, which means the first transaction in the file
        if(iterator == nullptr){
            iterator = new_node;
            head = new_node;
        }else{
            ////    need to append to the ending / tail of the blockchain
            iterator->next = new_node;
            iterator = iterator->next;
        }

        ++size;
    }
    BlockChain blockChain = {head, size};
    return blockChain;
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
        TransactionDumpInfo(*(iterator->transaction), file);

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
        file << TransactionHashedMessage(*(iterator->transaction)) << std::endl;

        iterator = iterator->next;
    }
    file << TransactionHashedMessage(*(iterator->transaction));
}

bool BlockChainVerifyFile(const BlockChain& blockChain, std::ifstream& file){
    ////    blockchain is empty, nothing to verify
    if(blockChain.size == 0){
        return true;
    }
    Node* iterator = blockChain.head;
    string line;
    while(std::getline(file, line)){
        ////    use the verify method of Transaction to check if the hashing is correct
        if(!TransactionVerifyHashedMessage(*(iterator->transaction), line)){
            return false;
        }
        iterator = iterator->next;
    }
    return true;
}

void BlockChainCompress(BlockChain& blockChain){
    ////    blockchain is empty, nothing to compress
    if(blockChain.size == 0){
        return ;
    }
    Node* iterator = blockChain.head;
    ////    check if the next is null because we check to compress each 2 consecutive nodes
    while (iterator->next != nullptr){
        Transaction* current_transaction = iterator->transaction;
        Transaction* next_transaction = iterator->next->transaction;
        ////    check if the 2 transactions have same sender and receiver
        if(current_transaction->sender == next_transaction->sender && current_transaction->receiver == next_transaction->receiver){
            current_transaction->value += next_transaction->value;

            ////    add a pointer to the next node which we are going to delete
            Node* to_delete = iterator->next;

            ////    move the next of the current node 2 nodes ahead
            iterator->next = to_delete->next;

            ////    delete the compressed node (the second one)
            delete to_delete->transaction;
            delete to_delete;

            ////    in this case we don't move the iterator because we want to check if we can compress it again with another node ahead
        }else{
            iterator = iterator->next;
        }
    }
}

void BlockChainTransform(BlockChain& blockChain, updateFunction function);





