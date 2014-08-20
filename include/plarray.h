#ifndef __PLARRAY_H__
#define __PLARRAY_H__

#include "plplot.h"

//Array structures for different types, these use function pointers to give some
//similarities to C++ classes. 
//Each function has the following properties
//
//void pushback( struct array *thisone, PLFLT val )
//add an element to the array by calling resize. If memory allocation fails then
//the element is not added.
//
//void resize ( struct plfltarray *thisone, size_t nelements )
//Allocates at least the memory needed to store nelements and assings the pointer
//to the new memory to mem, copying existing elements from mem and freeing existing
//memory if needed. n is set to nelements and allocated is set to the number of 
//elements available in the new mem pointer. This may be more than nelements to allow
//room for some expansion, avoiding excessive copying.
//If nelements is less than n, then n is simply changed, no memory allocation or 
//copying occurs.
//If memory allocation fails then the resize does not happen.
//
//void free ( struct plfltarray *thisone )
//frees mem and sets n and allocated to 0
//
//void shrinktofit ( struct plfltarray *thisone )
//This basically reduces the amount of memory used so that it just fits n elements.
//if n is equal to allocated nothing is done. Otherwise this will involve memory 
//allocation and copying. If the memory allocation fails then the array remains 
//unchanged.

#define PLARRAYDECLARE( type, name )\
\
typedef struct pl##name##array\
{\
	size_t n;\
	type *mem;\
	size_t allocated;\
	void (*pushback)( struct pl##name##array *thisone, type val );\
	void (*resize)( struct pl##name##array *thisone, size_t nelements );\
	void (*destroy)( struct pl##name##array *thisone );\
	void (*shrinktofit)( struct pl##name##array *thisone );\
	void (*copyfromarray)( struct pl##name##array *thisone, struct pl##name##array source );\
	void (*copyfrommem)( struct pl##name##array *thisone, const type *source, size_t n );\
} pl##name##array;\
\
void constructpl##name##array( pl##name##array *thisone, size_t n );

PLARRAYDECLARE( PLINT, int )
PLARRAYDECLARE( PLFLT, flt )
PLARRAYDECLARE( char, char )
PLARRAYDECLARE( unsigned char, uchar )
PLARRAYDECLARE( PLColor, color )

#endif