//
//  file_work.hpp
//  async
//
//  Created by Sunday Idiakose on 24/12/2019.
//  Copyright © 2019 Sunday Idiakose. All rights reserved.
//


#if __has_include("ppltasks")
    #include <ppltasks>
    #define PPL 1
#else
    #define PPL 0
#endif


#ifndef file_work_hpp
#define file_work_hpp

#include <stdio.h>
#include <string>
#include <vector>

std::vector<char> readFile(const std::string&);

size_t writeFile(const std::vector<char>&, const std::string& outPath);

size_t sync_copyFile(const std::string&, const std::string&);

size_t future_copyFile(const std::string&, const std::string&);

size_t packaged_copyFile(const std::string&, const std::string&);

size_t async_copyFile(const std::string&, const std::string&);

size_t ppl_copyFile(const std::string&, const std::string&);

size_t ppl_read_then_copy(const std::string&, const std::string&);
#endif /* file_work_hpp */
