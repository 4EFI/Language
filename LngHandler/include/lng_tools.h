#ifndef LNG_TOOLS_H
#define LNG_TOOLS_H

#include "config.h"

//-----------------------------------------------------------------------------

int GetOperationType( const char* str );

int GetIndexOperation( int numOp );

int GetType( const char* str, int* opType );

int GetIndexType( int numType );

//-----------------------------------------------------------------------------

#endif