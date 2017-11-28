/*
* Timer class.
*/

#pragma once

#include <sys/time.h>
#include <cwchar>  // for NULL
#include <fstream>

class Timer {
public:
    Timer() {
        start_time.tv_sec  = 0;
        start_time.tv_usec = 0;
        stop_time.tv_sec   = 0;
        stop_time.tv_usec  = 0;
    }

    inline void start() {
        gettimeofday(&start_time, nullptr);
    }

    inline void stop() {
        gettimeofday(&stop_time, nullptr);
    }

    double get_timing() const {
        return (stop_time.tv_sec - start_time.tv_sec) + (stop_time.tv_usec - start_time.tv_usec)*1e-6;
    }

    void write_to_file(int iterIdx) const {
        std::ofstream file(TIMING_FILE, std::ios_base::app);
        file << iterIdx << ' ' << get_timing() << "\n";
        file.close();
    }

    void clean_timing_file() {
        if( remove( TIMING_FILE.c_str() ) != 0 )
            std::cout << "Could NOT delete timing file: " << TIMING_FILE.c_str() << std::endl;
        else
            std::cout << "Deleted timing file: " << TIMING_FILE.c_str() << std::endl;
    }

private:
    struct timeval start_time, stop_time;
    std::string const TIMING_FILE = "timing.txt";
};
