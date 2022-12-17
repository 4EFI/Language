
#include <stdio.h>

#include "tree_lng.h"
#include "tree_lng_parsing.h"
#include "tree_lng_dump.h"
#include "str_algs.h"
#include "file_algs.h"

int main()
{
    const char* fileDataName = "../test.lng.tree";
    FILE* file = fopen( fileDataName, "r" ); 

    if( !file ) return -1;

    char* lngData = NULL;
    ReadAllFile( file, &lngData );

    Node* lngTree = LoadLngTree( lngData );

    LngGraphDumpTree( lngTree );    
}