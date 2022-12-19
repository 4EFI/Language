#ifndef ASM_CONVERTING_H
#define ASM_CONVERTING_H

#include <stdio.h>

//-----------------------------------------------------------------------------

struct Node;

int TreeToAsmConverting( Node* node, FILE* file );

int TreeToAsm          ( Node* node, FILE* file );
int MathExpressionToAsm( Node* node, FILE* file );
int IfToAsm            ( Node* node, FILE* file );
int WhileToAsm         ( Node* node, FILE* file );

//-----------------------------------------------------------------------------

#endif