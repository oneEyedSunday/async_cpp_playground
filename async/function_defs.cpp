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

size_t packaged_copyFile(const std::string& inFile, const std::string& outFile)
{
    using ReadTaskType = vector<char>(const string&);
    // define packaged task with signature of function to read
    packaged_task<ReadTaskType> readTask(readFile);
    
//    future<vector<char>> readFuture = readTask.get_future();
    future<vector<char>> readFuture { readTask.get_future() };// with initializer
    
    // pass task to thread with move, since it cannot be copied
    thread readThread { move(readTask), inFile };
    
    using WriteTaskType = size_t(const string&);
    
    // define the writeTask with a lamba
    // capture the readFuture by ref for reasons stated above
    // pass in the outPath
    packaged_task<WriteTaskType> writeTask([&readFuture](const string& outPath){
        return writeFile(readFuture.get(), outPath);
    });
    
    // a packaged task just wraps a promise
    future<size_t> writeFuture{ writeTask.get_future() };
    
    thread writeThread { move(writeTask), outFile };
    
    
    readThread.join();
    writeThread.join();
    return writeFuture.get();
    // really cannot see how this helps or simplifies
}

size_t async_copyFile(const string& inFile, const string& outFile)
{
    future<vector<char>> readFuture = async(readFile, inFile);
    auto copyFuture = async([&readFuture](const string& path){
        return writeFile(readFuture.get(), path);
    }, outFile);
    
    return copyFuture.get();
}
