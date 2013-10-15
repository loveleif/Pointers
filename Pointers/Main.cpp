#include "SharedPointer.h"
#include <iostream>
using namespace std;

int main() {
  int a,b,c,d;
  int *pa, *pb, *pc, *pd;
  pa = &a;
  pb = &b;
  pc = &c;
  pd = &d;
  SharedPointer<int> s1(pa);
  SharedPointer<int> s2(pa);

  return 0;
}