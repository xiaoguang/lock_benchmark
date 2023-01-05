#include "tester.hpp"
#include "ticks_clock.hpp"
#include <iostream>
#include <stdlib.h>

using std::cout;
using std::endl;
using namespace base;

struct Node {
  uint64_t a, b, c, d, e, f, g, h;
};

void TEST1(unsigned int incers, unsigned int decers, unsigned int load) {
  Lock   * lock   = new Mutex();
  Tester * tester = new Tester(lock);
  TicksClock::Ticks start = TicksClock::getTicks();
  tester->CORRECTNESS_TEST(incers, decers, load);
  TicksClock::Ticks stop  = TicksClock::getTicks();
  cout << "PASS : MUTEX "<< incers << " INCER(s) " << decers << " DECER(s) " << load << " LOAD(s)" << endl;
  cout << "DURATION : (CLOCK CYCLE) "<< stop - start << endl;
  delete tester;
  delete lock;
  return;
}

void TEST2(unsigned int incers, unsigned int decers, unsigned int load) {
  Lock   * lock   = new SpinLock();
  Tester * tester = new Tester(lock);
  TicksClock::Ticks start = TicksClock::getTicks();
  tester->CORRECTNESS_TEST(incers, decers, load);
  TicksClock::Ticks stop  = TicksClock::getTicks();
  cout << "PASS : SPINLOCK "<< incers << " INCER(s) " << decers << " DECER(s) " << load << " LOAD(s)" << endl;
  cout << "DURATION : (CLOCK CYCLE) "<< stop - start << endl;
  delete tester;
  delete lock;
  return;
}

void TEST3(unsigned int incers, unsigned int decers, unsigned int load) {
  Lock   * lock   = new TASpinLock();
  Tester * tester = new Tester(lock);
  TicksClock::Ticks start = TicksClock::getTicks();
  tester->CORRECTNESS_TEST(incers, decers, load);
  TicksClock::Ticks stop  = TicksClock::getTicks();
  cout << "PASS : TASpinLock "<< incers << " INCER(s) " << decers << " DECER(s) " << load << " LOAD(s)" << endl;
  cout << "DURATION : (CLOCK CYCLE) "<< stop - start << endl;
  delete tester;
  delete lock;
  return;
}


void TEST4(unsigned int incers, unsigned int decers, unsigned int load, unsigned int sl) {
  Lock   * lock   = new Mutex();
  Tester * tester = new Tester(lock);
  TicksClock::Ticks start = TicksClock::getTicks();
  tester->TIMEOUT_CORRECTNESS_TEST(incers, decers, load, sl);
  TicksClock::Ticks stop  = TicksClock::getTicks();
  cout << "PASS : MUTEX "<< incers << " INCER(s) " << decers << " DECER(s) " << load << " LOAD(s) " << sl << " USLEEP" << endl;
  cout << "DURATION : (CLOCK CYCLE) "<< stop - start << endl;
  delete tester;
  delete lock;
  return;
}

void TEST5(unsigned int incers, unsigned int decers, unsigned int load, unsigned int sl) {
  Lock   * lock   = new SpinLock();
  Tester * tester = new Tester(lock);
  TicksClock::Ticks start = TicksClock::getTicks();
  tester->TIMEOUT_CORRECTNESS_TEST(incers, decers, load, sl);
  TicksClock::Ticks stop  = TicksClock::getTicks();
  cout << "PASS : SPINLOCK "<< incers << " INCER(s) " << decers << " DECER(s) " << load << " LOAD(s) " << sl << " USLEEP" << endl;
  cout << "DURATION : (CLOCK CYCLE) "<< stop - start << endl;
  delete tester;
  delete lock;
  return;
}

void TEST6(unsigned int incers, unsigned int decers, unsigned int load, unsigned int sl) {
  Lock   * lock   = new TASpinLock();
  Tester * tester = new Tester(lock);
  TicksClock::Ticks start = TicksClock::getTicks();
  tester->TIMEOUT_CORRECTNESS_TEST(incers, decers, load, sl);
  TicksClock::Ticks stop  = TicksClock::getTicks();
  cout << "PASS : TASpinLock "<< incers << " INCER(s) " << decers << " DECER(s) " << load << " LOAD(s) " << sl << " USLEEP" << endl;
  cout << "DURATION : (CLOCK CYCLE) "<< stop - start << endl;
  delete tester;
  delete lock;
  return;
}

int main (int argc, char ** argv) {
  unsigned int incer = strtoul(argv[1], NULL, 0);
  unsigned int decer = strtoul(argv[2], NULL, 0);
  unsigned int load  = strtoul(argv[3], NULL, 0);
  unsigned int sl    = strtoul(argv[4], NULL, 0);
  TEST1(incer, decer, load);
  TEST2(incer, decer, load);
  TEST3(incer, decer, load);
  TEST4(incer, decer, load, sl);
  TEST5(incer, decer, load, sl);
  TEST6(incer, decer, load, sl);
  cout << sizeof(Node) << endl;
  return 1;
}
