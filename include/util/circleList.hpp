#pragma once

// !!! IMPORTANT !!! 
// the derived classes's constructors, must ensure the base class constructor is called
// OR, in the constructors place  " prev= next= this; "
// OR, make sure somehow next and prev are pointing to itself

// DERIVE classes from this & DEFINE what vars are in the node inside it
class circleList {
public:
  circleList *next, *prev;

  void add(circleList *in_node);        // inserts in_node in the list, after this node
  void addBefore(circleList *in_node);  // inserts in_node in the list, before this node
  
  void del();       // deletes this node, don't use this node for anything after calling this
  static void del(circleList *in_l);
  bool delNext();   // deletes next node, if there is one; returns false if there's no other node but this in the list
  bool delPrev();   // deletes previous node, if there is one; returns false if there's no other node but this in the list
  //static void del(circleList *in_node); // releases the node, then deletes from mem
  void delAll();    // deletes all nodes - deletes list

  void release();   // takes this node out of the loop

  circleList(): next(this), prev(this) {} // !!! IMPORTANT !!! derived class, use derivedList(): circleList(), etcVar(bla), etcVar(bla) {}
  virtual ~circleList() { del(); }        // being virtual, it seems delete[] knows to dealloc the derived parts... it never crashed/ no garbage remains in memory
};












///================///
// funcs definition //
///================///

// inserts in_node in the list, after this node
inline void circleList::add(circleList *in_l) {
  circleList *p= next;
  next= in_l;
  in_l->prev= this;
  in_l->next= p;
  p->prev= in_l;
}

// inserts in_node in the list, before this node
inline void circleList::addBefore(circleList *in_l) {
  circleList *p= prev;
  prev= in_l;
  in_l->next= this;
  in_l->prev= p;
  p->next= in_l;
}

// deletes next node, if there is one; returns false if there's no other node but this in the list
inline bool circleList::delNext() {
  if(next== this) return false;

  circleList *p= next->next;
  delete next;
  next= p;
  p->prev= this;

  return true;
}

// deletes previous node, if there is one; returns false if there's no other node but this in the list
inline bool circleList::delPrev() {
  if(prev== this) return false;
  circleList *p= prev->prev;
  delete prev;
  prev= p;
  p->next= this;

  return true;
}

// deletes this node, don't use this node for anything after calling this
inline void circleList::del() {
  release();
  delete this;
}

// releases the node, then deletes from mem
inline void circleList::del(circleList *in_l) {
  in_l->release();
  delete in_l;
}

// takes this node out of the loop
inline void circleList::release() {
  if(next== this) return;
  next->prev= prev;
  prev->next= next;
  next= prev= this;
}



inline void circleList::delAll() {
  while(delNext());
  del();
}





