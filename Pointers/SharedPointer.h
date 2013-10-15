#pragma once
#include <memory>
#include <iostream>
#include <unordered_map>
#include "WeakPointer.h"
using namespace std;

template <class T> class WeakPointer;

class Count {
  static unordered_map<void*, Count> counts;
public:
  int count, incarnation;
  Count(): count(0), incarnation(0) { }
  static Count& get(void* p) { return counts[p]; };
  static Count& at(void* p) { return counts.at(p); };
};

template <class T>
class SharedPointer {
  template <class T> friend class WeakPointer;

  T* p_;

  void Decrease() {
    if (p_ && --Count::at(p_).count == 0) {
      ++Count::at(p_).incarnation;
      delete p_;
    }
  }
  
public:
  Count& Count() { return Count::at(p_); }
  // Constructors
  SharedPointer(): p_(nullptr) { }
  SharedPointer(T* p): p_(p) { ++Count::get(p_).count; }
  SharedPointer(SharedPointer& sp): SharedPointer(sp.p_) { }
  SharedPointer(std::nullptr_t p): SharedPointer() { }
  SharedPointer(WeakPointer<T>& wp): SharedPointer(wp.p_) { }

  ~SharedPointer() {
    Decrease();
  }

  T* get() { return p_; }
  bool unique() { return _p && Counts::counts[p_] == 1; }

  SharedPointer& operator=(SharedPointer& other) {
    if (*this != other) {
      Decrease();
      p_ = other.p_;
      ++Count::get(p_).count;
    }
    return *this;
  }

  SharedPointer& operator=(std::nullptr_t other) {
    Decrease();
    p_ = nullptr;
    return *this;
  }

  bool operator==(SharedPointer& other) { return p_ == other.p_; }
  bool operator==(std::nullptr_t other) { return p_ == nullptr; }
  bool operator<(SharedPointer& other) { return p_ < other.p_; }
  bool operator<(std::nullptr_t other) { return false; }


  T& operator*() { return *p_; }
  const T& operator*() const { return *p_; }
  T& operator->() { return p_; }
  const T& operator->() const { return p_; }

  operator bool() const { return p_; }
};

unordered_map<void*, Count> Count::counts = unordered_map<void*, Count>();