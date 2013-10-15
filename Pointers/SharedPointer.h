#pragma once
#include <memory>
#include <unordered_map>
#include "WeakPointer.h"
using namespace std;

template <class T> class WeakPointer;

struct Count {
  int count;
  int incarnation;

  Count(): count(0), incarnation(0) { }
  int operator++() { return ++count; }
  int operator--() { 
    if (--count == 0) ++incarnation;
    return count;
  }
  operator int() const { return count; }
};

template <class T>
class SharedPointer {
   template <class T> friend class WeakPointer;

  T* p_;
  
  static unordered_map<void*, Count> counts;

  void Decrease() {
    if (p_ && --counts[p_] == 0) {
      delete p_;
    }
  }
  Count& Count() { return counts.at(p_); }
public:
  // Constructors
  SharedPointer(): p_(nullptr) { }
  SharedPointer(T* p): p_(p) { ++counts[p_]; }
  SharedPointer(SharedPointer& sp): SharedPointer(sp.p_) { }
  SharedPointer(std::nullptr_t p): SharedPointer() { }
  SharedPointer(WeakPointer<T>& wp): SharedPointer(wp.p_) { }

  ~SharedPointer() {
    Decrease();
  }

  T* get() { return p_; }
  bool unique() { return _p && counts[p_] == 1; }

  SharedPointer& operator=(SharedPointer& other) {
    if (*this != other) {
      Decrease();
      p_ = other.p_;
      ++counts[p_];
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

template <class T>
unordered_map<void*, Count> SharedPointer<T>::counts = unordered_map<void*, Count>();