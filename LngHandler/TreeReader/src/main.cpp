
#include <stdio.h>

#include "tree_lng.h"
#include "tree_lng_reader.h"
#include "tree_lng_dump.h"
#include "str_algs.h"
#include "file_algs.h"

int main()
{
    const char* fileDataName = "lng_data.txt";
    FILE* file = fopen( fileDataName, "r" ); 

    if( !file ) return -1;

    char* lngData = NULL;
    ReadAllFile( file, &lngData );

    Node* lngNode = LoadLngTree( lngData );

    LngGraphDump( lngNode );    
}