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

//-----------------------------------------------------------------------------

struct  Node;
typedef Node* Elem_t; // For Stack 

//-----------------------------------------------------------------------------

const int MaxStrLen = 255;

const double Accuracy = 0.001;

//-----------------------------------------------------------------------------

#endif