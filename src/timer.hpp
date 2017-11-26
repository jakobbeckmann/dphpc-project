/*
* Timer class.
*/

#include <chrono>

// TODO: Check if this is steady everywhere we care about, if it isn't consider using steady_clock instead.
using hrc = std::chrono::high_resolution_clock;

class timer {
public:
	timer() {
		start = hrc::now();
	}

	~timer() {
		hrc::time_point end = hrc::now();
		std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end-start).count() << "\n";
	}

private:
	hrc::time_point start;
};

