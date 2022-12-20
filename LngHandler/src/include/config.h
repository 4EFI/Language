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
struct VarTable;

typedef  LngNode* TreeElem_t;
typedef VarTable*     Elem_t;

// #define STACK_DUMP

//-----------------------------------------------------------------------------

enum Types
{
    END_PROG_TYPE,
    SEMICOLON_TYPE,

    ST_TYPE,
    
    OP_TYPE, 

    VAL_TYPE, 
    VAR_TYPE,

    L_COMMENT_TYPE, // /*
    R_COMMENT_TYPE, // */

    L_BRACKET_TYPE, // (
    R_BRACKET_TYPE, // )

    L_BRACE_TYPE,
    R_BRACE_TYPE,

    VAR_INIT_TYPE,

    EQ_TYPE,

    IF_TYPE,
    ELSE_TYPE, 

    WHILE_TYPE, 
    
    FUNC_TYPE,
    PARAM_TYPE,
    COMMA_TYPE, 

    IN_TYPE,
    OUT_TYPE
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
    const char* strStandart;
    const char* strAsm;
};

static OpStr OpStrings[] = 
{  
    { OP_ADD, "+",   "ADD", "add" },
    { OP_SUB, "-",   "SUB", "sub" },
    { OP_MUL, "*",   "MUL", "mul" },
    { OP_DIV, "/",   "DIV", "div" },
    { OP_DEG, "^",   "POW", "pow" },
    { OP_SIN, "sin", "SIN", "sin" },
    { OP_COS, "cos", "COS", "cos" },
    { OP_LN,  "ln",  "LN",  "ln"  }
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
    const char* strStandart;
};

static TypeStr TypeStrings[] = 
{
    { END_PROG_TYPE,  "\\0", "" },
    { SEMICOLON_TYPE, ";", "" },

    { ST_TYPE, "ST", "ST" },
    
    { L_COMMENT_TYPE, "!%", "" },
    { R_COMMENT_TYPE, "%!", "" },

    { L_BRACE_TYPE, "!:", "" },
    { R_BRACE_TYPE, ":!", "" },
    
    { L_BRACKET_TYPE, "(", "" },
    { R_BRACKET_TYPE, ")", "" },

    { VAR_INIT_TYPE, "варибля", "VAR" },
    { EQ_TYPE, "=", "EQ" }, 

    { IF_TYPE,   "проверочка", "IF" },
    { ELSE_TYPE, "иначе", "ELSE" },

    { WHILE_TYPE, "живем", "WHILE" },
    
    { FUNC_TYPE,  "FUNC",  "FUNC" },
    { COMMA_TYPE, ",",     "" },
    { PARAM_TYPE, "PARAM", "PARAM" },

    { IN_TYPE,  "IN",  "IN" }, 
    { OUT_TYPE, "OUT", "OUT" }
};

static const int NumTypeStrings = sizeof( TypeStrings ) / sizeof( TypeStr );

//  End types description
//-----------------------------------------------------------------------------


//  Constants
//-----------------------------------------------------------------------------

const int MaxStrLen   = 255;
const int MaxNumNodes = 10000;

const double Accuracy = 0.001;

//-----------------------------------------------------------------------------

#endif