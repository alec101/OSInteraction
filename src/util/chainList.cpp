#include <stdio.h>
#include "../../include/util/typeShortcuts.h"
#include "../../include/util/chainList.h"
//#define CHAINLIST_SAFECHECKS 1 //to check for bad calls to chainList

///---------------///
// chainData class //
///---------------///

chainData::~chainData() {
}

///---------------///
// chainList class //
///---------------///


chainList::chainList() {
  first= null;
  last= null;
  nrNodes= 0;
}


chainList::~chainList() {
  delData();
}

void chainList::delData() {
  while(first)
    del(first);
}

// must alloc from code, then call this func.
///------------------------------------------

void chainList::add(chainData *p2) {		
  if(last) {              /// list has members
    last->next= p2;
    p2->prev= last;
    p2->next= null;       /// do not depend on the constructor! it wont be called in the derived class! ...nice source of errors
    last= p2;
  } else {                /// list is empty
    first= last= p2;
    p2->next= null;       /// do not depend on the constructor! it wont be called in the derived class! ...nice source of errors
    p2->prev= null;       /// this is the place to initialize these vars
  }

  nrNodes++;
}

void chainList::addFirst(chainData *p2) {
  chainData *p= first;

  if(p) {                 // list has members
    p->prev= p2;
    p2->next= p;
    first= p2;
  } else {                // list is empty
    first= last= p2;
    p2->next= null;       /// do not depend on the constructor! it wont be called in the derived class! ...nice source of errors
    p2->prev= null;       /// this is the place to initialize these vars
  }

  nrNodes++;
}

// fast - few instructions - NO SEARCHES / PASSTHRUs
void chainList::del(chainData *p) {
  #ifdef CHAINLIST_SAFECHECKS
  if((!nrNodes) || (!p)) {
    //AfxMessageBox("strange error in chainList::delNode(chainData *)");
    return;
  }
  #endif

  /// make the links next/prev
  if(p->prev) p->prev->next= p->next;
  if(p->next) p->next->prev= p->prev;
  if(p== first) first= p->next;
  if(p== last) last= p->prev;
  /// delete
  delete p;
  nrNodes--;
}

// slow - goes thru the list, with many instructions per cicle
void chainList::deli(int nr) {
  #ifdef CHAINLIST_SAFECHECKS
  if(!nrNodes) return;
  if(nr> nrNodes) return;
  #endif
  
  /// find the item to be deleted
  chainData *p= first;
  for(int a= 0; a< nr; a++)       // <<< SLOW PART >>>
    p= p->next;

  /// make the next/prev links
  if(p->prev) p->prev->next= p->next;
  if(p->next) p->next->prev= p->prev;
  if(p== first) first= p->next;
  if(p== last) last= p->prev;

  /// delete
  delete p;
  nrNodes--;
}


///---------------------------------------------------------------///
// GET - SEARCH list funcs - not to be used regulary or in a cycle //
///---------------------------------------------------------------///

// get must be used rarely: if there's a for() {get()} it will pass n*n times thru list. if the list is 100000 items long... do the math... MANY zeroes of instructions...
chainData *chainList::get(int nr) {
  #ifdef CHAINLIST_SAFECHECKS
  if(!nrNodes) return null;
  if(nr> nrNodes) return null;
  #endif
  
  chainData *p= first;
  for(int a= 0; a< nr; a++)                   // <<< SLOW PART >>>
    p= p->next;

  return p;
}

// same as get, use RARELY
int chainList::search(chainData *e) {
  chainData *p= first;
  for(int a= 0; a< nrNodes; a++, p= p->next)  // <<< SLOW PART >>>
    if(p== e) return a;

  return -1;
}




