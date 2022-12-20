
#include "config.h"
#include "var_handler.h"

#include "stack.h"
#include "my_assert.h"
#include "LOG.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

//-----------------------------------------------------------------------------

Stack StkVarTables = { 0 };

//-----------------------------------------------------------------------------

int AddLocalVarsBlock()
{
    static int isStkEmpty = true;
    if(        isStkEmpty        ) { isStkEmpty = false; StackCtor( &StkVarTables, 1 ); }

    VarTable* varTable = ( VarTable* )calloc( 1, sizeof( VarTable ) );
    varTable->numVars  = 0;
    
    StackPush( &StkVarTables, varTable );

    return 1;
}

//-----------------------------------------------------------------------------

int GetTableVarPos( const char* varName )
{
    ASSERT( varName != NULL, 0 );

    int numBack = 0;

    for( int curTable = StkVarTables.size - 1; curTable >= 0; curTable-- )
    {        
        int curTableSize = StkVarTables.data[ curTable ]->numVars;
        
        for( int i = 0; i < curTableSize; i++ )
        {
            char* curVarName = StkVarTables.data[ curTable ]->varNames[i];
            
            if( !strcmp( varName, curVarName ) )
            {
                return i /* - numBack */ ;
            }
        }
    }

    printf( "Variable \"%s\" does not exist...\n", varName );

    assert( 0 );
    return  0;
}

//-----------------------------------------------------------------------------

int AddVarToTable( const char* varName )
{
    ASSERT( varName != NULL, 0 );

    int curTable  = StkVarTables.size - 1;

    LOG( "%d", curTable );

    int curVarPos = StkVarTables.data[ curTable ]->numVars;

    StkVarTables.data[ curTable ]->varNames[ curVarPos ] = ( char* )varName; // set varName
    StkVarTables.data[ curTable ]->numVars++;

    return curVarPos;
}

//-----------------------------------------------------------------------------

