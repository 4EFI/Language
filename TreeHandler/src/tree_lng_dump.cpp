
#include "tree_lng_dump.h"
#include "tree_lng.h"
#include "my_assert.h"

#include <stdio.h>
#include <args.h>

// DiffDump
//-----------------------------------------------------------------------------

int GraphVizNodes( Node* node, FILE* dotFile, int* nodeNum )
{
    ASSERT( dotFile != NULL && nodeNum != NULL, 0 );
    
    int leftNum  = 0;
    int rightNum = 0;
    
    if/* */( node->left )
    {
        leftNum  = GraphVizNodes( node->left,  dotFile, nodeNum );
    }    
    if( node->right )
    {   
        rightNum = GraphVizNodes( node->right, dotFile, nodeNum );
    }

    int   typeNum = VAL_TYPE;
    char* typeStr = NULL;
    char* color   = NULL;

    if( node->value != NULL ) typeNum = node->value->type; 

    if/* */( typeNum == Types::OP_TYPE  ) { typeStr = "op"  ; color = "lightgrey"  ; }
    else if( typeNum == Types::VAL_TYPE ) { typeStr = "val" ; color = "lightgreen" ; }
    else if( typeNum == Types::VAR_TYPE ) { typeStr = "var" ; color = "lightblue"  ; }

    char valueStr[ MaxStrLen ] = "";
    PrintDiffNodeValue( valueStr, node );
    
    fprintf( dotFile, "\tnode%d[ shape = record, style = \"filled\", fillcolor = \"%s\", label = \"{ p: %p | n: %p | { %s | %s } }\" ];\n", 
                       *nodeNum, color, node->parent, node, typeStr, valueStr );                                      

    if( node->left )
    {
        fprintf( dotFile, "\t\"node%d\" -> \"node%d\"\n", *nodeNum, leftNum );
    }
    if( node->right )
    {
        fprintf( dotFile, "\t\"node%d\" -> \"node%d\"\n", *nodeNum, rightNum );
    }

    (*nodeNum)++;
    return *nodeNum - 1;
}

//-----------------------------------------------------------------------------

FILE* DiffCreateDotDumpFile( Node* node, const char* fileName )
{
    ASSERT( node != NULL, NULL );

    FILE* tempDotFile = fopen( fileName, "w" );

    fprintf( tempDotFile, "digraph ListDump\n" );
    fprintf( tempDotFile, "{\n" );
    {
        int curNodeNum = 1;  
        GraphVizNodes( node, tempDotFile, &curNodeNum );
    }
    fprintf( tempDotFile, "}\n" );

    return tempDotFile;
}

//-----------------------------------------------------------------------------

int DiffGraphDump( Node* node, const char* str, ... )
{
    ASSERT( node != NULL, 0 );

    const char* tempDotFileName = "temp_graph_viz_tree.dot"; 
    FILE*       tempDotFile = DiffCreateDotDumpFile( node, tempDotFileName );
    fclose(     tempDotFile     );

    static int dumpNum = 0;

    char     graphName[MaxStrLen] = "";
    sprintf( graphName, "img/graph%d.png", dumpNum++ );

    CreateGraphVizImg( tempDotFileName, graphName, "png" );

    // Delete temp file
    remove( tempDotFileName );

    // Create html
    fclose( FileLngDump );
    FileLngDump = fopen( FileLngDumpName, "a+" );

    va_list   arg = {0};
    va_start( arg, str );

    // Create html file
    fprintf( FileLngDump, "<pre>" );
    fprintf( FileLngDump, "<font size = 4>" );
   vfprintf( FileLngDump, str, arg );
    fprintf( FileLngDump, "</font>" );

    if( str ) 
    {
        fprintf( FileLngDump, "\n\n" );
    }

    fprintf( FileLngDump, "<img src = \"%s\", style = \" max-width: 95vw \">", graphName );
    fprintf( FileLngDump, "<hr>" );

    va_end( arg );
    fclose( FileLngDump );

    return 1;
}

//-----------------------------------------------------------------------------
// End DiffDump
