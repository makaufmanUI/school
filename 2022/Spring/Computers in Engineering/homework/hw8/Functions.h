/*****************************************************************
*                                                                *
*    Functions.h                                                 *
*                                                                *
*    Declares the Functions used in main.cpp (no longer used)    *
*                                                                *
******************************************************************/

#ifndef HW8_FUNCTIONS_H
#define HW8_FUNCTIONS_H

#include "Node.h"
typedef Node* NodePtr;      // this allows you to use just  "NodePtr x"  instead of  "Node* x"



void printNodes(NodePtr startPtr);
void insertNodeInLinkedList(NodePtr* ptr2startPtr, int value);
void deleteNodeFromLinkedList(NodePtr* ptr2startPtr, int value);



#endif //HW8_FUNCTIONS_H
