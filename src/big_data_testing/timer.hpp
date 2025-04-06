#ifndef TIMER_CLASS_H
#define TIMER_CLASS_H

#include <iostream>
#include <chrono>
#include <string>

class timer
{
    std::chrono::steady_clock::time_point start;

public:

    timer(): start(std::chrono::steady_clock::now()) {}

    ~timer()
    {
        auto end = std::chrono::steady_clock::now();

        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    
        std::cout << " " << seconds / 60 << "m " << seconds % 60 << "s" << std::endl;
    }
};

#endif