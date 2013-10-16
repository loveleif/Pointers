#pragma once
#include <memory>
#include <iostream>
#include <unordered_map>
#include "WeakPointer.h"
using namespace std;

template <class T> class WeakPointer;

// Count
// =====
class Count {
  // Maps pointer to Counts
  static unordered_map<void*, Count> counts;
public:
  int count; // Number of SharedPointers
  int incarnation; // Incarnation of this pointer, incremented at every delete
  Count(): count(0), incarnation(0) { }
  // Returns the Count for the given pointer, creates a new Count if neccessary.
  static Count& get(void* p) { return counts[p]; };
  // Returns the Count for the given pointer, throws exception if p isn't mapped.
  static Count& at(void* p) { return counts.at(p); };
};

// SharedPointer
// =============
template <class T>
class SharedPointer {
  template <class T> friend class WeakPointer;

  T* p_; // The pointer

  // --- Decreases count by one for this pointer. Calls delete if count == 0.
  void Decrease() {
    if (p_ && --Count::at(p_).count == 0) {
      ++Count::at(p_).incarnation;
      delete p_;
    }
  }
  
public:
  // --- Constructors
  SharedPointer(): p_(nullptr) { }
  SharedPointer(T* p): p_(p) { ++Count::get(p_).count; }
  SharedPointer(SharedPointer& sp): SharedPointer(sp.p_) { }
  SharedPointer(std::nullptr_t p): SharedPointer() { }
  SharedPointer(WeakPointer<T>& wp): SharedPointer(wp.p_) { }

  ~SharedPointer() { Decrease(); }

  // --- Returns the pointer
  T* get() { return p_; }
  // --- Returns true if this is the only SharedPointer pointing at this pointer
  bool unique() { return _p && Counts::counts[p_] == 1; }
  // --- Returns the Count of this SmartPointer
  Count& Count() { return Count::at(p_); }

  // --- Assignment
  SharedPointer& operator=(SharedPointer& other) {
    if (*this != other) {
      Decrease();
      p_ = other.p_;
      ++Count::at(p_).count;
    }
    return *this;
  }
  SharedPointer& operator=(std::nullptr_t other) {
    Decrease();
    p_ = nullptr;
    return *this;
  }

  // --- Relational operators
  bool operator==(SharedPointer& other) { return p_ == other.p_; }
  bool operator==(std::nullptr_t other) { return p_ == nullptr; }
  bool operator<(SharedPointer& other) { return p_ < other.p_; }
  bool operator<(std::nullptr_t other) { return false; }

  // --- Access
  T& operator*() { return *p_; }
  const T& operator*() const { return *p_; }
  T& operator->() { return p_; }
  const T& operator->() const { return p_; }

  // --- bool representation
  operator bool() const { return p_ ? true : false; }
};

unordered_map<void*, Count> Count::counts = unordered_map<void*, Count>();