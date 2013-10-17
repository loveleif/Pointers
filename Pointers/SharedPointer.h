#pragma once
#include "WeakPointer.h"

template <class T> class WeakPointer;

struct Count {
  int shared_count, weak_count;

  Count(): shared_count(1), weak_count(0) { }
  bool DeleteIfPossible() {
    if (shared_count == 0 && weak_count == 0) {
      delete this;
      return true;
    }
    return false;
  }
  bool expired() { return shared_count == 0; }
};

// SharedPointer
// =============
template <class T>
class SharedPointer {
  template <class T> friend class WeakPointer;

  T* p_;
  Count* count_;
  
public:
  // --- Constructors
  SharedPointer(): p_(nullptr), count_(nullptr) { }
  SharedPointer(T* p): p_(p), count_(new Count) { }
  SharedPointer(T* p, Count* count): p_(p), count_(count) { 
    if (p_) {
      if (!count_) count_ = new Count;
      else ++count_->shared_count; 
    }
  }
  SharedPointer(std::nullptr_t p): SharedPointer() { }
  SharedPointer(SharedPointer& sp): SharedPointer(sp.p_, sp.count_) { }
  SharedPointer(WeakPointer<T>& wp): SharedPointer(wp.p_, wp.count_) { }

  ~SharedPointer() { Decrease(); }

  // --- Returns the pointer
  T* get() { return p_; }
  // --- Returns true if this is the only SharedPointer pointing at this pointer
  bool unique() { return _p && count_->shared_count == 1; }

  // --- Assignment
  SharedPointer& operator=(SharedPointer& other) {
    if (*this != other) {
      Decrease();
      p_ = other.p_;
      count_ = other.count_;
      if (p_) ++count_->shared_count;
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
  bool operator!=(SharedPointer& other) { return !(*this == other); }
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
    if (!p_) return;
    --count_->shared_count;
    if (count_->expired()) {
      delete p_;
      if (count_->DeleteIfPossible()) count_ = nullptr;
      p_ = nullptr;
    }
  }
};
