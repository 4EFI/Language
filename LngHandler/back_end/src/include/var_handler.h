#ifndef VAR_HANDLER_H
#define VAR_HANDLER_H

//-----------------------------------------------------------------------------

const int MaxNumVars = 100;

struct VarTable
{
    char* varNames[ MaxNumVars ];
    int   numVars;
};

//-----------------------------------------------------------------------------

int AddLocalVarsBlock();

int GetTableVarPos( const char* varName );
int AddVarToTable ( const char* varName );

//-----------------------------------------------------------------------------

#endif