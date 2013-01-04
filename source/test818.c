/**
 * Trivial functions for testing how EUPHORIA's EUPHORIA to C bridge c_func and c_proc 
 * convert the values from EUPHORIA to C.
 * 
 * Note: Attemps to use bool and C_BOOL in any of these macros result in an error.  There is 
 * something special about one of these identifiers...  */ 

#include "euphoria.h"
/* smallest EUPHORIA integer */
#define EUPHORIA_MIN_INT MININT
#define EUPHORIA_MAX_INT MAXINT
#include <string.h>

#ifdef __WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#define C000_INT_VALUE 0xc0000000
#define C000_LONG_VALUE (0xC * (1L << (8*sizeof(long)-8)))
#define C000_SHORT_VALUE (0xC * (1 << (8*sizeof(short)-8)))
#define C000_LONGLONG_VALUE (0xC * (1LL << (8*sizeof(long long)-8)))
#define C000_FLOAT_VALUE (float)(0xC * (1 << (8*sizeof(float)-8)))
#define C000_DOUBLE_VALUE (double)(0xC * (1 << (8*sizeof(double)-8)))
typedef signed char Byte;
typedef unsigned char UByte;
typedef enum {false,true} Bool;

/* The expression falls within the signed int range but outside of that of EUPHORIA */
#define MAKE_BORDER_FUNCTIONS(ctype,etype) \
	EXPORT ctype etype ## _below_EUPHORIA_MIN_INT() \
	{\
		return EUPHORIA_MIN_INT - 20;\
	}\
	EXPORT ctype etype ## _above_EUPHORIA_MAX_INT() \
	{\
		return EUPHORIA_MAX_INT + 20;\
	}\
	EXPORT ctype etype ## _NOVALUE() \
	{\
		return NOVALUE; \
	}\
	EXPORT ctype etype ## _half_MIN() \
	{\
		return EUPHORIA_MIN_INT/2;\
	}\
	EXPORT ctype etype ## _half_MAX() \
	{\
		return EUPHORIA_MAX_INT/2; \
	}

#if INT32_MAX == INTPTR_MAX
MAKE_BORDER_FUNCTIONS(int,C_INT)
#endif

#if INT32_MAX == INTPTR_MAX || !defined(EWINDOWS)
MAKE_BORDER_FUNCTIONS(long,C_LONG)
#endif
MAKE_BORDER_FUNCTIONS(long long,C_LONGLONG)

#define MAKE_ID_FUNCTION(ctype,etype) \
	EXPORT ctype etype ## _id(ctype val) {\
		return val;\
	}
MAKE_ID_FUNCTION(Bool, C_BOOL)
MAKE_ID_FUNCTION(signed char, C_CHAR)
MAKE_ID_FUNCTION(Byte, C_BYTE)
MAKE_ID_FUNCTION(UByte, C_UBYTE)
MAKE_ID_FUNCTION(unsigned char, C_UCHAR)
MAKE_ID_FUNCTION(short, C_SHORT)
MAKE_ID_FUNCTION(unsigned short, C_USHORT)
MAKE_ID_FUNCTION(int, C_INT)
MAKE_ID_FUNCTION(unsigned int, C_UINT)
MAKE_ID_FUNCTION(void*, C_POINTER)
MAKE_ID_FUNCTION(long, C_LONG)
MAKE_ID_FUNCTION(unsigned long, C_ULONG)
MAKE_ID_FUNCTION(long long, C_LONGLONG)

MAKE_ID_FUNCTION(float, C_FLOAT)
MAKE_ID_FUNCTION(double, C_DOUBLE)

	
#define MAKE_GET_VAL_FN(ctype,etype,prefix,testvalue) \
	ctype etype ## prefix ## _value = testvalue; \
	EXPORT ctype etype ## prefix() { return etype ## prefix ## _value; }
	
MAKE_GET_VAL_FN(char,      C_CHAR,     _BFF_FD, 0xC0 - 20)
MAKE_GET_VAL_FN(short,     C_SHORT,    _BFF_FD, C000_SHORT_VALUE - 20)
MAKE_GET_VAL_FN(int,       C_INT,      _BFF_FD, C000_INT_VALUE - 20)
MAKE_GET_VAL_FN(long,      C_LONG,     _BFF_FD, C000_LONG_VALUE - 20)
MAKE_GET_VAL_FN(long long, C_LONGLONG, _BFF_FD, C000_LONGLONG_VALUE - 20)

MAKE_GET_VAL_FN(char,      C_CHAR,     _M20, -20)
MAKE_GET_VAL_FN(short,     C_SHORT,    _M20, -20)
MAKE_GET_VAL_FN(int,       C_INT,      _M20, -20)
MAKE_GET_VAL_FN(long,      C_LONG,     _M20, -20)
MAKE_GET_VAL_FN(long long, C_LONGLONG, _M20, -20)

MAKE_GET_VAL_FN(char,      C_CHAR,     _M100, -100)
MAKE_GET_VAL_FN(short,     C_SHORT,    _M100, -10000)
MAKE_GET_VAL_FN(int,       C_INT,      _M100, -1000000000)
MAKE_GET_VAL_FN(long,      C_LONG,     _M100, ((sizeof(long) == sizeof(long long)) ? -1000000000000000000LL : -1000000000L) )
MAKE_GET_VAL_FN(long long, C_LONGLONG, _M100, -1000000000000000000)

EXPORT unsigned long long bit_repeat(Bool bit, unsigned char count) {
	long long bit_vector = 0LL;
	// make sure bit is boolean.
	bit = (bit != false);
	while (count--) {
		bit_vector <<= 1;
		bit_vector |= bit;
	}
	return bit_vector;
}

double Dpow(double d, unsigned short n) {
	double base = 1.0;
	while (n--) base *= d;
	return base;
}


EXPORT double powsum(double d1, unsigned short n1, 
	double d2, unsigned short n2,
	double d3, unsigned short n3,
	double d4, unsigned short n4,
	double d5, unsigned short n5) {
	return Dpow(d1,n1)+Dpow(d2,n2)+Dpow(d3,n3)+Dpow(d4,n4)+Dpow(d5,n5);
}

EXPORT object object_func( object foo ){
	return foo;
}
