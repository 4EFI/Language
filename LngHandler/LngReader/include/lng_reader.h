#ifndef LNG_READER_H
#define LNG_READER_H

//-----------------------------------------------------------------------------

struct Node;
struct Stack;

Node*  GetLngTree     ( const char* str );
Stack* LngTokenization( const char* str );

Node* GetGrammar     ( Stack* nodes, int* curPos );
Node* GetExpression  ( Stack* nodes, int* curPos );
Node* GetMulDiv      ( Stack* nodes, int* curPos );
Node* GetPower       ( Stack* nodes, int* curPos );
Node* GetBracket     ( Stack* nodes, int* curPos );
Node* GetStrMathsFunc( Stack* nodes, int* curPos ); // ln sin cos
Node* GetVar         ( Stack* nodes, int* curPos );
Node* GetNumber      ( Stack* nodes, int* curPos );

//-----------------------------------------------------------------------------

#endif