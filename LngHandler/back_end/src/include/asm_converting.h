#ifndef ASM_CONVERTING_H
#define ASM_CONVERTING_H

#include <stdio.h>

//-----------------------------------------------------------------------------

struct Node;

int TreeToAsm(  );

int MathExpressionToAsm( Node* node, FILE* file );

int VarHandler(  );

//-----------------------------------------------------------------------------

#endif