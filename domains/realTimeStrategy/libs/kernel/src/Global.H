/*
  Global definitions
  
  Licensed under GPL 3
  (c) Michael Buro, mburo@cs.ualberta.ca
*/


#ifndef Global_H
#define Global_H

#include "All.H"
#include "Vector.H"

#include <cstdio>
#include <cstdarg>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cassert>
#include <cmath>
#include <string>
#include <set>
#include <map>
#include "Vector.H"
#include <boost/format.hpp>
#include <boost/array.hpp>
#include <boost/algorithm/string.hpp> // split and friends
#include <ext/hash_set>
#include <ext/hash_map>

typedef   signed char        sint1;
typedef unsigned char        uint1;
typedef   signed short       sint2;
typedef unsigned short       uint2;
typedef   signed int         sint4;
typedef unsigned int         uint4;
typedef               float  real4;
typedef               double real8;
typedef          long double realC;
typedef   signed long long   sint8;
typedef unsigned long long   uint8;

typedef void* vptr;
typedef char* cptr;

typedef const void* cvptr;
typedef const char* ccptr;

typedef const void* const cvptrc;
typedef const char* const ccptrc;

const sint1 min_sint1 = 0x80;
const sint1 max_sint1 = 0x7F;
const uint1 min_uint1 = 0x00;
const uint1 max_uint1 = 0xFF;
const sint2 min_sint2 = 0x8000;
const sint2 max_sint2 = 0x7FFF;
const uint2 min_uint2 = 0x0000;
const uint2 max_uint2 = 0xFFFF;
const sint4 min_sint4 = 0x80000000;
const sint4 max_sint4 = 0x7FFFFFFF;
const uint4 min_uint4 = 0x00000000;
const uint4 max_uint4 = 0xFFFFFFFF;
const sint8 min_sint8 = 0x8000000000000000LL;
const sint8 max_sint8 = 0x7FFFFFFFFFFFFFFFLL;
const uint8 min_uint8 = 0x0000000000000000LL;
const uint8 max_uint8 = 0xFFFFFFFFFFFFFFFFLL;

using boost::array;

template <class T, int N1, int N2> class array2 :
  public boost::array<boost::array<T, N2>, N1> {};

template <class T, int N1, int N2, int N3> class array3 :
  public boost::array<boost::array<boost::array<T, N3>, N2>, N1> {};

template <class T, int N1, int N2, int N3, int N4> class array4 :
  public boost::array<boost::array<boost::array<boost::array<T, N4>, N3>, N2>, N1> {};

#define FOREVER    for (;;)
#define FOR(i, n)  for (i = 0; i < (n); ++i)
#define FORS(i, n)  for (sint4 i = 0; i < (n); ++i)
#define FORU(i, n)  for (uint4 i = 0; i < (n); ++i)

#if defined(__GNUC__)
#define FORT(i, n)  for (typeof(n) i = 0; i < (n); ++i)
#else
#define FORT(i, n)  for (int i = 0; i < (n); ++i)
#endif

#define FIND(cont, it, elem) typeof(cont.find(elem)) it = cont.find(elem);

#define FORALL(CONT,i)  \
  for (typeof((CONT).begin()) i = (CONT).begin(), _ = (CONT).end(); i != _; ++i) 

// CAUTION: using a lowercase for #define causes problems when using 
// other libraries.. 
#define ERRSTR std::cerr
#define REMSTR std::cout
#define ABORT  throw "ABORT"

#if defined(__GNUC__)

#define ERR(s) {\
    ERRSTR << std::endl << __FILE__ << " " << __FUNCTION__ << "() (line " << __LINE__ << "): " \
	   << s << std::endl; ABORT;					\
}

#define ERR2(s1, s2) {\
    ERRSTR << std::endl << __FILE__ << " " << __FUNCTION__ << "() (line " << __LINE__ << "): " \
	   << s1 << " " << s2 << std::endl; ABORT;			\
}

#else

#define ERR(s) {\
  ERRSTR << endl << __FILE__  << " (line " << __LINE__ << "): "\
         << s << endl; ABORT;\
}

