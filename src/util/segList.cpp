#ifdef _WIN32
#define OS_WIN
#include <Windows.h>
#endif /// OS_WIN

#ifdef __linux__
#define OS_LINUX
#include <time.h>
#endif /// OS_LINUX

#ifdef __APPLE__
#define OS_MAC
#include <mach/mach.h>                  // high resolution clock funcs
#include <mach/mach_time.h>             // high resolution clock funcs
#endif /// OS_MAC

#include <mutex>
#include "util/typeShortcuts.h"
#include "util/chainList.h"
#include "util/segList.h"


// !!!!!!!!!!!
// there shouldn't be many segments, else there are too many allocs/ loops get bigger = adios speed.
// TRY AND MAKE THE SEGMENT SIZE AS OPTIMAL AS POSSIBLE WITH THAT IN MIND

//#define CHAINLIST_SAFECHECKS 1 //to check for bad calls to chainList



///--------------///
// _segment class //
///--------------///

_segment::_segment() {
  next= prev= null;
  data= null;
  freeSpc= null;
  freeSpcPeak= 0;
  timeIdle= 0;					/// 0= segment in use
}


_segment::~_segment() {
  if(data)
    delete[] (int8 *)data;
  if(freeSpc)
    delete[] freeSpc;
}



///-------------///
// segList class //
///-------------///

// hidden chainList with all created segLists; used for actions on all lists




class _ALLsegsData:public chainData {
public:
  segList *list;
};

std::mutex _ALLsegsMutex;
chainList _ALLsegs;



segList::segList(int segmentSize, int uSize, bool ignoreLocking, int idleTime) {
  unitSize= uSize;
  segSize= segmentSize;
  timeMaxIdle= idleTime;

  first= last= null;
  nrNodes= 0;
  
  addSegment();         // create the first memory segment - there is 1 at start; delData removes all segments, but if new members are added, a new segment is reallocated

  /// add this segList to the chainList with all segLists
  _ALLsegsData *p= new _ALLsegsData;
  p->list= this;
  
  if(ignoreLocking)
    _ALLsegs.add(p);
  else {
    _ALLsegsMutex.lock();
    _ALLsegs.add(p);
    _ALLsegsMutex.unlock();
  }
}


segList::~segList() {
  if(_ALLsegs.first) {
    _ALLsegsMutex.lock();
    /// remove this list from the chainList with all created segLists
    _ALLsegsData *p= (_ALLsegsData *)_ALLsegs.first;
    while(p)
      if(p->list== this)
        break;
      else
        p= (_ALLsegsData *)p->next;
  
    if(p) _ALLsegs.del(p);
    _ALLsegsMutex.unlock();
  }
  delData();
}


void segList::delData() {
  /// delete list members
  while(first)
    del(first);

  /// dealloc memory
  while(seg.nrNodes)
    //removeSegment();
    seg.del(seg.last);
}



///---------------------------///
// MEMORY ALLOC/ DEALLOC funcs //
///---------------------------///

void segList::addSegment() {
  _segment *p= new _segment;
  p->data= new int8[unitSize* segSize];
  p->freeSpc= new void*[segSize];

  /// set free space to point to the data allocated
  for(int a= 0; a< segSize; a++)
     p->freeSpc[a]= (int8*)p->data+ a* unitSize;

  p->freeSpcPeak= segSize;
  p->timeIdle= 0;
  seg.add((chainData*)p);
}


//void segList::removeSegment() {
//  seg.del(seg.last);
//}



///--------------///
// ADD NODE funcs //
///--------------///

segData *segList::add() {
  segData *ret= null;
  _segment *s= (_segment*)seg.first;

  /// find a free segment unit(segData) space
  while(s) {
    if(s->freeSpcPeak) break;            // found
    s= (_segment *)s->next;
  }

  /// not found? alloc mem for another segment
  if(!s) {
    addSegment();
    s= (_segment*)seg.last;
  }

  ret= (segData *)s->freeSpc[s->freeSpcPeak- 1];    /// 'alloc' mem
  ret->seg= s;                            /// ret's segment
  s->freeSpcPeak--;                       /// decrease peak; when 0, segment is full
  s->timeIdle= 0;                         /// set timeIdle to 0, signifying segment is used

  /// do the next/prev link
  if(last) {
    last->next= ret;
    ret->prev= last;
    ret->next= null;
    last= ret;
  } else {
    first= last= ret;
    ret->next= ret->prev= null; /// keep this init here, if a constructor is used, it wont be called by derived class
  }

  nrNodes++;
  return ret;
}



segData *segList::addFirst() {
  segData *ret= null;
  _segment *s= (_segment*)seg.first;

  /// find a free segment unit(segData) space
  while(s) {
    if(s->freeSpcPeak) break;               // found
    s= (_segment *)s->next;
  }

  /// not found? alloc mem for another segment
  if(!s) {
    addSegment();
    s= (_segment*)seg.last;
  }

  ret= (segData *)s->freeSpc[s->freeSpcPeak- 1];    /// 'alloc' mem
  ret->seg= s;                            /// ret's segment
  s->freeSpcPeak--;                       /// decrease peak; when 0, segment is full
  s->timeIdle= 0;                         /// set timeIdle to 0, signifying segment is used

  /// do the next/prev linking
  if(first) {
    first->prev= ret;
    ret->next= first;
    ret->prev= null;
    first= ret;
  } else {
    first= last= ret;
    ret->next= ret->prev= null;
  }

  nrNodes++;
  return ret;
}



///--------------///
// DEL NODE FUNCS //
///--------------///

