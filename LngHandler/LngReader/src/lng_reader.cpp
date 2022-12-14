
#include "config.h"
#include "lng_reader.h"

#include "dsl.h"
#include "my_assert.h"
#include "tree_lng.h"
#include "tree_lng_dump.h"
#include "LOG.h"
#include "stack.h"
#include "lng_tools.h"
#include "ru_translitor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

//-----------------------------------------------------------------------------

Node* GetLngTree( const char* str )
{
	ASSERT( str != NULL, NULL );

	Stack* nodes = LngTokenization( str );

	LngGraphDumpNodes( nodes );

	int curPos;
	Node* node = GetGrammar( nodes, &curPos );

	return node;
}

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


//	Recursive descent 
//-----------------------------------------------------------------------------

Node* GetGrammar( Stack* nodes, int* curPos )
{   
    Node* node = GetExpression( nodes, curPos );
	LinkNodeParents( node, NULL );

	assert( CUR_NODE_TYPE == END_RROG_TYPE );

	return node;
}

//-----------------------------------------------------------------------------

Node* GetExpression( Stack* nodes, int* curPos )
{
	Node* node = GetMulDiv( nodes, curPos );

	while( CUR_NODE_TYPE == OP_TYPE && ( CUR_NODE_OP == OP_ADD || CUR_NODE_OP == OP_SUB ) ) // + or -
	{
		int op = CUR_NODE_OP;
		(*curPos)++;

		Node* nodeR = GetMulDiv( nodes, curPos );
		Node* nodeL = CopyLngNode( node );	

		if( op == OP_ADD )
        {        
            node = ADD( nodeL, nodeR );
        }
		else
        {
			node = SUB( nodeL, nodeR );	
        }
	}

	return node;
}

//-----------------------------------------------------------------------------

Node* GetMulDiv( Stack* nodes, int* curPos )
{
	Node* node = GetPower( nodes, curPos );

	while( CUR_NODE_TYPE == OP_TYPE && ( CUR_NODE_OP == OP_MUL || CUR_NODE_OP == OP_DIV ) ) // * or /
	{
		int op = CUR_NODE_OP;
		(*curPos)++;

		Node* nodeR = GetPower( nodes, curPos );
		Node* nodeL = CopyLngNode( node );	

		if( op == OP_MUL )
        {
			node = MUL( nodeL, nodeR );
        }
		else
        {
			node = DIV( nodeL, nodeR );
        }
	}

	return node;
}

//-----------------------------------------------------------------------------

Node* GetPower( Stack* nodes, int* curPos )
{
	Node* node = GetBracket( nodes, curPos );

	if( CUR_NODE_TYPE == OP_TYPE && CUR_NODE_OP == OP_DEG ) // ^ 
	{
		(*curPos)++;

		Node* nodeR = GetPower( nodes, curPos );
		Node* nodeL = CopyLngNode( node );	

		node = POW( nodeL, nodeR );
	}

	return node;
}

//-----------------------------------------------------------------------------

Node* GetBracket( Stack* nodes, int* curPos )
{	
	if( CUR_NODE_TYPE == L_BRACKET_TYPE )
	{
		(*curPos)++;
		
		Node* node = GetExpression( nodes, curPos );
		
		assert( CUR_NODE_TYPE == R_BRACKET_TYPE );
		(*curPos)++;

		return node;
	}
    
    return GetStrMathsFunc( nodes, curPos );
}

//-----------------------------------------------------------------------------

Node* GetStrMathsFunc( Stack* nodes, int* curPos ) // sin cos ln
{
	if( CUR_NODE_TYPE != OP_TYPE ) return GetVar( nodes, curPos );

	Node* nodeR = GetBracket( nodes, curPos ); 
	
	switch( CUR_NODE_OP )
	{
		case OP_SIN: return SIN( NULL, nodeR );
		case OP_COS: return COS( NULL, nodeR );
		case OP_LN:  return LN ( NULL, nodeR );
	}

	return GetVar( nodes, curPos );
}

//-----------------------------------------------------------------------------

Node* GetVar( Stack* nodes, int* curPos )
{	
	if( CUR_NODE_TYPE == VAR_TYPE ) return CUR_NODE;
	
	return GetNumber( nodes, curPos );
}

//-----------------------------------------------------------------------------

Node* GetNumber( Stack* nodes, int* curPos )
{
	assert( CUR_NODE_TYPE == VAL_TYPE );
	
	(*curPos)++;

	return CUR_NODE;
}

// End recursive descent
//-----------------------------------------------------------------------------