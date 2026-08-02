#include <iostream>
#include "application.h"

int runApplication(int argc, char* argv[]){
    if(argc != 2){
        std::cout << "Usage: source-inspector <source-file>" << std::endl;
        return 1;
    }
    std::cout << "Analyzing: " << argv[1] << std::endl;

    return 0;
}
