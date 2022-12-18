
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

//-----------------------------------------------------------------------------

int TreeToAsm( Node* node, FILE* file )
{
    ASSERT( node != NULL, NULL );    
    ASSERT( file != NULL, NULL );

    int isAsm = 0;

    isAsm += MathExpressionToAsm( node, file );

    if( isAsm ) return 0;
    
    if( node->left  ) TreeToAsm( node->left,  file );
    if( node->right ) TreeToAsm( node->right, file );

    return 1;
}

//-----------------------------------------------------------------------------

int MathExpressionToAsm( Node* node, FILE* file )
{
    ASSERT( node != NULL, NULL );    
    ASSERT( file != NULL, NULL );

    if( NODE_TYPE != OP_TYPE  && 
        NODE_TYPE != VAR_TYPE && 
        NODE_TYPE != VAL_TYPE )
    {   
        return NULL;
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

}

//-----------------------------------------------------------------------------