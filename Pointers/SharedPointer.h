#pragma once
#include <memory>
#include <iostream>
#include <unordered_map>
#include "WeakPointer.h"
using namespace std;

template <class T> class WeakPointer;

struct Count {
  int shared_count_;
  int weak_count_;
  Count(): shared_count_(1), weak_count_(0) { }
  Count& AddShared() { ++shared_count_; return *this; }
  Count& AddWeak() { ++weak_count_; return *this; }
  Count& RemoveShared() { --shared_count_; return *this; }
  Count& RemoveWeak() { --weak_count_; return *this; }
  bool expired() { return shared_count_ == 0; }
  bool removable() { return shared_count_ == 0 && weak_count_ == 0; }
};

// SharedPointer
// =============
template <class T>
class SharedPointer {
  template <class T> friend class WeakPointer;

  T* p_; // The pointer
  Count* count_;
  
public:
  // --- Constructors
  SharedPointer(): p_(nullptr) { }
  SharedPointer(T* p): p_(p), count_(new Count) { }
  SharedPointer(SharedPointer& sp): p_(p), count_(sp.count_->AddShared()) { }
  SharedPointer(std::nullptr_t p): SharedPointer() { }
  SharedPointer(WeakPointer<T>& wp): p_(wp.p_), count_(wp.count_->AddWeak()) { }

  ~SharedPointer() { 
    Decrease();
    if (
  }

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

private:
  void Decrease() {
    count_->RemoveShared();
    if (count_->expired())
      delete p_;
  }
};

unordered_map<void*, Count> Count::counts = unordered_map<void*, Count>();