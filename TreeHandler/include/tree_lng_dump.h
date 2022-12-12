#ifndef TREE_LNG_DUMP
#define TREE_LNG_DUMP

#include "config.h"

#include <stdio.h>
#include <tree_lng.h>

static const char* FileLngDumpName = "lng_dump.html";

int GraphVizNodes( Node* node, FILE* dotFile, int* nodeNum );

FILE* LngCreateDotDumpFile( Node* node, const char* fileName );
int   LngGraphDump        ( Node* node, const char* str = "", ... );

#endif