
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

int AddLocalVarsBlock( FILE* file, int isNewFunc )
{
    ASSERT( file != NULL, 0 );
    
    static int isStkEmpty = true;
    if(        isStkEmpty        ) { isStkEmpty = false; StackCtor( &StkVarTables, 1 ); }

    VarTable* varTable  = ( VarTable* )calloc( 1, sizeof( VarTable ) );
    varTable->numVars   = 0;
    varTable->isNewFunc = isNewFunc;
    
    StackPush( &StkVarTables, varTable );

    int stkSize = StkVarTables.size;
    if( stkSize > 1 ) // Shift rax
    {
        fprintf( file, "push %d\n", StkVarTables.data[ stkSize - 2 ]->numVars );
        fprintf( file, "push rax\n" );
        fprintf( file, "add\n" );
        fprintf( file, "pop rax ; Shifting top the var register\n" );

        LOG( "New block" );
    }

    return 1;
}

//-----------------------------------------------------------------------------

int RemoveLocalVarsBlock( FILE* file )
{
    ASSERT( file != NULL, 0 );

    StackPop( &StkVarTables );

    int stkSize = StkVarTables.size;
    if( stkSize > 0 )
    {
        fprintf( file, "push %d\n", -StkVarTables.data[ stkSize - 1 ]->numVars );
        fprintf( file, "push rax\n" );
        fprintf( file, "add\n" );
        fprintf( file, "pop rax ; Shifting down the var register\n" );
        
        LOG( "Remove block" );
    }

    return 1;
}

//-----------------------------------------------------------------------------

int GetTableVarPos( const char* varName )
{
    ASSERT( varName != NULL, 0 );

    int numBack        = 0;
    int indexLastBlock = StkVarTables.size - 1;

    int isNewFunc = false;

    for( int curTable = indexLastBlock; curTable >= 0; curTable-- )
    {        
        int curTableSize = StkVarTables.data[ curTable ]->numVars;
        
        if( !isNewFunc || curTable == 0 )
        {
            for( int i = 0; i < curTableSize; i++ )
            {
                char* curVarName = StkVarTables.data[ curTable ]->varNames[i];
                
                if( !strcmp( varName, curVarName ) )
                {                
                    if( curTable == indexLastBlock ) { return i; }
                    else                             { return -( numBack + curTableSize - i ); } 
                }
            }

            isNewFunc = StkVarTables.data[ curTable ]->isNewFunc;
        }

        if( curTable != indexLastBlock ) numBack += curTableSize; // If not the last local block
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

    LOG( "CurTable = %d", curTable );

    int curVarPos = StkVarTables.data[ curTable ]->numVars;

    StkVarTables.data[ curTable ]->varNames[ curVarPos ] = ( char* )varName; // set varName
    StkVarTables.data[ curTable ]->numVars++;

    return curVarPos;
}

//-----------------------------------------------------------------------------