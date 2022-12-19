
#include "config.h"

#include "lng_tokenization.h"
#include "dsl.h"
#include "tree_lng.h"
#include "tree_lng_dump.h"
#include "my_assert.h"
#include "LOG.h"
#include "lng_tools.h"
#include "ru_translitor.h"

#include <stdio.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------

int PrintPreorderLngNodes( Node* node, FILE* file ) 
{    
    if( node == NULL )
    {        
        fprintf( file, "{ NIL } " );
        return 0;
    }

    char str[ MaxStrLen ] = "";
    PrintLngNode( str, node, TREE );

    fprintf( file, "{ %s ", str );

    if( node->left || node->right )
    {
        PrintPreorderLngNodes( node->left,  file );
        PrintPreorderLngNodes( node->right, file );
    }
    
    fprintf( file, "} " );

    return 1;
}

//-----------------------------------------------------------------------------