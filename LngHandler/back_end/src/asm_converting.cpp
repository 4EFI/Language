
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

int MathExpressionToAsm( Node* node, FILE* file )
{
    ASSERT( node != NULL, NULL );    
    ASSERT( file != NULL, NULL );

    if( node->right ) MathExpressionToAsm( node->right, file );
    if( node->left  ) MathExpressionToAsm( node->left,  file );

    if/* */( NODE_TYPE == OP_TYPE ) 
    {
        const char* str = OpStrings[ GetIndexOperation( NODE_OP ) ].strAsm;
        fprintf( file, "%s\n", str );
    }
    else if( NODE_TYPE == VAL_TYPE )
    {
        fprintf( file, "push %g\n", NODE_VAL );
    }

    return 1; 
}

//-----------------------------------------------------------------------------