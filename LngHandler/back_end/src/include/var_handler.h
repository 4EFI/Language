#ifndef VAR_HANDLER_H
#define VAR_HANDLER_H

//-----------------------------------------------------------------------------

struct Var
{
    char* varName;
    int   pos;
};

const int MaxNumVars = 1024;

struct VarTable
{
    Var* table;
    int numVars;
};

int VarTableCtor( VarTable* varTable );

//-----------------------------------------------------------------------------

int AddLocalVarsBlock();

//-----------------------------------------------------------------------------

#endif