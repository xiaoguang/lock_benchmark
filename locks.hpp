#ifndef BASE_LOCK_HPP
#define BASE_LOCK_HPP

#include <stdio.h>
#include <pthread.h>

namespace base {

class Lock {

 public:
  Lock() {}
  virtual ~Lock() {}
  virtual void lock() = 0;
  virtual void unlock() = 0;

};

class Mutex : public Lock {

 public:
  Mutex()               { pthread_mutex_init(&m_, NULL); }
  virtual ~Mutex()      { pthread_mutex_destroy(&m_); }

  void lock()   { pthread_mutex_lock(&m_); }
  void unlock() { pthread_mutex_unlock(&m_); }

 private:
  pthread_mutex_t m_;
  // non-copyable, non-assignable
  Mutex(Mutex &);
  Mutex & operator= (Mutex &);

};

class ScopedMutex {

 public:
  // avoid internal coercion
  explicit ScopedMutex(Mutex * mutex) : m_(mutex)  { m_->lock(); }
  ~ScopedMutex()                                   { m_->unlock(); }

 private:
  Mutex * m_;
  // non-copyable, non-assignable
  ScopedMutex(ScopedMutex &);
  ScopedMutex & operator= (ScopedMutex &);

};

class SpinLock : public Lock {

 public:
  SpinLock() : l_(false) {}
  virtual ~SpinLock() {}

  void lock() {
    while (!__sync_bool_compare_and_swap(&l_, false, true));
  }

  void unlock() {
    __sync_lock_test_and_set(&l_, false);
  }

 private:
  bool l_;
  // non-copyable, non-assignable
  SpinLock(SpinLock &);
  SpinLock & operator= (SpinLock &);

};

// Test And(TA) spin lock
class TASpinLock : public Lock {

 public:
  TASpinLock() : l_(false) {}
  virtual ~TASpinLock() {}

  void lock() {
    // test l_ on local cache
    while (true) {
      while (l_);
      if (__sync_bool_compare_and_swap(&l_, false, true))
        return;
    }
  }

  void unlock() {
    __sync_lock_test_and_set(&l_, false);
  }

 private:
  bool l_;
  // non-copyable, non-assignable
  TASpinLock(TASpinLock &);
  TASpinLock & operator= (TASpinLock);

};

// Array Queue Lock
class ArrayLock : public Lock {

 public:
  ArrayLock() : mySlot(0) {}
  virtual ~ArrayLock() {}

 private:
  __thread int mySlot;
};

} // namespace base

#endif // BASE_LOCK_HPP
