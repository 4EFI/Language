#ifndef LNG_READER_H
#define LNG_READER_H

//-----------------------------------------------------------------------------

struct Node;

Node* GetLngTree     ( const char* str );
Node* LngTokenization( const char* str );

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