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
void resizepl##name##array( pl##name##array *thisone, size_t n )\
{\
	if( n > thisone->allocated )\
	{\
		size_t newsize = thisone->allocated*2;\
		if ( newsize < n )\
			newsize = n;\
		type *mem=(type*)malloc( newsize * sizeof( type ) );\
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
		type *mem=(type*)malloc( thisone->n * sizeof( type ) );\
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
void constructpl##name##array( pl##name##array *thisone, size_t n )\
{\
	thisone->n = 0;\
	thisone->allocated = 0;\
	thisone->mem = NULL;\
	thisone->pushback = &pushbackpl##name##array;\
	thisone->resize = &resizepl##name##array;\
	thisone->destroy = &destroypl##name##array;\
	thisone->shrinktofit = &shrinktofitpl##name##array;\
	thisone->copyfromarray = &copyfrompl##name##array;\
	thisone->copyfrommem = &copyfrom##name##mem;\
	\
	thisone->resize( thisone, n );\
}

PLARRAYDEFINE( PLINT, int )
PLARRAYDEFINE( PLFLT, flt )
PLARRAYDEFINE( char, char )
PLARRAYDEFINE( PLColor, color )

void constructplfltarray( plfltarray *thisone, size_t n )
{
	thisone->n = 0;
	thisone->allocated = 0;
	thisone->mem = NULL;
	thisone->pushback = &pushbackplfltarray;
	thisone->resize = &resizeplfltarray;
	thisone->destroy = &destroyplfltarray;
	thisone->shrinktofit = &shrinktofitplfltarray;
	thisone->copyfromarray = &copyfromplfltarray;
	thisone->copyfrommem = &copyfrommem;
	
	thisone->resize( thisone, n );
}
