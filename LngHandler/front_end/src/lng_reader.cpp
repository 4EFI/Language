
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

	int curPos = 0;
	Node* node = GetGrammar( nodes, &curPos );

	LngGraphDumpTree( node );

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


//	Recursive descent 
//-----------------------------------------------------------------------------

Node* GetGrammar( Stack* nodes, int* curPos )
{   
   	Node* node = GetStatememt( nodes, curPos );

	assert( CUR_NODE_TYPE == END_RROG_TYPE );

	LinkNodeParents( node, NULL );
	return node;
}

//-----------------------------------------------------------------------------

Node* GetStatememt( Stack* nodes, int* curPos )
{
	LOG( "Enter GetStatement" );
	LOG( "%d :", *curPos );
	LOG( "%d", CUR_NODE_TYPE );
	
	if( CUR_NODE_TYPE == SEMICOLON_TYPE )
	{
		NEXT_TOKEN
		return GetStatememt( nodes, curPos );
	}

	if( CUR_NODE_TYPE == END_RROG_TYPE || 
		CUR_NODE_TYPE == R_BRACKET_TYPE ) return NULL;

	Node* nodeL = GetWhile    ( nodes, curPos );
	Node* nodeR = GetStatememt( nodes, curPos );
	
	return CREATE_TYPE_NODE_LR( ST_TYPE, nodeL, nodeR );
}

//-----------------------------------------------------------------------------

Node* GetInitVar( Stack* nodes, int* curPos )
{
	LOG( "Enter GetInitVar" );
	LOG( "%d :", *curPos );
	
	if( CUR_NODE_TYPE == VAR_INIT_TYPE )
	{
		NEXT_TOKEN
		assert( CUR_NODE_TYPE == VAR_TYPE );
	
		Node* nodeL = CUR_NODE;
		NEXT_TOKEN

		assert( CUR_NODE_TYPE == EQ_TYPE );
		NEXT_TOKEN
		
		Node* nodeR = GetAddSub( nodes, curPos ); 

		return CREATE_TYPE_NODE_LR( VAR_INIT_TYPE, nodeL, nodeR );
	}

	return GetAddSub( nodes, curPos );
}

//-----------------------------------------------------------------------------

Node* GetEqual( Stack* nodes, int* curPos )
{
	LOG( "Enter GetEqual" );
	LOG( "%d :", *curPos );
	
	if( CUR_NODE_TYPE == VAR_TYPE )
	{
		Node* nodeL = CUR_NODE;
		NEXT_TOKEN

		assert( CUR_NODE_TYPE == EQ_TYPE );
		NEXT_TOKEN

		Node* nodeR = GetAddSub( nodes, curPos );

		return CREATE_TYPE_NODE_LR( EQ_TYPE, nodeL, nodeR );
	}

	return GetInitVar( nodes, curPos );
}

//-----------------------------------------------------------------------------

Node* GetIf( Stack* nodes, int* curPos )
{
	LOG( "Enter GetIf" );
	LOG( "%d :", *curPos );
	
	if( CUR_NODE_TYPE == IF_TYPE )
	{
		NEXT_TOKEN
		Node* nodeL = GetAddSub( nodes, curPos );

		assert( CUR_NODE_TYPE == L_BRACKET_TYPE );
		NEXT_TOKEN

		Node* nodeR = GetElse( nodes, curPos );

		assert( CUR_NODE_TYPE == R_BRACKET_TYPE );
		NEXT_TOKEN

		return CREATE_TYPE_NODE_LR( IF_TYPE, nodeL, nodeR );
	}

	return GetEqual( nodes, curPos );
}

//-----------------------------------------------------------------------------

Node* GetElse( Stack* nodes, int* curPos )
{
	if( CUR_NODE_TYPE == ELSE_TYPE )
	{
		assert( CUR_NODE_TYPE == L_BRACKET_TYPE );
		NEXT_TOKEN
		
		Node* node = CREATE_TYPE_NODE_LR( ELSE_TYPE, GetStatememt( nodes, curPos ), NULL );	

		assert( CUR_NODE_TYPE == R_BRACKET_TYPE );
		NEXT_TOKEN

		return node;
	}

	return GetStatememt( nodes, curPos );
}

