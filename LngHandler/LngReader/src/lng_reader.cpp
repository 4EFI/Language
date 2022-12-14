
#include "config.h"
#include "lng_reader.h"

#include "dsl.h"
#include "my_assert.h"
#include "tree_lng.h"
#include "tree_lng_dump.h"
#include "LOG.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

//-----------------------------------------------------------------------------

Node* GetLngTree( const char* str )
{
	ASSERT( str != NULL, NULL );

	Node* node = LngTokenization( str );

	return node;
}

//-----------------------------------------------------------------------------

int GetNumOperation( const char* str )
{
	ASSERT( str != NULL, -1 );

	for( int i = 0; i < NumOpStrings; i++ )
    {
        if( strcmp( str, OpStrings[i].str ) == 0 )
        {
            return OpStrings[i].opNum;
        }
    }	

	return -1;
}

int GetNumType( const char* str )
{
	ASSERT( str != NULL, -1 );



	return -1;
}

Node* LngTokenization( const char* str )
{
	ASSERT( str != NULL, NULL );

	char* str_ptr = ( char* )str;

	while( *str_ptr != '\0' )
	{	
		int numReadSyms = 0;

		double num = 0;
		int isNum = sscanf( str_ptr, "%lf%n", &num, &numReadSyms ); 

		if( isNum )
		{
			LOG( "%d: lf = %lf; %d", isNum, num, numReadSyms );
		}
		else
		{
			char strType[ MaxStrLen ] = "";

			int isStr = sscanf( str_ptr, "%s%n", strType, &numReadSyms );

			LOG( "%d: s = '%s' %d", isStr, strType, numReadSyms );

			if( !isStr ) break;
		}

		str_ptr += numReadSyms; 
	}

	Node* node = NULL;

	return node;
}

//-----------------------------------------------------------------------------


//	Recursive descent 
//-----------------------------------------------------------------------------

Node* GetGrammar( char** str )
{   
	char* str_ptr = ( char* )(*str);

    Node* node = GetExpression( &str_ptr );
	LinkNodeParents( node, NULL );

	assert( *str_ptr == '\0' );

	return node;
}

//-----------------------------------------------------------------------------

Node* GetExpression( char** str )
{
	Node* node = GetMulDiv( str );

	while( **str == '+' || **str == '-' )
	{
		char op = **str;
		(*str)++;

		Node* nodeR = GetMulDiv( str );
		Node* nodeL = CopyLngNode( node );	

		if( op == '+' )
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

Node* GetMulDiv( char** str )
{
	Node* node = GetPower( str );

	while( **str == '*' || **str == '/' )
	{
		char op = **str;
		(*str)++;

		Node* nodeR = GetPower( str );
		Node* nodeL = CopyLngNode( node );	

		if( op == '*' )
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

Node* GetPower( char** str )
{
	Node* node = GetBracket( str );

	if( **str == '^' ) 
	{
		(*str)++;

		Node* nodeR = GetPower( str );
		Node* nodeL = CopyLngNode( node );	

		node = POW( nodeL, nodeR );
	}

	return node;
}

//-----------------------------------------------------------------------------

Node* GetBracket( char** str )
{	
	if( **str == '(' )
	{
		(*str)++;
		
		Node* node = GetExpression( str );
		
		assert( **str == ')' );
		(*str)++;

		return node;
	}
    
    return GetStrMathsFunc( str );
}

//-----------------------------------------------------------------------------

Node* GetStrMathsFunc( char** str )
{
	int op  = IsStrMathsFunc( str );
	if( op != -1 )
	{
		Node* nodeR = GetBracket( str ); 
		
		switch( op )
		{
			case OP_SIN: return SIN( NULL, nodeR );
			case OP_COS: return COS( NULL, nodeR );
			case OP_LN:  return LN ( NULL, nodeR );
		}
	}
	
    return GetVar( str );
}

//-----------------------------------------------------------------------------

int IsStrMathsFunc( char** str )
{
	if/* */( **str == 's' && *(*str + 1) == 'i' && *(*str + 2) == 'n' )
	{
		*str += 3;
		return OP_SIN;
	}
	else if( **str == 'c' && *(*str + 1) == 'o' && *(*str + 2) == 's' )
	{
		*str += 3;
		return OP_COS;
	}
	else if( **str == 'l' && *(*str + 1) == 'n' )
	{
		*str += 2;
		return OP_LN;
	}

	return -1;
}

//-----------------------------------------------------------------------------

Node* GetVar( char** str )
{	
	if( isalpha( **str ) )
	{        
		char* var    = ( char* )calloc( MaxStrLen, sizeof( char ) );
		char* oldStr = *str;
				
		int i = 0;	
		while( isalpha(**str) && i < MaxStrLen )
		{
			var[i] = **str;
			(*str)++;
			
            i++;	
		}
		
		assert( oldStr != *str );

		return CREATE_VAR_NODE( var );;
	}
	
	return GetNumber( str );
}

//-----------------------------------------------------------------------------

Node* GetNumber( char** str )
{
	double val   = 0;

    const char* sOld = *str;
	
    while( '0' <= **str && **str <= '9' )
    {
        val = val * 10 + (**str) - '0';
        (*str)++; 
    }

	assert( *str != sOld );
	
	return CREATE_VAL_NODE( val );
}

// End recursive descent
//-----------------------------------------------------------------------------