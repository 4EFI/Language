
#include "config.h"
#include "lng_parsing.h"
#include "lng_tokenization.h"

#include "dsl.h"
#include "my_assert.h"
#include "tree_lng.h"
#include "tree_lng_dump.h"
#include "LOG.h"
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

	Node** nodes = LngTokenization( str );

	LngGraphDumpNodes( nodes );

	int curPos = 0;
	Node* node = GetGrammar( nodes, &curPos );

	LOG( "Tree have been parsed" );

	LngGraphDumpTree( node );

	return node;
}

//-----------------------------------------------------------------------------


//	Recursive descent 
//-----------------------------------------------------------------------------

Node* GetGrammar( Node** nodes, int* curPos )
{   
   	Node* node = GetStatement( nodes, curPos );

	assert( CUR_NODE_TYPE == END_RROG_TYPE );

	LinkNodeParents( node, NULL );
	return node;
}

//-----------------------------------------------------------------------------

Node* GetStatement( Node** nodes, int* curPos )
{
	LOG( "Enter GetStatement" );
	LOG( "%d :", *curPos );
	LOG( "%d", CUR_NODE_TYPE );
	
	if( CUR_NODE_TYPE == SEMICOLON_TYPE )
	{
		NEXT_TOKEN
		return GetStatement( nodes, curPos );
	}

	if( CUR_NODE_TYPE == END_RROG_TYPE || 
		CUR_NODE_TYPE == R_BRACE_TYPE ) return NULL;

	Node* nodeL = GetFunction ( nodes, curPos );
	Node* nodeR = GetStatement( nodes, curPos );
	
	return CREATE_TYPE_NODE_LR( ST_TYPE, nodeL, nodeR );
}

//-----------------------------------------------------------------------------

Node* GetFunction( Node** nodes, int* curPos )
{
	LOG( "Enter GetFunction" );
	LOG( "%d :", *curPos );
	
	int oldCurPos = *curPos;
	
	if( CUR_NODE_TYPE == VAR_INIT_TYPE )
    {
		NEXT_TOKEN
		assert( CUR_NODE_TYPE == VAR_TYPE );

		char* name = CUR_NODE_VAR;

        NEXT_TOKEN

		if( CUR_NODE_TYPE != L_BRACKET_TYPE )
		{
			*curPos = oldCurPos;
			
			return GetInitVar( nodes, curPos );
		}

		assert( CUR_NODE_TYPE == L_BRACKET_TYPE ); // (
		NEXT_TOKEN

        Node* nodeL = GetParams( nodes, curPos );

		assert( CUR_NODE_TYPE == R_BRACKET_TYPE ); // )
		NEXT_TOKEN

		// Get body
		ASSERT_L_BRACE // !:

		Node* nodeR = GetStatement( nodes, curPos );

		ASSERT_R_BRACE // :!
    
        Node* node = CREATE_TYPE_NODE_LR( FUNC_TYPE, nodeL, nodeR );
		node->value->varValue = name;

		return node;
    }

    return GetWhile( nodes, curPos );
}

//-----------------------------------------------------------------------------

Node* VarInitHandler( Node** nodes, int* curPos )
{
	NEXT_TOKEN

	Node* nodeL = CUR_NODE;

	assert( CUR_NODE_TYPE == VAR_TYPE );
	NEXT_TOKEN

	Node* nodeR = NULL;

	if( CUR_NODE_TYPE == EQ_TYPE )
	{
		NEXT_TOKEN

		nodeR = GetAddSub( nodes, curPos );
	}

	return CREATE_TYPE_NODE_LR( VAR_INIT_TYPE, nodeL, nodeR );
}

//-----------------------------------------------------------------------------

Node* GetParams( Node** nodes, int* curPos )
{
	LOG( "Enter GetParams" );
	LOG( "%d :", *curPos );
	
	Node* node     = NULL;
	Node* lastNode = NULL;

	int isNewParam = false;
	
	while( CUR_NODE_TYPE == VAR_INIT_TYPE || isNewParam )
	{
		Node* nodeVar   = VarInitHandler( nodes, curPos );

		Node* nodeParam = CREATE_TYPE_NODE_LR( PARAM_TYPE, nodeVar, NULL );

		if( !lastNode ) { node            = nodeParam; lastNode = node;            }
		else            { lastNode->right = nodeParam; lastNode = lastNode->right; }
		
		if( CUR_NODE_TYPE == COMMA_TYPE )
		{
			isNewParam = true;
			NEXT_TOKEN
		}
	}

	return node;
}

