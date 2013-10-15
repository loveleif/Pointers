#pragma once
#include <memory>
#include <vector>
using namespace std;

  struct Count {
    void* vp_;
    int count;
    int incarnation;

    int operator++() { return ++count; }
    int operator--() { 
      if (--count == 0) ++incarnation;
      return count;
    }
    operator int() const { return count; }
  };

template <class T>
class SharedPointer {
  friend class WeakPointer;

  T* p_;
  int index_;
  
  static vector<Count> counts;

  void Increase() {
    
  }
  void Decrease() {
    if (p_ && --counts[index_] == 0) {
      delete p_;
    }
  }
  Count& Find(void* p) {
    for (auto iter = counts.cbegin(); iter->vp_ != p && iter != counts.cend(); ++iter);
    if (iter == counts.cend()) {
      counts.push_back({ p, 0, 0 });
      index_ = counts.size() - 1;
    }



  }
public:
  // Constructors
  SharedPointer(): p_(nullptr) { }
  SharedPointer(T* p): p_(p) {
    //find(counts.cbegin(), counts.cend(), 
    ++counts[index_];
  }
  SharedPointer(SharedPointer& sp): SharedPointer(sp.p_) { }
  SharedPointer(std::nullptr_t p): SharedPointer() { }
  //SharedPointer(WeaksPointer& p);

  ~SharedPointer() {
    Decrease();
  }

  T* get() { return p_; }
  bool unique() { return _p && counts[index_] == 1; }

  SharedPointer& operator=(SharedPointer& other) {
    Decrease();
    p_ = other.p_;
    ++counts[index_];
    return *this;
  }

  SharedPointer& operator=(std::nullptr_t other) {
    Decrease();
    p_ = nullptr;
  }

  bool operator==(SharedPointer& other) { return p_ == other.p_; }

  bool operator==(std::nullptr_t other) { return p_ == nullptr; }

  T& operator*() { return *p_; }
  const T& operator*() const { return *p_; }
  T& operator->() { return p_; }
  const T& operator->() const { return p_; }

  operator bool() const { return p_; }
};

template <class T>
vector<Count> SharedPointer<T>::counts = vector<Count>();