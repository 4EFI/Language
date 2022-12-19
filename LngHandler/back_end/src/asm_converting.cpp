
#include "config.h"
#include "asm_converting.h"

#include "dsl.h"
#include "my_assert.h"
#include "tree_lng.h"
#include "tree_lng_dump.h"
#include "LOG.h"
#include "stack.h"
#include "lng_tools.h"
#include "ru_translitor.h"
#include "var_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

//-----------------------------------------------------------------------------

int TreeToAsmConverting( Node* node, FILE* file )
{
    ASSERT( node != NULL, 0 );    
    ASSERT( file != NULL, 0 );

    AddLocalVarsBlock();

    TreeToAsm( node, file );

    fprintf( file, "\nout\n" ); // for test !!!! (DELETE IT!!)

    fprintf( file, "\nhlt\n" );

    return 1;
}

//-----------------------------------------------------------------------------


//  ASM rules
//-----------------------------------------------------------------------------

int TreeToAsm( Node* node, FILE* file )
{
    ASSERT( node != NULL, 0 );    
    ASSERT( file != NULL, 0 );

    int isAsm = 0;

    isAsm += MathExpressionToAsm( node, file );
    isAsm += IfToAsm            ( node, file );
    isAsm += WhileToAsm         ( node, file );

    if( isAsm ) return 0;
    
    if( node->left  ) TreeToAsm( node->left,  file );
    if( node->right ) TreeToAsm( node->right, file );

    return 1;
}

//-----------------------------------------------------------------------------

int MathExpressionToAsm( Node* node, FILE* file )
{
    ASSERT( node != NULL, 0 );    
    ASSERT( file != NULL, 0 );

    if( NODE_TYPE != OP_TYPE  && 
        NODE_TYPE != VAR_TYPE && 
        NODE_TYPE != VAL_TYPE )
    {   
        return 0;
    }

    if( node->left  ) MathExpressionToAsm( node->left,  file );
    if( node->right ) MathExpressionToAsm( node->right, file );

    switch( NODE_TYPE )
    {
        case OP_TYPE:
        {
            const char* str = OpStrings[ GetIndexOperation( NODE_OP ) ].strAsm;
            fprintf( file, "%s\n", str );

            break;
        }

        case VAL_TYPE:
        {
            fprintf( file, "push %g\n", NODE_VAL );
            break;
        } 
    }

    return 1; 
}

//-----------------------------------------------------------------------------

int IfToAsm( Node* node, FILE* file )
{
    ASSERT( node != NULL, 0 );    
    ASSERT( file != NULL, 0 );

    static int ifCount = 0;

    if( NODE_TYPE != IF_TYPE ) return 0;

    ifCount++;

    MathExpressionToAsm( node->left, file );

    fprintf( file, "push 0\nje :endif%03d\n\n", ifCount );

    int isElse = ( ( IS_R_EXISTS && R_TYPE == ELSE_TYPE ) ? 1 : 0 );     
    if( isElse )
    {
        TreeToAsm( node->right->left, file );
    }
    else
    {
        TreeToAsm( node->right, file );
    }

    fprintf( file, "\nendif%03d:\n\n", ifCount );

    if( isElse )
    {
        TreeToAsm( node->right->right, file );
    }

    return 1;
}

//-----------------------------------------------------------------------------

int WhileToAsm( Node* node, FILE* file )
{
    ASSERT( node != NULL, 0 );    
    ASSERT( file != NULL, 0 );

    static int whileCount = 0;

    if( NODE_TYPE != WHILE_TYPE ) return 0;

    whileCount++;

    // While start
    fprintf( file, "\nwhile%03d:\n", whileCount );

    MathExpressionToAsm( node->left, file );

    fprintf( file, "push 0\nje :endWhile%03d\n\n", whileCount );

    TreeToAsm( node->right, file );

    fprintf( file, "\njmp :while%03d\n" "endWhile%03d:\n\n", whileCount, whileCount );

    return 1;
}

//-----------------------------------------------------------------------------