//-----------------------------------------------------------------------------

Node* GetInitVar( Node** nodes, int* curPos )
{
	LOG( "Enter GetInitVar" );
	LOG( "%d :", *curPos );
	
	if( CUR_NODE_TYPE == VAR_INIT_TYPE )
	{
		return VarInitHandler( nodes, curPos );
	}

	return GetAddSub( nodes, curPos );
}

//-----------------------------------------------------------------------------

Node* GetEqual( Node** nodes, int* curPos )
{
	LOG( "Enter GetEqual" );
	LOG( "%d :", *curPos );
	
	if( CUR_NODE_TYPE == VAR_TYPE )
	{
		Node* nodeL = CUR_NODE;
		NEXT_TOKEN

		if( CUR_NODE_TYPE != EQ_TYPE )
		{
			PREV_TOKEN
			return GetAddSub( nodes, curPos );
		}
		
		NEXT_TOKEN

		Node* nodeR = GetAddSub( nodes, curPos );

		return CREATE_TYPE_NODE_LR( EQ_TYPE, nodeL, nodeR );
	}

	return GetInitVar( nodes, curPos );
}

//-----------------------------------------------------------------------------

Node* GetIf( Node** nodes, int* curPos )
{
	LOG( "Enter GetIf" );
	LOG( "%d :", *curPos );

	Node* node     = NULL;
	Node* lastNode = NULL;
	
	int isElse = false;
	
	while( CUR_NODE_TYPE == IF_TYPE )
	{
		isElse = false;
		
		NEXT_TOKEN
		Node* nodeL = GetAddSub( nodes, curPos );

		ASSERT_L_BRACE // !:

		Node* nodeR = GetStatement( nodes, curPos );

		ASSERT_R_BRACE // :!

		Node* tempNode = CREATE_TYPE_NODE_LR( IF_TYPE, nodeL, NULL );

		if( !lastNode ) { node            = tempNode; lastNode = node;            }
		else            { lastNode->right = tempNode; lastNode = lastNode->right; }

		if( CUR_NODE_TYPE == ELSE_TYPE )
		{
			NEXT_TOKEN
			isElse = true;

			lastNode->right = CREATE_TYPE_NODE_LR( ELSE_TYPE, nodeR, NULL );
			lastNode        = lastNode->right;
		} 
		else
		{
			lastNode->right = nodeR;
			return node;
		}
	}

	if( isElse )
	{
		assert( CUR_NODE_TYPE == L_BRACKET_TYPE );
		NEXT_TOKEN

		lastNode->right = GetStatement( nodes, curPos );

		assert( CUR_NODE_TYPE == R_BRACKET_TYPE );
		NEXT_TOKEN

		return node;
	}

	return GetEqual( nodes, curPos );
}

//-----------------------------------------------------------------------------

Node* GetWhile( Node** nodes, int* curPos )
{
	LOG( "Enter GetWhile" );
	LOG( "%d :", *curPos );
	
	if( CUR_NODE_TYPE == WHILE_TYPE )
	{
		NEXT_TOKEN
		
		Node* nodeL = GetAddSub( nodes, curPos );

		ASSERT_L_BRACE // !:

		Node* nodeR = GetStatement( nodes, curPos );

		ASSERT_R_BRACE // :!

		return CREATE_TYPE_NODE_LR( WHILE_TYPE, nodeL, nodeR );	
	}

	GetIf( nodes, curPos );
}

//-----------------------------------------------------------------------------

Node* GetAddSub( Node** nodes, int* curPos )
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

Node* GetMulDiv( Node** nodes, int* curPos )
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

Node* GetPower( Node** nodes, int* curPos )
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

Node* GetBracket( Node** nodes, int* curPos )
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

Node* GetStrMathsFunc( Node** nodes, int* curPos ) // sin cos ln
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

Node* GetVar( Node** nodes, int* curPos )
{	
	LOG( "Enter GetVar" );
	LOG( "%d :", *curPos );

	Node* node = CUR_NODE;
	
	if( CUR_NODE_TYPE == VAR_TYPE ) { NEXT_TOKEN return node; }
	
	return GetNumber( nodes, curPos );
}

//-----------------------------------------------------------------------------

Node* GetNumber( Node** nodes, int* curPos )
{
	LOG( "Enter GetNumber" );
	LOG( "%d :", *curPos );
	
	assert( CUR_NODE_TYPE == VAL_TYPE );

	Node* node = CUR_NODE;

	NEXT_TOKEN

	return node;
}

// End recursive descent
//-----------------------------------------------------------------------------