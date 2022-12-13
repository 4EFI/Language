
#include <stdio.h>
#include <locale.h>

#include "tree_lng.h"
#include "lng_reader.h"
#include "file_algs.h"
#include "LOG.h"

int main()
{
    setlocale( LC_ALL, "rus" );
    
    const char* fileDataName = "test.lng";
    FILE* file = fopen( fileDataName, "r" ); 

    if( !file ) return -1;

    char* lngData = NULL;
    ReadAllFile( file, &lngData );
    
    LOG( "%s", lngData );

    // Node* lngNode = GetGrammar(  );
}