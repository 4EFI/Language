

#include "config.h"

#include "tree_lng.h"
#include "tree_lng_reader.h"
#include "dsl.h"
#include "my_assert.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-----------------------------------------------------------------------------

Node* LoadLngTree( const char* lngData ) 
{
    ASSERT( lngData != NULL, NULL );

    Node* currNode = CREATE_VAL_NODE( 0 );
    int   currOp   = -1;

    int len = strlen( lngData );
    for( int i = 0; i < len; i++ )
    {
        if( lngData[i] == ' ' ) continue;

        if ( lngData[i] == '{' )
        {
            if( !currNode->left )
            {
                currNode = TreeAddChild( currNode, NULL, LEFT_SIDE );
                continue;
            }
            else
            {
                currNode = TreeAddChild( currNode, NULL, RIGHT_SIDE );
                currOp   = -1;
                continue;
            }
        }

        if( lngData[i] == '}' )
        {
            currNode = currNode->parent;
            continue;
        }

        //i += SetDiffNode( currNode, diffData + i, &currOp );
        //i --; 
    }

    //LinkNodeParents( node, NULL );

    return currNode;
}

//-----------------------------------------------------------------------------