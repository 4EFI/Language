
#include "config.h"
#include "var_handler.h"

#include "stack.h"
#include "my_assert.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

//-----------------------------------------------------------------------------

Stack StkVarTables = { 0 };

//-----------------------------------------------------------------------------

int VarTableCtor( VarTable* varTable )
{
    ASSERT( varTable != NULL, 0 );

    varTable = ( VarTable* )calloc( 1, sizeof( VarTable ) );

    varTable->table   = ( Var* )calloc( MaxNumVars, sizeof( Var ) );
    varTable->numVars = 0;

    return 1;
}

int AddLocalVarsBlock()
{
    VarTable          *varTable = NULL;
    if( !VarTableCtor( varTable ) ) return 0;
    
    StackPush( &StkVarTables, varTable );

    return 1;
}

//-----------------------------------------------------------------------------