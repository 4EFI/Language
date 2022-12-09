
#include "tree_lng_reader.h"

#include "tree_lng.h"
#include "my_assert.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-----------------------------------------------------------------------------

int LoadLngTree( const char* lngData, Node* node ) 
{
    ASSERT( node    != NULL, 0 );
    ASSERT( lngData != NULL, 0 );

    Node* currNode = node;
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

        if( lngData[i] == ')' )
        {
            currNode = currNode->parent;
            continue;
        }

        //i += SetDiffNode( currNode, diffData + i, &currOp );
        i --;
    }

    //LinkNodeParents( node, NULL );

    return 1;
}

//-----------------------------------------------------------------------------