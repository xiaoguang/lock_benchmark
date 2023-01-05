all:
	g++ -pthread thread.hpp thread.cpp locks.hpp ticks_clock.hpp ticks_clock.cpp tester.hpp run.cpp -o TESTS
clean:
	rm -f TESTS