#define ERR2(s1, s2) {\
  ERRSTR << endl << __FILE__  << " (line " << __LINE__ << "): "\
         << s1 << " " << s2 << endl; ABORT;\
}
#endif

#define REM(x) REMSTR << "REMARK: " << x << std::endl;
#define REM2(x,y) REMSTR << "REMARK: " << (x) << " " << (y) << std::endl;
#define REM3(x,y,z) REMSTR << "REMARK: " << (x) << " " << (y) << " " << (z) << std::endl;
  
// bool debug is defined as static, per file for easy shutting on/off dbg msgs
#define DPR                if (debug) std::cout
#define DLPR(l)            if ((l) <= debug_level) std::cout

#define SQR(x)             ((x)*(x))
#define ABS(x)             ((x) >= 0 ? (x) : (-1*(x)))
#define DELETE(x)          if ((x) != NULL) { delete (x); (x) = NULL; }
#define DELETEARR(x)       if ((x) != NULL) { delete [] (x); (x) = NULL; }
#define PDIST(p1,p2)       sqrt(SQR((p1).first-(p2).first) + SQR((p1).second-(p2).second))
#define DIST(x1,x2,y1,y2)  sqrt(SQR((x1)-(y1)) + SQR((x2)-(y2)))
#define MIN(x,y)           ((x) < (y) ? (x) : (y))
#define MAX(x,y)           ((x) > (y) ? (x) : (y))

struct StringHash 
{
  size_t operator()( const std::string& x ) const
  {
    return __gnu_cxx::hash< const char* >()( x.c_str() );
  }
};

inline sint8 mult(const sint8 x, const sint8 y)
{
  assert(fabs(real8(x)*real8(y)) < 1e18);
  return x*y;
}

inline uint4 square(const uint4& x) {
  assert(x <= 65535);
  return x*x;
}

inline sint4 square(const sint4& x) {
  assert(abs(x) <= 46340);
  return x*x;
}

inline sint8 square(const sint8& x) {
  #ifdef GCC
  assert(::llabs(x) <= 3037000499LL);
  #endif
  return x*x;
}

inline real4 square(const real4& x) {
  assert(fabs(x) <= 1.84391e19);
  return x*x;
}

inline real8 square(const real8& x) {
  assert(fabs(x) <= 1.0e150L);
  return x*x;
}

template<class T> T sign(T &x) {
  if (x > 0) return +1;
  if (x < 0) return -1;
  return 0;
}

int to_int(const std::string & str);
double to_double(const std::string & str);
float to_float(const std::string & str);
bool to_bool(const std::string & str);
long to_long(const std::string & str);
std::string to_string(int x); 
std::string to_string(long x); 
std::string to_string(double x); 
std::string to_string(std::pair<int,int> p); 

std::string trim(const std::string & str);

std::string join(const std::vector<std::string> & pieces, 
                 std::string delimiter, 
                 int start, 
                 int end); 

std::string join(const std::vector<std::string> & pieces, std::string delimiter); 

std::string join(const std::vector<std::string> & pieces, 
                 std::string delimiter, 
                 int start); 

void splitup(std::vector<std::string> & vec, const std::string & str, const std::string & delimiter);

double distance(int x1, int y1, int x2, int y2);


template<class T>                         
bool setwise_equal(T & v1, T & v2)
{
  bool ret = true;
  
  if (v1.size() != v2.size())
    return false; 
  
  if (v1 == v2)  // this is always stronger
    return true; 
  
  int size = v1.size(); 
  
  bool * v2flags = new bool[size];
  
  FORS(i, size) 
    v2flags[i] = false; 
  
  FORS(i, size)
  {    
    bool match = false; 
    
    FORS(j, size)
    {
      if (!v2flags[j] && v1[i] == v2[j])
      {
        // match!
        v2flags[j] = true; 
        match = true; 
        break; 
      }
    }
    
    if (!match)
    {
      ret = false; 
      break; 
    }
  }  
  
  DELETEARR(v2flags); 
  
  return ret;
}



#endif