//-----------------------------------------------------------------------------

Node* GetWhile( Stack* nodes, int* curPos )
{
	if( CUR_NODE_TYPE == WHILE_TYPE )
	{
		NEXT_TOKEN
		
		Node* nodeL = GetAddSub( nodes, curPos );

		assert( CUR_NODE_TYPE == L_BRACKET_TYPE ); 
		NEXT_TOKEN

		Node* nodeR = GetStatememt( nodes, curPos );

		assert( CUR_NODE_TYPE == R_BRACKET_TYPE ); 
		NEXT_TOKEN

		return CREATE_TYPE_NODE_LR( WHILE_TYPE, nodeL, nodeR );	
	}

	GetIf( nodes, curPos );
}

//-----------------------------------------------------------------------------

Node* GetAddSub( Stack* nodes, int* curPos )
{
	Node* node = GetMulDiv( nodes, curPos );

	while( CUR_NODE_TYPE == OP_TYPE && ( CUR_NODE_OP == OP_ADD || CUR_NODE_OP == OP_SUB ) ) // + or -
	{
		int op = CUR_NODE_OP;
		NEXT_TOKEN

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
	LOG( "Enter GetMulDiv" );
	LOG( "%d :", *curPos );
	
	Node* node = GetPower( nodes, curPos );

	while( CUR_NODE_TYPE == OP_TYPE && ( CUR_NODE_OP == OP_MUL || CUR_NODE_OP == OP_DIV ) ) // * or /
	{
		int op = CUR_NODE_OP;
		NEXT_TOKEN

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
	LOG( "Enter GetPower" );
	LOG( "%d :", *curPos );
	
	Node* node = GetBracket( nodes, curPos );

	if( CUR_NODE_TYPE == OP_TYPE && CUR_NODE_OP == OP_DEG ) // ^ 
	{
		NEXT_TOKEN

		Node* nodeR = GetBracket( nodes, curPos );
		Node* nodeL = CopyLngNode( node );	

		node = POW( nodeL, nodeR );
	}

	return node;
}

//-----------------------------------------------------------------------------

Node* GetBracket( Stack* nodes, int* curPos )
{		
	LOG( "Enter GetBracket" );
	LOG( "%d :", *curPos );
	
	if( CUR_NODE_TYPE == L_BRACKET_TYPE )
	{				
		NEXT_TOKEN

		LOG( "(" );
		
		Node* node = GetAddSub( nodes, curPos );
		
		assert( CUR_NODE_TYPE == R_BRACKET_TYPE );
		NEXT_TOKEN

		LOG( ")" );

		return node;
	}
    
    return GetStrMathsFunc( nodes, curPos );
}

//-----------------------------------------------------------------------------

Node* GetStrMathsFunc( Stack* nodes, int* curPos ) // sin cos ln
{	
	LOG( "Enter GetStrMathsFunc" );
	LOG( "%d :", *curPos );
	
	if( CUR_NODE_TYPE != OP_TYPE ) return GetVar( nodes, curPos );

	int op = CUR_NODE_OP;
	
	NEXT_TOKEN

	Node* nodeR = GetBracket( nodes, curPos );
	
	switch( op )
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
	LOG( "Enter GetVar" );
	LOG( "%d :", *curPos );

	Node* node = CUR_NODE;
	
	if( CUR_NODE_TYPE == VAR_TYPE ) { NEXT_TOKEN return node; }
	
	return GetNumber( nodes, curPos );
}

//-----------------------------------------------------------------------------

Node* GetNumber( Stack* nodes, int* curPos )
{
	LOG( "Enter GetNumber" );
	LOG( "%d :", *curPos );
	
	if( CUR_NODE_TYPE != VAL_TYPE ) return NULL;

	Node* node = CUR_NODE;

	NEXT_TOKEN

	return node;
}

// End recursive descent
//-----------------------------------------------------------------------------