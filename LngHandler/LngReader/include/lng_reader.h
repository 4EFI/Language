#ifndef LNG_READER_H
#define LNG_READER_H

//-----------------------------------------------------------------------------

struct Node;
struct Stack;

Node*  GetLngTree     ( const char* str );
Stack* LngTokenization( const char* str );

Node* GetGrammar     ( char** str );
Node* GetExpression  ( char** str );
Node* GetMulDiv      ( char** str );
Node* GetPower       ( char** str );
Node* GetBracket     ( char** str );
Node* GetStrMathsFunc( char** str ); // ln sin cos
Node* GetVar         ( char** str );
Node* GetNumber      ( char** str );

int IsStrMathsFunc( char** str );

//-----------------------------------------------------------------------------

#endif