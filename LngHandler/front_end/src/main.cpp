
#include <stdio.h>
#include <locale.h>

#include "ru_translitor.h"
#include "tree_lng.h"
#include "tree_lng_dump.h"
#include "lng_parsing.h"
#include "lng_saving.h"
#include "file_algs.h"
#include "LOG.h"

//-----------------------------------------------------------------------------

int main()
{
    setlocale(LC_ALL, ".UTF-8");
    
    const char* fileDataName = "test.lng";
    FILE* file = fopen( fileDataName, "r" ); 
    if(  !file  ) return -1;

    char* lngData = NULL;
    ReadAllFile( file, &lngData );

    Node* lngNode = GetLngTree( lngData );

    char fileOutName[ MaxStrLen ] = "";
    sprintf( fileOutName, "%s.tree", fileDataName );

    FILE* file_out = fopen( fileOutName, "w" ); 
    if(  !file_out  ) return -1;

    PrintPreorderLngNodes( lngNode, file_out );

    return 0;
}

//-----------------------------------------------------------------------------

// ( 2 + 2 - sin ( 2 ) ) ^ 5

/* варибля владос сначала 0
владос теперь 7

варибля гей сначала 1

проверочка владос - 0
{
	
}
иначе проверочка саня - гей
{

} */