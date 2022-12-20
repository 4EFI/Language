#ifndef VAR_HANDLER_H
#define VAR_HANDLER_H

#include "stdio.h"

//-----------------------------------------------------------------------------

const int MaxNumVars = 100;

struct VarTable
{
    char* varNames[ MaxNumVars ];
    int   numVars;
    int   isNewFunc;
};

//-----------------------------------------------------------------------------

int AddLocalVarsBlock   ( FILE* file, int isNewFunc = false );
int RemoveLocalVarsBlock( FILE* file );

int GetTableVarPos( const char* varName );
int AddVarToTable ( const char* varName );

//-----------------------------------------------------------------------------

#endif