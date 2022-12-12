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

typedef LngNode* TreeElem_t;

enum Types
{
    OP_TYPE, 
    VAL_TYPE, 
    VAR_TYPE
};

enum OperationTypes
{
    OP_ADD = 0, 
    OP_SUB = 1,

    OP_MUL = 3,
    OP_DIV = 4,

    OP_LN  = 6,

    OP_SIN = 8,
    OP_COS = 9,

    OP_DEG
};

struct Operation
{
    int opType;

    const char* opStr;
};

static Operation Operations[] = 
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

static int NumOperations = sizeof( Operations ) / sizeof( Operation );

static int UnaryOperations[] = 
{
    OP_SIN,
    OP_COS,
    OP_LN
};

static int NumUnaryOperations = sizeof( UnaryOperations ) / sizeof( int ); 

//-----------------------------------------------------------------------------

const int MaxStrLen = 255;

const double Accuracy = 0.001;

//-----------------------------------------------------------------------------

#endif