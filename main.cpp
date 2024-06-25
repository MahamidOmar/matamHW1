#include <iostream>
#include <fstream>
#include <string>

#include "Utilities.h"
#include "BlockChain.h"

using std::string;

int main(int argc, char* argv[]){
    if(argc != 4){
        ////    print the error message from utilities
        std::cerr << getErrorMessage() << std::endl;
        return 1;
    }

    ////    assign the values received from the shell
    string op = argv[1];
    string source = argv[2];
    string target = argv[3];

    ////    open the files, and read the data of the blockchain
    std::ifstream source_file(source);

    ////    if the file did not open correctly
    if (!source_file) {
        std::cerr << getErrorMessage() << std::endl;
        return 1;
    }

    std::ofstream target_file(target);
    if (!target_file) {
        ////    don't forget to close the source file if opening the target failed
        source_file.close();
        std::cerr << getErrorMessage() << std::endl;
        return 1;
    }

    ////    create a new blockchain using the load method
    BlockChain blockChain = BlockChainLoad(source_file);

    ////    don't forget to close the source file after finished reading
    source_file.close();

    if(op == "format"){
        ////    print to the target file
        BlockChainDump(blockChain, target_file);
    }else if(op == "hash"){
        ////    print to the target file
        BlockChainDumpHashed(blockChain, target_file);
    }else if(op == "compress"){
        ////    compress the blockchain
        BlockChainCompress(blockChain);

        ////    print to the target file
        BlockChainDump(blockChain, target_file);
    }else if(op == "verify"){
        std::ifstream target_file_reading(target);
        ////    target file opened successfully and can read from it
        if(target_file_reading){
            if(BlockChainVerifyFile(blockChain, target_file_reading)){
                std::cout << "Verification passed" << std::endl;
            }else{
                std::cout << "Verification failed" << std::endl;
            }
            ////    can close the target file after reading from it
            target_file_reading.close();
        }else{
            std::cerr << getErrorMessage() << std::endl;
        }
    }else{
        ////    the op is not legal
        std::cerr << getErrorMessage() << std::endl;
    }

    ////    delete the blockchain after finished, so that no leaks are allowed
    destroyBlockChain(blockChain);

    ////    close the target file after printing to it
    target_file.close();
    return 0;
}


