#ifndef TESTER_HPP
#define TESTER_HPP

#include "locks.hpp"
#include "thread.hpp"
#include <iostream>
#include <vector>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>

namespace base {

using std::vector;
using std::cout;
using std::endl;

class Tester {

 public:
  Tester(Lock * lock) : counter(new int(0))
           , thread_pool(NULL)
           , lock_(lock) {}
  ~Tester() {
    delete counter;
    if (thread_pool != NULL) {
      thread_pool->clear();
      delete thread_pool;
    }
  }
  int getCounter() { return *counter; }

  void CORRECTNESS_TEST (unsigned int decers, unsigned int incers, unsigned int load) {
    unsigned int i  = 0;
    unsigned int sz = decers + incers;
    thread_pool = new vector<pthread_t>(sz);
    for (i; i < incers; i++)
      (*thread_pool)[i] = makeThread(std::tr1::bind(&Tester::inc_counter, this, load));
    for (i; i < sz; i++)
      (*thread_pool)[i] = makeThread(std::tr1::bind(&Tester::dec_counter, this, load));
    thread_pool_join();
    assert(*counter == 0);
  }


  void TIMEOUT_CORRECTNESS_TEST (unsigned int decers, unsigned int incers, unsigned int load, unsigned int sl) {
    unsigned int i  = 0;
    unsigned int sz = decers + incers;
    thread_pool = new vector<pthread_t>(sz);
    for (i; i < incers; i++)
      (*thread_pool)[i] = makeThread(std::tr1::bind(&Tester::inc_counter_timeout, this, load, sl));
    for (i; i < sz; i++)
      (*thread_pool)[i] = makeThread(std::tr1::bind(&Tester::dec_counter_timeout, this, load, sl));
    thread_pool_join();
    assert(*counter == 0);
  }

 private:
  int *               counter;
  vector<pthread_t> * thread_pool;
  Lock *              lock_;

  inline void inc_counter(unsigned int load) {
    for (unsigned int i = 0; i < load; i++) {
      lock_->lock();
      *counter += 1;
      lock_->unlock();
    }
  }
  inline void dec_counter(unsigned int load) {
    for (unsigned int i = 0; i < load; i++) {
      lock_->lock();
      *counter -= 1;
      lock_->unlock();
    }
  }

  inline void inc_counter_timeout(unsigned int load, unsigned int sl) {
    for (unsigned int i = 0; i < load; i++) {
      lock_->lock();
      *counter += 1;
      usleep(sl);
      lock_->unlock();
    }
  }
  inline void dec_counter_timeout(unsigned int load, unsigned long sl) {
    for (unsigned int i = 0; i < load; i++) {
      lock_->lock();
      *counter -= 1;
      usleep(sl);
      lock_->unlock();
    }
  }

  void thread_pool_join() {
    vector<pthread_t>::iterator pool_iter;
    for (pool_iter = thread_pool->begin(); pool_iter != thread_pool->end(); pool_iter++)
      pthread_join(*pool_iter, NULL);
  }
  void thread_pool_exit() {
    vector<pthread_t>::iterator pool_iter;
    for (pool_iter = thread_pool->begin(); pool_iter != thread_pool->end(); pool_iter++)
      pthread_exit(NULL);
  }

};

} // namespace base

#endif // define TESTER_HPP
