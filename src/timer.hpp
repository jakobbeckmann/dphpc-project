/*
* Timer class.
*/

#include <sys/time.h>
#include <cwchar>  // for NULL

class timer {
public:
	timer() {
		start_time.tv_sec  = 0;
		start_time.tv_usec = 0;
		stop_time.tv_sec   = 0;
		stop_time.tv_usec  = 0;
	}

	inline void start() {
		gettimeofday(&start_time, NULL);
	}

	inline void stop() {
		gettimeofday(&stop_time, NULL);
	}

	double get_timing() const {
		return (stop_time.tv_sec - start_time.tv_sec) + (stop_time.tv_usec - start_time.tv_usec)*1e-6;
	}

	void write_to_file(int const n_cores) const {
        std::ofstream file(TIMING_FILE, std::ios_base::app);
        file << n_cores << "," << get_timing() << "\n";
	}

	void clean_timing_file() {
		if( remove( TIMING_FILE.c_str() ) != 0 )
			std::cout << "Could NOT delete timing file: " << TIMING_FILE.c_str() << std::endl;
		else
			std::cout << "Deleted timing file: " << TIMING_FILE.c_str() << std::endl;
	}

private:
	struct timeval start_time, stop_time;
    std::string const TIMING_FILE = "../Output/timing.txt";
};

