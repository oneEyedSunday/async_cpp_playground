//
//  function_defs.cpp
//  async
//
//  Created by Sunday Idiakose on 24/12/2019.
//  Copyright © 2019 Sunday Idiakose. All rights reserved.
//

#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <future>

using namespace std;


vector<char> readFile(const string& inPath)
{
    ifstream file(inPath, ios::binary | ios::ate);
    size_t length = (size_t)file.tellg();
    vector<char> buffer(length);
    file.seekg(0, std::ios::beg);
    file.read(&buffer[0], length);
    return buffer;
}

size_t writeFile(const vector<char>& buffer, const string& outPath)
{
    ofstream file(outPath, ios::binary);
    file.write(&buffer[0], buffer.size());
    return (size_t)file.tellp();
}

size_t sync_copyFile(const string& inFile, const string& outFile)
{
    return writeFile(readFile(inFile), outFile);
}


size_t future_copyFile(const string& inFile, const string& outFile)
{
    promise<vector<char> > readPromise;
    future<vector<char> > readFuture = readPromise.get_future();
    
    // pass a lambda to the thread
    // capturing by reference the readPromise to make them available
    // and the input file name
    thread readFileThread([&readPromise, inFile]() {
        readPromise.set_value(readFile(inFile));
    });
    
    promise<size_t> writePromise;
    future<size_t> writeFuture = writePromise.get_future();
    
    thread writeFileThread([&readFuture, &writePromise, outFile]() {
        // this waits on read thread since .get() is blocking
        writePromise.set_value(writeFile(readFuture.get(), outFile));
    });
    
    readFileThread.join();
    writeFileThread.join();
    
    return writeFuture.get();
}

