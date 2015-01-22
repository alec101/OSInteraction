#pragma once
//#define CHAINLIST_SAFECHECKS 1 //to check for bad calls to chainList

// ADVANTAGES:    -few to no memory allocs/deallocs (should be a huge boost in speed)
// DISADVANTAGES: -additional memory space for each unit - [2* sizeof(void*)] per unit - not a big hit 
//                  (32bytes total memory for one node: *prev, *next, *seglistSpecial, *segment - on a 64bit system)


// SEGLIST CREATION: segList( 'segment size', sizeof('segData derived class'));

/// Memory alloc is automatic, everything you need to think about is the segment size (number of chainList nodes that are allocated per segment);
///   it must be right, so not too many allocs happen

/// You can call checkIdle() from time to time, to auto deallocate memory of idle segments
/// Set timeMaxIdle (milisecs) to a desired idle time (default is 5 mins) after wich a segment is deallocated

// FUNCTIONS THAT WILL WORK ON ALL CREATED segList's:
/// call segList::checkIdleALL() to check ALL CREATED LISTS for idle segments that can be deallocated
/// call segList::delEmptySegmentsALL() to force dealloc ALL CREATED LISTS unused segments



// TODO:  i think there is a posibility to auto-know the derived class size, so the constructor will only need to know a segment size ( and not a unit size )


class segList;
class _segment;


// DERIVE classes from this one & DEFINE what vars are in the chain (node/blabla)
class segData {
  friend class segList;
  _segment *seg;
public:
  segData *next, *prev;

  // there are no virtual constructors / base constructor will not be called by derived class...
  // this is big source of errors, therefore, just do not create a constructor
  virtual ~segData() {};    /// being virtual, it seems delete[] knows to dealloc the derived parts... it never crashed/ no garbage remains in memory
};



// ------------------================= SEGLIST CLASS ====================------------------
///========================================================================================

// 'handler' class, just make a variable from this one
class segList {
  friend class segData;

  // memory alloc private vars, nothing to bother

  int unitSize;                 /// each segData (derived class) size
  int segSize;                  /// segment size
  chainList seg;                /// chainlist with all segments of memory allocated / memory alloc is automatic, nothing to bother
  inline void addSegment();     /// memory allocation
  inline void removeSegment();  /// memory deallocation

public:
  segData *first, *last;     // last is important for add(), else there has to be a loop that passes thru all the list (if the list has 100k nodes... goodbye speed)
  int nrNodes;               // VERY USEFULL - nr nodes in list
  
  // fast funcs

  segData *add();            // [FAST] returns pointer to the space allocated (it allocs mem only when no segment has free space)
  segData *addFirst();       // [FAST] adds the node to the front of the list not the back (makes it first)
  void del(segData *);       // [FAST] remove specified node - NO searches involved
  
  // slow funcs

  void deli(int);           /// [SLOW] removes specified node number - good to have but searches are involved
  segData *get(int);        /// [SLOW] returns node with specified number - searches involved
  int search(segData *);    /// [SLOW] returns number ID of the specified node or -1 if failed to find
  
  // idle time check func

  unsigned int timeMaxIdle; /// miliseconds: idle time after wich an empty segment is dealocated - default is 5 mins
  void checkIdle();          // [SLOW] call this func RARELY to check for idle memory segments that need to dealocate
  void delEmptySegments();   // [SLOW] forced delete of all segments that are not used

  // funcs that will work for _ALL_ created segments; (there is a hidden chainList with all created segments)

  static void checkIdleALL();       // [VERY SLOW] this is like checkIdle, but for ALL segments that were created; 
  static void delEmptySegmentsALL();// [VERY SLOW] same as delEmptySegments() but for ALL segLists that were created; basically a garbage collector

  // constructor / destructor

  segList(int segmentSize, int uSize, int idleTime= 300000);  // initialize by providing a segment size, and a sizeof(derived segData)
  ~segList();
  void delData();           /// teh real destructor (tm) - can be called at any time to dealloc everything (removes EVERYTHING tho)
};









// INTERNAL STUFF - nothing to bother here

/// allocation / deallocation segment list
class _segment: protected chainData {
friend class segList;
  void *data;       /// actual memory allocated
  
  void **freeSpc;   /// list with all free spaces in segment ( unitSize* segSize total space)
  int freeSpcPeak;  /// peak of freeSpc. acts like some kind of stack: u pick a free space from the top/ u put the space taken back

  unsigned int timeIdle;    /// if 0, segment is in use. Else it holds the time @ start idling (used internally by segList::checkIdle())

  _segment();
  ~_segment();
};




