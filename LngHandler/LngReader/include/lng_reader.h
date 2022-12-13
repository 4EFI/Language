#ifndef LNG_READER_H
#define LNG_READER_H

struct Node;

Node* GetGrammar   ( char** str );
Node* GetExpression( char** str );
Node* GetMulDiv    ( char** str );
Node* GetPower     ( char** str );
Node* GetBracket   ( char** str );
Node* GetStrFunc   ( char** str );
Node* GetVar       ( char** str );
Node* GetNumber    ( char** str );

int IsStrFunc( char** str );

#endif