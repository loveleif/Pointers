#pragma once
#include <memory>
#include <unordered_map>
#include "SharedPointer.h"

template <class T> class SharedPointer;

template <class T>
class WeakPointer {
  template <class T> friend class SharedPointer;

  T* p_;
  int incarnation_;

public:
  // Constructors
  WeakPointer(): p_(nullptr) { }
  WeakPointer(WeakPointer& wp): p_(wp.p_), incarnation_(wp.incarnation_) { }
  WeakPointer(SharedPointer<T>& sp): p_(sp.p_), incarnation_(sp.Count().incarnation) { }

  ~WeakPointer() { }

  WeakPointer& operator=(WeakPointer& other) {
    p_ = other.p_;
    incarnation_ = other.incarnation_;
    return *this;
  }
  WeakPointer& operator=(SharedPointer<T>& sp) {
    p_ = sp.p_;
    incarnation_ = sp.Count().incarnation;
    return *this;
  }

  operator bool() const { return lock(); }

  SharedPointer<T> lock() const {
    return expired() ? SharedPointer<T>() : SharedPointer<T>(p_);
  }

  bool expired() const {
    if (!p_) return true;

    Count count = Count::at(p_);
    return count.count == 0 || count.incarnation != incarnation_;
  }
};
