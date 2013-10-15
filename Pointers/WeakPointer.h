#pragma once
#include <memory>
#include <unordered_map>

template <class T>
class WeakPointer {
  T* p_;

public:
  // Constructors
  WeakPointer(): p_(nullptr) { }
  WeakPointer(WeakPointer& wp): p_(wp.p_) { }
  WeakPointer(SharedPointer sp): p_(sp._p) { }

  ~WeakPointer() { }

  WeakPointer& operator=(WeakPointer& other) { p_ = other._p; }
  WeakPointer& operator=(SharedPointer<T>& other) { p_ = sp.p_; }

  // TODO operator bool() const { return p_; }

  SharedPointer<T> lock() const {
    expired() ? SharedPointer<T>(p_) : SharedPointer<T>();
  }


};

template <class T>
std::unordered_map<void*, int> WeakPointer<T>::counts = std::unordered_map<void*, int>();