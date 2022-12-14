#ifndef CONFIG_H
#define CONFIG_H

//-----------------------------------------------------------------------------

struct LngNode
{
    int type; 

    // union in progress
    double dblValue;
    int    opValue;
    char*  varValue;
};

struct Node;

typedef LngNode* TreeElem_t;
typedef    Node*     Elem_t;

// #define STACK_DUMP

//-----------------------------------------------------------------------------

enum Types
{
    OP_TYPE, 

    VAL_TYPE, 
    VAR_TYPE,

    L_COMMENT_TYPE, // /*
    R_COMMENT_TYPE, // */

    L_BRACKET_TYPE, // (
    R_BRACKET_TYPE, // )

    VAR_INIT_TYPE,
};

//-----------------------------------------------------------------------------


// Operation description
//-----------------------------------------------------------------------------

enum OperationTypes
{
    OP_ADD = 0, 
    OP_SUB = 1,

    OP_MUL = 3,
    OP_DIV = 4,

    OP_LN  = 6,

    OP_SIN = 8,
    OP_COS = 9,

    OP_DEG = 11
};

static int UnaryOperations[] = 
{
    OP_SIN,
    OP_COS,
    OP_LN
};

static const int NumUnaryOperations = sizeof( UnaryOperations ) / sizeof( int ); 

struct OpStr
{
    int         opNum;
    const char* str;
};

static OpStr OpStrings[] = 
{  
    { OP_ADD, "+"   },
    { OP_SUB, "-"   },
    { OP_MUL, "*"   },
    { OP_DIV, "/"   },
    { OP_DEG, "^"   },
    { OP_SIN, "sin" },
    { OP_COS, "cos" },
    { OP_LN,  "ln"  }
};

static const int NumOpStrings = sizeof( OpStrings ) / sizeof( OpStr );

// End operation description
//-----------------------------------------------------------------------------


//  Types description
//-----------------------------------------------------------------------------

struct TypeStr
{
    int         typeNum;
    const char* str;
};

static TypeStr TypeStrings[] = 
{
    { L_COMMENT_TYPE, "/*" },
    { R_COMMENT_TYPE, "*/" },
    
    { L_BRACKET_TYPE, "(" },
    { R_BRACKET_TYPE, ")" },

    { VAR_INIT_TYPE, "варибля" }
};

static const int NumTypeStrings = sizeof( TypeStrings ) / sizeof( TypeStr );

//  End types description
//-----------------------------------------------------------------------------


//  Constants
//-----------------------------------------------------------------------------

const int MaxStrLen = 255;

const double Accuracy = 0.001;

//-----------------------------------------------------------------------------

#endif