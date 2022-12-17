
#include "config.h"

#include "lng_tokenization.h"
#include "dsl.h"
#include "tree_lng.h"
#include "tree_lng_dump.h"
#include "my_assert.h"
#include "LOG.h"
#include "stack.h"
#include "lng_tools.h"
#include "ru_translitor.h"

#include <stdio.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------

int PrintPreorderLngNodes( Node* node, FILE* file ) 
{
    ASSERT( node != NULL && file != NULL, 0 );

    char str[ MaxStrLen ] = "";
    PrintLngNode( str, node, TREE );

    fprintf( file, "{ %s ", str );

    if( node->left )  
    {
        PrintPreorderLngNodes( node->left, file );
    }
    if( node->right ) 
    {
        PrintPreorderLngNodes( node->right, file );
    }
    
    fprintf( file, "} " );

    return 1;
}

//-----------------------------------------------------------------------------