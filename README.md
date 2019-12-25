# To compile 
```sh
cd async && 
g++ -c file_work.cpp function_defs.cpp main.cpp --std=c++11 && 
g++ file_work.o function_defs.o main.o -o ${PATH_TO_EXEC}/run.sh &&
${PATH_TO_EXEC}/run.sh inputfile outputfile
```


# TODO
* makefile


# Sources
* Paolo Severini's waste of bandwidth😉 [Concurrency in C++](https://paoloseverini.wordpress.com/2014/04/07/concurrency-in-c11/) 


