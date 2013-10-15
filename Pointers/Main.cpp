#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <cassert>
#include <assert.h>
#include <iostream>
using namespace std;

#include "SharedPointer.h"
#include "WeakPointer.h"


void TestG() {
	//-	Konstruktor som tar:	
	//	o	inget	G
	//	o	En SharedPointer	G
	//	o	En pekare	G


	SharedPointer<float>p11;
	assert(!p11);
	SharedPointer<float>p12(new float);
	assert(p12);
	SharedPointer<float>p13(p11);
	assert(!p13);
	SharedPointer<float>p14(p12);
	assert(p14);
	SharedPointer<float>p15(nullptr);
	assert(!p15);

	//-	Destruktor	G
	//It will test itself
	//-	Tilldelning från en	
	//	o	En SharedPointer	G
	//	o	En nullptr	G
	p14=nullptr;
	assert(!p14);
	p14=p12;
	assert(p14);

	p14=p14;
	assert(p14);
	//-	Jämförelse med (== och <)
	SharedPointer<float> p31(new float);
	//	o	En SharedPointer	G
	assert(p11==nullptr);
	assert(p11<p12);
	assert(!(p12<p11));
	assert(p14==p12);
	assert(!(p14==p31));
	assert((p14<p31) || (p31<p14));
}


//OBS! För att testprogrammet ska fungera krävs en "operator bool" i WeakPointer
//Den är egentligen inte bra men underlättar testprogrammt!
void TestVG(){
//Weak pointer skall ha det som det står VG på nedan
//-	Konstruktor som tar:		
//	o	inget	G	VG
//	o	En SharedPointer	G	VG
//	o	En WeakPointer	VG	VG

  
	WeakPointer<float>wp11;
	assert(!wp11);
  
	SharedPointer<float>sp12(new float);
	WeakPointer<float>wp13(wp11);
	assert(!wp13);
	WeakPointer<float>wp14(sp12);
	assert(wp14);

	SharedPointer<float>sp17(wp14);
  
//-	Destruktor	G	VG
	//It will test itself
//-	Tilldelning från en		
//	o	En SharedPointer	G	VG
//	o	En WeakPointer		VG
  
  
	WeakPointer<float>wp15;
	wp14=wp11;
	assert(!wp14);
	wp14=sp12;
	assert(wp14);
	wp14=wp14;
	assert(wp14);

	assert(!wp15);
	wp15=wp14;
	assert(wp15);


//-	funktioner:		
//	o	lock()		VG
	auto sp51=wp11.lock();
	assert(!sp51);
	sp51=wp14.lock();
	assert(sp51);
//	o	expired()		VG
	assert(!wp15.expired());
	wp15=SharedPointer<float>(nullptr);
	assert(wp15.expired());
  
}

int main() {
  _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	TestG();
	TestVG();
	//system("Pause");
}