#include "source_analyzer.h"
#include <fstream>
#include <string>

int countLines(const char* filePath){
    std::ifstream input(filePath);

    if(!input.is_open()){
        return -1;
    }

    int count = 0;
    std::string line;

    while(std::getline(input, line)){
        count++;
    }

    return count;
}