// fast function
void segList::del(segData *p) {	
  #ifdef CHAINLIST_SAFECHECKS
  if((!nrNodes) || (!p))
    return;
  #endif

  /// set the next / prev link
  if(p->prev) p->prev->next= p->next;
  if(p->next) p->next->prev= p->prev;
  if(p== first) first= p->next;
  if(p== last) last= p->prev;

  p->seg->freeSpc[p->seg->freeSpcPeak]= p;  /// mark memory as free
  p->seg->freeSpcPeak++;                    /// increase the peak (if(peak == segmentSize) the whole segment is free)

  nrNodes--;
}


/// SLOW - goes thru the list, with many instructions per cicle (in a small list is ok)
void segList::deli(int nr) {
  #ifdef CHAINLIST_SAFECHECKS
  if(!nrNodes) return;
  if(nr> nrNodes) return;
  #endif

  /// find the member
  segData *p= first;
  for(int a= 0; a< nr; a++)      // <<< THIS PART COULD BE SLOW IN A BIG LIST >>>
    p= p->next;

  #ifdef CHAINLIST_SAFECHECKS
  //if(!p) return;                /// failsafe -NOPE if(nr> nrNodes) does the same
  #endif

  /// set the next-prev link
  if(p->prev) p->prev->next= p->next;
  if(p->next) p->next->prev= p->prev;
  if(p== first) first= p->next;
  if(p== last) last= p->prev;

  p->seg->freeSpc[p->seg->freeSpcPeak]= p;  /// mark memory as free
  p->seg->freeSpcPeak++;                    /// increase the peak (if(peak == segmentSize) the whole segment is free)

  nrNodes--;
}




///---------------------------------------------------------------///
// GET - SEARCH list funcs - not to be used regulary or in a cycle //  
///---------------------------------------------------------------///


/// get must be RARELY used: if one makes a for(a) { get(b)} it gets thru a*b times thru the list. if the list is 100000units.... do the math
segData *segList::get(int nr) {
  #ifdef CHAINLIST_SAFECHECKS
  if(!nrNodes) return null;
  if(nr> nrNodes) return null;
  #endif

  segData *p= first;
  for(int a= 0; a< nr; a++)
    p= p->next;

  return p;
}


/// same as the get func, should be RARELY used
int segList::search(segData *e) {
  segData *p= first;
  for(int a= 0; a< nrNodes; a++, p= p->next)
    if(p== e) return a;

  return -1;
}


// call this func from time to time to clear idle allocated segments... from time to time as in very rarely... 1 time per minute? (maybe rarer)
void segList::checkIdle() {
  if(seg.nrNodes == 1)    // it wont dealloc the first segment
    return;
  
  /// getting the present time
  uint present;
  
  #ifdef OS_WIN
  present= GetTickCount();
  #endif /// OS_WIN

  #ifdef OS_LINUX
  timespec out;
  clock_gettime(CLOCK_MONOTONIC, &out);
  present= (out.tv_sec* 1000)+ (out.tv_nsec/ 1000000);
  #endif /// OS_LINUX

  #ifdef OS_MAC
  uint64_t time;
  static mach_timebase_info_data_t machInfo;
  if(machInfo.denom== 0)
    mach_timebase_info(&machInfo);

  time= (mach_absolute_time()* machInfo.numer/ machInfo.denom)/ 1000000;
  present= (uint)time;
  #endif /// OS_MAC
  
  _segment *p= null, *t;

  if(seg.first)                     /// ignore the first segment (if there is one)
    p= (_segment *)seg.first->next;

  while(p)
    if(p->freeSpcPeak == segSize) {                    // if all space is free
      if(p->timeIdle) {                               /// check if the idle timer was started
        if((present- p->timeIdle)> timeMaxIdle) {     /// check difference between present-idle start time
          t= p;
          p= (_segment *)p->next;
          seg.del(t);
        }
      } else {                                        /// if all space is free, start the idle timer
        p->timeIdle= present;
        p= (_segment*)p->next;		
      }
    }	else {                                           // else the segment is in use / check next
      p->timeIdle= 0;                                 /// assure the idle timer is not used
      p= (_segment*)p->next;
    }
}


/// deletes all segments that are empty;
void segList::delEmptySegments() {
  _segment *p= null, *t;
  if(seg.first)                     /// ignores the first segment (if there is one)
    p= (_segment*)seg.first->next;

  while(p)
    if(p->freeSpcPeak == segSize) { /// if the segment is empty
      t= p;
      p= (_segment *)p->next;
      seg.del(t);                    // delete it
    } else
      p= (_segment *)p->next;
}



///--------------------------------------------///
// funcs that will work on ALL CREATED segLists //
///--------------------------------------------///

// NOT THREAD SAFE - each checkIdle will use/del segments that could be accessed from different threads
// checks ALL created segLists for idle segments; can be VERY SLOW if many lists are created; use rarely!!!!
void checkIdleALL() {
  _ALLsegsMutex.lock(); // with the mutex, this is still not thread safe
  _ALLsegsData *p= (_ALLsegsData *)_ALLsegs.first;
  while(p) {
    p->list->checkIdle();
    p= (_ALLsegsData *)p->next;
  }

  _ALLsegsMutex.unlock(); // with the mutex, this is still not thread safe
}

// NOT THREAD SAFE - each checkIdle will use/del segments that could be accessed from different threads
// same as delEmptySegments() but for ALL segLists that were created; basically a garbage collector
void delEmptySegmentsALL() {
  _ALLsegsMutex.lock(); // with the mutex, this is still not thread safe

  _ALLsegsData *p= (_ALLsegsData *)_ALLsegs.first;
  while(p) {
    p->list->delEmptySegments();
    p= (_ALLsegsData *)p->next;
  }

  _ALLsegsMutex.unlock();
}









