
#include "config.h"
#include "asm_converting.h"

#include "dsl.h"
#include "my_assert.h"
#include "tree_lng.h"
#include "tree_lng_dump.h"
#include "LOG.h"
#include "stack.h"
#include "lng_tools.h"
#include "ru_translitor.h"
#include "var_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

//-----------------------------------------------------------------------------

int TreeToAsmConverting( Node* node, FILE* file )
{
    ASSERT( node != NULL, 0 );    
    ASSERT( file != NULL, 0 );

    AddLocalVarsBlock( file );

    fprintf( file, "jmp :Mein\n" );

    TreeToAsm( node, file );

    fprintf( file, "\nhlt\n" );

    return 1;
}

//-----------------------------------------------------------------------------


//  ASM rules
//-----------------------------------------------------------------------------

int TreeToAsm( Node* node, FILE* file )
{
    ASSERT( node != NULL, 0 );    
    ASSERT( file != NULL, 0 );

    int isAsm = 0;

    isAsm += FuncToAsm          ( node, file );
    isAsm += InputToAsm         ( node, file );
    isAsm += OutputToAsm        ( node, file );
    isAsm += WhileToAsm         ( node, file );
    isAsm += IfToAsm            ( node, file );
    isAsm += VarInitToAsm       ( node, file );
    isAsm += VarEqualToAsm      ( node, file );
    isAsm += MathExpressionToAsm( node, file );
    isAsm += CallFuncToAsm      ( node, file );

    if( isAsm ) return 0;
    
    if( node->left  ) TreeToAsm( node->left,  file );
    if( node->right ) TreeToAsm( node->right, file );

    return 1;
}

//-----------------------------------------------------------------------------

int MathExpressionToAsm( Node* node, FILE* file )
{ 
    ASSERT( file != NULL, 0 );

    if( !node ) return 0;

    if( NODE_TYPE != OP_TYPE  && 
        NODE_TYPE != VAR_TYPE && 
        NODE_TYPE != VAL_TYPE )
    {   
        return 0;
    }

    if( node->left  ) MathExpressionToAsm( node->left,  file );
    if( node->right ) MathExpressionToAsm( node->right, file );

    switch( NODE_TYPE )
    {
        case OP_TYPE:
        {
            const char* str = OpStrings[ GetIndexOperation( NODE_OP ) ].strAsm;
            fprintf( file, "%s\n", str );

            break;
        }

        case VAL_TYPE:
        {
            fprintf( file, "push %g\n", NODE_VAL );
            break;
        } 

        case VAR_TYPE:
        {
            VarRAMPosToAsm( NODE_VAR, file ); 

            fprintf( file, "push [ rbx ] ; push \"%s\"\n", NODE_VAR );
            break;
        }

        default:
            return 0;
    }

    return 1; 
}

//-----------------------------------------------------------------------------

int IfToAsm( Node* node, FILE* file )
{
    ASSERT( node != NULL, 0 );    
    ASSERT( file != NULL, 0 );

    static int ifCount = 0;

    if( NODE_TYPE != IF_TYPE ) return 0;

    ifCount++;

    AddLocalVarsBlock( file );    // {

    CallFuncToAsm( node->left, file );

    fprintf( file, "push 0\nje :endif%03d\n\n", ifCount );

    int isElse = ( ( IS_R_EXISTS && R_TYPE == ELSE_TYPE ) ? 1 : 0 );     
    if( isElse )
    {
        TreeToAsm( node->right->left, file );
    }
    else
    {
        TreeToAsm( node->right, file );
    }

    fprintf( file, "\nendif%03d:\n\n", ifCount );

    if( isElse )
    {
        AddLocalVarsBlock( file );    // {

        TreeToAsm( node->right->right, file );

        RemoveLocalVarsBlock( file ); // }
    }

    RemoveLocalVarsBlock( file ); // }

    return 1;
}

//-----------------------------------------------------------------------------

int WhileToAsm( Node* node, FILE* file )
{
    ASSERT( node != NULL, 0 );    
    ASSERT( file != NULL, 0 );

    static int whileCount = 0;

    if( NODE_TYPE != WHILE_TYPE ) return 0;

    whileCount++;

    AddLocalVarsBlock( file );    // {

    // While start
    fprintf( file, "\nwhile%03d:\n", whileCount );

    CallFuncToAsm( node->left, file );

    fprintf( file, "push 0\nje :endWhile%03d\n\n", whileCount );

    TreeToAsm( node->right, file );

    fprintf( file, "\njmp :while%03d\n" "endWhile%03d:\n\n", whileCount, whileCount );

    RemoveLocalVarsBlock( file ); // }

    return 1;
}

//-----------------------------------------------------------------------------

