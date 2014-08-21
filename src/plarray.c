#include "plarray.h"
#include <string.h>



#define PLARRAYDEFINE( type, name )\
\
void pushbackpl##name##array ( pl##name##array *thisone, type val )\
{\
	size_t startsize=thisone->n;\
	thisone->resize( thisone, startsize + 1 );\
	if( thisone->n == startsize + 1)\
		thisone->mem[ thisone->n - 1 ] = val;\
}\
\
type popbackpl##name##array ( pl##name##array *thisone )\
{\
	type result = thisone->mem[thisone->n - 1];\
	thisone->resize( thisone, thisone->n - 1 );\
	return result;\
}\
\
void resizepl##name##array( pl##name##array *thisone, size_t n )\
{\
	if( n > thisone->allocated )\
	{\
		type *mem;\
		size_t newsize = thisone->allocated*2;\
		if ( newsize < n )\
			newsize = n;\
		mem=(type*)malloc( newsize * sizeof( type ) );\
		if( !mem )\
			return;\
		if( thisone->mem )\
		{\
			memcpy( mem, thisone->mem, thisone->n * sizeof( type ) );\
			free( thisone->mem );\
		}\
		thisone->mem = mem;\
	}\
	thisone->n = n;\
}\
\
void destroypl##name##array ( pl##name##array *thisone )\
{\
	if( thisone->mem )\
		free( thisone->mem );\
	thisone->n = 0;\
	thisone->allocated = 0;\
}\
\
void shrinktofitpl##name##array( pl##name##array *thisone )\
{\
	if( thisone->mem && thisone->n != thisone->allocated )\
	{\
		type *mem=(type *)malloc( thisone->n * sizeof( type ) );\
		if( !mem )\
			return;\
			\
		if( thisone->mem )\
		{\
			memcpy( mem, thisone->mem, thisone->n * sizeof( type ) );\
			free( thisone->mem );\
		}\
		thisone->mem = mem;\
	}\
}\
\
void copyfrompl##name##array( struct pl##name##array *thisone, struct pl##name##array source )\
{\
	thisone->copyfrommem( thisone, source.mem, source.n );\
}\
\
void copyfrom##name##mem( struct pl##name##array *thisone, const type *source, size_t n )\
{\
	thisone->resize( thisone, n );\
	if(thisone->n != n)\
		return;\
	if( source )\
	{\
		memcpy( thisone->mem, source, n * sizeof( type ) );\
	}\
}\
\
void append##name##mem( struct pl##name##array *thisone, const type *source, size_t n )\
{\
	size_t orign = thisone->n;\
	thisone->resize( thisone, orign + n );\
	if( thisone->n != orign + n )\
		return;\
	memcpy( thisone->mem + orign, source, n );\
}\
\
void constructpl##name##array( pl##name##array *thisone, size_t n )\
{\
	thisone->n = 0;\
	thisone->allocated = 0;\
	thisone->mem = NULL;\
	thisone->pushback = &pushbackpl##name##array;\
	thisone->popback = &popbackpl##name##array;\
	thisone->resize = &resizepl##name##array;\
	thisone->destroy = &destroypl##name##array;\
	thisone->shrinktofit = &shrinktofitpl##name##array;\
	thisone->copyfromarray = &copyfrompl##name##array;\
	thisone->copyfrommem = &copyfrom##name##mem;\
	thisone->appendmem = &append##name##mem;\
	\
	thisone->resize( thisone, n );\
}

PLARRAYDEFINE( PLINT, int )
PLARRAYDEFINE( PLFLT, flt )
PLARRAYDEFINE( char, char )
PLARRAYDEFINE( unsigned char, uchar )
PLARRAYDEFINE( PLColor, color )
