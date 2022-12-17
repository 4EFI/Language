
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

Stack* LngTokenization( const char* str )
{
	ASSERT( str != NULL, NULL );

	Stack*     stk = ( Stack* )calloc( 1, sizeof( Stack ) );
	StackCtor( stk, 1 );

	char*   str_ptr = ( char* )str;
	while( *str_ptr != '\0' )
	{	
		int numReadSyms = 0;

		double num = 0;
		int isNum = sscanf( str_ptr, "%lf%n", &num, &numReadSyms ); 
		if( isNum == -1 ) break;

		if( isNum )
		{
			StackPush( stk, CREATE_VAL_NODE( num ) );
		}
		else
		{
			char* strType = ( char* )calloc( MaxStrLen, sizeof( char ) );

			int  isStr = sscanf( str_ptr, "%s%n", strType, &numReadSyms );
			if( !isStr ) break;

			int opType = -1;
			int type   = GetType( strType, &opType );

			if/* */( type == OP_TYPE )
			{
				StackPush( stk, CREATE_OP_NODE( opType, NULL, NULL ) );	
			}
			else if( type == -1 )
			{					
				StackPush( stk, CREATE_VAR_NODE( TranslitString( strType, strlen( strType ) ) ) );
			}
			else
			{
				StackPush( stk, CREATE_TYPE_NODE( type ) );
			}
		}

		str_ptr += numReadSyms; 
	}

	StackPush( stk, CREATE_TYPE_NODE( END_RROG_TYPE ) );

	return stk;
}

//-----------------------------------------------------------------------------