int VarInitToAsm( Node* node, FILE* file )
{
    ASSERT( node != NULL, 0 );    
    ASSERT( file != NULL, 0 );

    if( NODE_TYPE != VAR_INIT_TYPE ) return 0;

    AddVarToTable( L_VAR );

    int isVal = CallFuncToAsm( node->right, file );
    
    VarRAMPosToAsm( L_VAR, file );
    
    if( isVal ) fprintf( file, "pop [ rbx ] ; set \"%s\"\n\n", L_VAR );

    return 1;
}  

//-----------------------------------------------------------------------------

int VarEqualToAsm( Node* node, FILE* file )
{
    ASSERT( node != NULL, 0 );    
    ASSERT( file != NULL, 0 );

    if( NODE_TYPE != EQ_TYPE ) return 0;

    CallFuncToAsm( node->right, file );

    VarRAMPosToAsm( L_VAR, file );
    fprintf( file, "pop [ rbx ] ; set \"%s\"\n\n", L_VAR );

    return 1;
}

//-----------------------------------------------------------------------------

int InputToAsm( Node* node, FILE* file )
{
    ASSERT( node != NULL, 0 );    
    ASSERT( file != NULL, 0 );

    if( NODE_TYPE != IN_TYPE ) return 0;

    CallParamsToAsm( node->left, file, IN );

    return 1;
}

//-----------------------------------------------------------------------------

int OutputToAsm( Node* node, FILE* file )
{
    ASSERT( node != NULL, 0 );    
    ASSERT( file != NULL, 0 );

    if( NODE_TYPE != OUT_TYPE ) return 0;

    CallParamsToAsm( node->left, file, OUT );

    return 1;
}

//-----------------------------------------------------------------------------

int FuncToAsm( Node* node, FILE* file )
{
    ASSERT( node != NULL, 0 );    
    ASSERT( file != NULL, 0 );

    if( NODE_TYPE != FUNC_TYPE ) return 0;

    fprintf( file, "%s:\n", L_VAR );
    AddLocalVarsBlock( file, true ); // {

    FuncParamsToAsm( node->left->left, file );

    TreeToAsm( node->right, file );

    RemoveLocalVarsBlock( file );    // }
    fprintf( file, "ret\n" );
    fprintf( file, "end%s:\n", L_VAR );

    return 1;
}   

//-----------------------------------------------------------------------------

int FuncParamsToAsm( Node* node, FILE* file )
{
    ASSERT( node != NULL, 0 );    
    ASSERT( file != NULL, 0 );

    if( NODE_TYPE != PARAM_TYPE ) return 0;

    if( node->left ) 
    {
        VarInitToAsm( node->left, file );
        fprintf( file, "pop [ rbx ]\n\n" ); // set var
    }

    if( node->right ) FuncParamsToAsm( node->right, file );

    return 1;
}

//-----------------------------------------------------------------------------

int CallParamsToAsm( Node* node, FILE* file, int typeParams )
{ 
    ASSERT( file != NULL, 0 );

    if( !node ) return 0;

    if( NODE_TYPE != PARAM_TYPE ) return 0;

    if( typeParams == IN ) fprintf( file, "in\n" );

    if( node->left ) 
    {
        if/* */( typeParams == IN  ) VarRAMPosToAsm( L_VAR,      file );
        else if( typeParams == OUT ) CallFuncToAsm ( node->left, file );
    }

    if( typeParams == OUT ) fprintf( file, "out\n" );
    if( typeParams == IN  ) fprintf( file, "pop [ rbx ]\n" ); // set var

    if( node->right ) CallParamsToAsm( node->right, file, typeParams );
    
    if( node->left  ) 
    {
        if( typeParams == FUNC ) CallFuncToAsm( node->left, file );
    }

    return 1;
}

//-----------------------------------------------------------------------------

int CallFuncToAsm( Node* node, FILE* file )
{    
    ASSERT( file != NULL, 0 );

    if( !node ) return 0;

    if( NODE_TYPE != CALL_TYPE ) return MathExpressionToAsm( node, file );

    CallParamsToAsm( node->left->left, file, FUNC );

    fprintf( file, "push rbx\n pop rcx ; save pos before call\n" );
    fprintf( file, "call :%s\n", L_VAR );
    fprintf( file, "push rcx\n pop rcx ; save pos before call\n" );

    return 1;
}

//-----------------------------------------------------------------------------

int VarRAMPosToAsm( const char* varName, FILE* file )
{
    ASSERT( varName != NULL, 0 );
    ASSERT( file    != NULL, 0 );

    int pos = GetTableVarPos( varName );
    
    fprintf( file, "push rax\n" );
    fprintf( file, "push %d\n", pos );
    fprintf( file, "add\n" );
    fprintf( file, "pop rbx ; set \"%s\" pos\n\n", varName );

    return pos;
}

//-----------------------------------------------------------------------------