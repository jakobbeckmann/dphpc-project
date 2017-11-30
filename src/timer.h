/*
* Timer class.
*/

#pragma once

#include <chrono>
#include <fstream>

using c = std::chrono::steady_clock;

class Timer {
public:
    Timer() {
    }

    inline void start() {
        start_time = c::now();
    }

    inline void stop() {
        stop_time = c::now();
    }

    double get_timing() const {
        return std::chrono::duration_cast<std::chrono::microseconds>(stop_time-start_time).count() * 1.0e-6;
    }

    void write_to_file(int iterIdx) const {
        std::ofstream file(TIMING_FILE, std::ios_base::app);
        file << iterIdx << ' ' << get_timing() << "\n";
        file.close();
    }

    void clean_timing_file() {
        if( remove( TIMING_FILE.c_str() ) != 0 )
            std::cout << "Could NOT delete timing file: " << TIMING_FILE.c_str() << "\n";
        else
            std::cout << "Deleted timing file: " << TIMING_FILE.c_str() << "\n";
    }

private:
    c::time_point start_time, stop_time;
    std::string const TIMING_FILE = "timing.txt";
};
