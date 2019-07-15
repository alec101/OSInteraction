#pragma once
#ifndef CHAINLIST_DEFINED
#define CHAINLIST_DEFINED 1

#include <stdint.h>

//#define CHAINLIST_SAFECHECKS 1 //to check for bad calls to chainList

#ifndef CHAINLIST_DEALLOC
#define CHAINLIST_DEALLOC(x) delete x
#endif



// DERIVE classes from this one & DEFINE what vars are in the node inside it
class chainData {
public:
  chainData *next, *prev;

  /// don't create a constructor, it won't be called by derived class... big source of errors
  virtual ~chainData() {}     // being virtual, it seems delete[] knows to dealloc the derived parts... it never crashed/ no garbage remains in memory
};




/// manipulator class, no derives necesary, just make a var from this one
struct chainList {
  chainData *first, *last;    // last is important for add(), else there has to be a passthru till the last member (if the list is 100k long?)
  uint32_t nrNodes;           // VERY USEFUL - number of nodes in list

  // fast funcs
  
  void add(chainData *);      // [FAST] alloc mem, then pass pointer to add(). fast function, no searches involved
  void addFirst(chainData *); // [FAST] adds the node to the front of the list not the back (makes it first)
  void addAfter(chainData *, chainData *afterPoint);   // [FAST] alloc mem, then pass pointer. it will be inserted in the list, after the specified chainNode
  void addBefore(chainData *, chainData *beforePoint); // [FAST] alloc mem, then pass pointer. it will be inserted in the list, before the specified chainNode
  void del(chainData *);      // [FAST] deletes specified chainList - NO searches involved
  void release(chainData *);  // [FAST] releases object from the chainList, doesn't delete the object from memory - NO searches involved

  // slow funcs

  // [SLOW] dels specified item number - searches involved - SLOW for large lists 
  void deli(uint32_t);

  void addi(chainData *, uint32_t);// [SLOW] alloc mem, then pass pointer. it will be inserted in the list, at the specified position. searches involved - SLOW for large lists
  void releasei(uint32_t);         // [SLOW] releases selected object (based on it's index number) from the chainList, doesn't delete the object from memory - SLOW for large lists
  chainData *get(uint32_t);        // [SLOW] returns specified item number - searches involved - SLOW for large lists
  uint32_t search(chainData *);    // [SLOW] returns ~0u (max uint32) if failed to find - SLOW for large lists

  bool isMember(chainData *); // [SLOW] returns true if the chainData is part of this list

  // constructor / destructor

  chainList(): first(nullptr), last(nullptr), nrNodes(0) {}
  ~chainList() { delData(); }
  void delData() { while(first) del(first); } // teh real destructor (tm) - can be called at any time to dealloc everything
}; /// chainList class





















///========================///
// SRC =========----------- //
///========================///


// must alloc from code, then call this func.
///------------------------------------------

inline void chainList::add(chainData *p2) {		
  if(last) {              /// list has members
    last->next= p2;
    p2->prev= last;
    p2->next= nullptr;    /// do not depend on the constructor! it wont be called in the derived class! ...nice source of errors
    last= p2;
  } else {                /// list is empty
    first= last= p2;
    p2->next= nullptr;    /// do not depend on the constructor! it wont be called in the derived class! ...nice source of errors
    p2->prev= nullptr;    /// this is the place to initialize these vars
  }

  nrNodes++;
}


inline void chainList::addFirst(chainData *p2) {
  if(first) {             // list has members
    first->prev= p2;
    p2->next= first;
    p2->prev= nullptr;   /// do not depend on the constructor! it wont be called in the derived class! ...nice source of errors
    first= p2;
  } else {                // list is empty
    first= last= p2;
    p2->next= nullptr;   /// do not depend on the constructor! it wont be called in the derived class! ...nice source of errors
    p2->prev= nullptr;   /// this is the place to initialize these vars
  }

  nrNodes++;
}


inline void chainList::addAfter(chainData *p, chainData *afterPoint) {
  #ifdef CHAINLIST_SAFECHECKS
  if((!p) || (!afterPoint)) return;
  #endif

  if(afterPoint->next) afterPoint->next->prev= p;
  else last= p;
  p->next= afterPoint->next;
  afterPoint->next= p;
  p->prev= afterPoint;
  nrNodes++;
}


inline void chainList::addBefore(chainData *p, chainData *beforePoint) {
  #ifdef CHAINLIST_SAFECHECKS
  if((!p) || (!beforePoint)) return;
  #endif

  if(beforePoint->prev) beforePoint->prev->next= p;
  else first= p;
  p->prev= beforePoint->prev;
  beforePoint->prev= p;
  p->next= beforePoint;
  nrNodes++;
}


inline void chainList::addi(chainData *p, uint32_t n) {
  #ifdef CHAINLIST_SAFECHECKS
  if((n> nrNodes) || (!p)) return;
  #endif

  /// find the node that will be inserted before
  chainData *t= first;
  for(uint32_t a= n; a> 0; a--)       // <<< SLOW PART >>>
    t= t->next;

  if(!t) add(p);
  else addBefore(p, t);
}


// fast - few instructions - NO SEARCHES / PASSTHRUs
inline void chainList::del(chainData *p) {
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
  CHAINLIST_DEALLOC(p);
  nrNodes--;
}

// slow - goes thru the list, with many instructions per cicle
inline void chainList::deli(uint32_t nr) {
  #ifdef CHAINLIST_SAFECHECKS
  if(nr> nrNodes) return;
  #endif
  
  chainData *p= get(nr);
  if(p) del(p);
}


// [FAST] releases object from the chainList, doesn't delete the object from memory - NO searches involved
inline void chainList::release(chainData *p) {
  #ifdef CHAINLIST_SAFECHECKS
  if((!nrNodes) || (!p))
    return;
  #endif

  /// make the links next/prev
  if(p->prev) p->prev->next= p->next;
  if(p->next) p->next->prev= p->prev;
  if(p== first) first= p->next;
  if(p== last) last= p->prev;

  /// p belongs to no list from now on
  p->next= nullptr;
  p->prev= nullptr;

  /// update nr nodes
  nrNodes--;
}


// [SLOW] releases object from the chainList, doesn't delete the object from memory - NO searches involved
inline void chainList::releasei(uint32_t nr) {
  chainData *p= get(nr);
  release(p);
}

///---------------------------------------------------------------///
// GET - SEARCH list funcs - not to be used regulary or in a cycle //
///---------------------------------------------------------------///

// get must be used rarely: if there's a for() {get()} it will pass n*n times thru list. if the list is 100000 items long... do the math... MANY zeroes of instructions...
inline chainData *chainList::get(uint32_t nr) {
  #ifdef CHAINLIST_SAFECHECKS
  if(!nrNodes) return null;
  if(nr> nrNodes) return null;
  #endif
  
  chainData *p= first;
  for(uint32_t a= 0; a< nr; a++)                   // <<< SLOW PART >>>
    p= p->next;

  return p;
}

// same as get, use RARELY
inline uint32_t chainList::search(chainData *e) {
  chainData *p= first;
  for(uint32_t a= 0; a< nrNodes; a++, p= p->next)  // <<< SLOW PART >>>
    if(p== e) return a;

  return ~0u;
}


inline bool chainList::isMember(chainData *in_p) {
  for(chainData *c= first; c; c= c->next)
    if(c== in_p)
      return true;
  return false;
}



#endif /// ifndef CHAINLIST_HPP

