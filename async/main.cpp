//
//  main.cpp
//  async
//
//  Created by Sunday Idiakose on 24/12/2019.
//  Copyright © 2019 Sunday Idiakose. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include "file_work.hpp"

void displayOptions();
void handleSelection(const std::string&, const std::string&);

int main(int argc, const char * argv[]) {
    if (argc < 2)
    {
        std::cerr << "I expect an argument specifying the name/location of the file to be read" << std::endl;
        exit(-1);
    }
    std::cout << "File name/location: " << argv[1] << std::endl;
    
    std::vector<char> contents = readFile(argv[1]);
    
    if (argc == 2)
    {
        std::cout << "Attempting to read back file contents..." << std::endl;
        for (std::vector<char>::const_iterator i = contents.begin(); i != contents.end(); ++i)
            std::cout << *i << ' ';
    } else {
        displayOptions();
        handleSelection(argv[1], argv[2]);
    }
    
    return 0;
}

void displayOptions()
{
    std::cout << "How do you wish to proceed?: " << std::endl;
   std::cout << "1: Synchronously: \n"
   "2: Using Futures: \n"
   "3: Using Packaged Tasks: \n"
   "4: With std::async: \n"
   "5: With PPL(Parallel Patterns Library): \n"
   "6: Task Continuations \n"
    ">> ";
}

void handleSelection(const std::string& inPath, const std::string& outPath)
{
    std::string response = "1";
    
    if (!std::getline(std::cin, response)) return;
    
    std::vector<char> contents = readFile(inPath);
    
    switch (response.substr(0, 1).c_str()[0]) {
        case '1':
            std::cout << "Attempting to write file contents to " << outPath << "..." << std::endl;
            writeFile(contents, outPath);
            break;
            
        default:
            std::cerr << "Wrong selection" << std::endl;
            break;
    }
    
}
