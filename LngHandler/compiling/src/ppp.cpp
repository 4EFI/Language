
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define ssystem( str, ... )              \
{										 \
	char temp[ MaxStrLen ] = "";         \
	sprintf( temp, str, ##__VA_ARGS__ ); \
	system(  temp  );                    \
}

int main()
{
	ssystem( "lng_to_tree.exe" );	
	ssystem( "tree_to_asm.exe" );	
}