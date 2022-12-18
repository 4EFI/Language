#ifndef ASM_CONVERTING_H
#define ASM_CONVERTING_H

#include <stdio.h>

//-----------------------------------------------------------------------------

struct Node;

int TreeToAsm( Node* node, FILE* file );

int MathExpressionToAsm( Node* node, FILE* file );
int IfToAsm            ( Node* node, FILE* file );

//-----------------------------------------------------------------------------

#endif