#pragma once

//#define CHAINLIST_SAFECHECKS 1 //to check for bad calls to chainList

// DERIVE classes from this one & DEFINE what vars are in the node inside it
class chainData {
public:
  chainData *next, *prev;

  /// don't create a constructor, it won't be called by derived class... big source of errors
  virtual ~chainData();       /// being virtual, it seems delete[] knows to dealloc the derived parts... it never crashed/ no garbage remains in memory
};

/// manipulator class, no derives necesary, just make a var from this one
class chainList {
public:
  chainData *first, *last;     // last is important for add(), else there has to be a passthru till the last member (if the list is 100k long?)
  int nrNodes;                 // VERY USEFULL - number of nodes in list

  // fast funcs

  void add(chainData *);       // [FAST] alloc mem, then pass pointer to add(). fast function, no searches involved
  void addFirst(chainData *);  // [FAST] adds the node to the front of the list not the back (makes it first)
  void del(chainData *);       // [FAST] deletes specified chainList - NO searches involved
  
  // slow funcs

  void deli(int);             /// [SLOW] dels specified item number - searches involved - SLOW for large lists 
  chainData *get(int);        /// [SLOW] returns specified item number - searches involved - SLOW for large lists
  int search(chainData *);    /// [SLOW] returns -1 if failed to find - SLOW for large lists

  // constructor / destructor

  chainList();
  ~chainList();
  void delData();              // teh real destructor (tm) - can be called at any time to dealloc everything
};








