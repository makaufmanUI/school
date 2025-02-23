/***************************************************
*                                                  *
*    Node.cpp                                      *
*                                                  *
*    Defines the Node class declared in Node.h     *
*                                                  *
****************************************************/

#include "Node.h"
#include <iostream>






/***********************************************************
*                                                          *
*   Default constructor                                    *
*   Assigns both member pointers to NULL, and data to 0.   *
*                                                          *
*   Args: None                                             *
*   Returns: None                                          *
*                                                          *
************************************************************/
Node::Node()
{
    std::cout << "> Node created with default value 0." << std::endl;
    data = 0;
    nextPtr = nullptr;
    prevPtr = nullptr;
}


/**************************************************************************
*                                                                         *
*   Explicit constructor                                                  *
*   Assigns both member pointers to NULL, and data to parameter value.    *
*                                                                         *
*   Args:                                                                 *
*       data (type int) - the value to be stored in the node              *
*   Returns: None                                                         *
*                                                                         *
***************************************************************************/
Node::Node(int data)
{
    std::cout << "> Node created with value " << data << "." << std::endl;
    this->data = data;
    nextPtr = nullptr;
    prevPtr = nullptr;
}


/*******************************************
*                                          *
*   Destructor                             *
*   Prints a node destruction message.     *
*                                          *
*   Args: None                             *
*   Returns: None                          *
*                                          *
********************************************/
Node::~Node()
{
    std::cout << "> Node destroyed." << std::endl;
}


/*******************************************
*                                          *
*   print() method                         *
*   Prints the data stored in the node.    *
*                                          *
*   Args: None                             *
*   Returns: None                          *
*                                          *
********************************************/
void Node::print() const
{
    std::cout << data << " ";
}


/**************************************************
*                                                 *
*   getData() method                              *
*   Returns the data stored in the node.          *
*                                                 *
*   Args: None                                    *
*   Returns:                                      *
*       data (type int) - the data in the node    *
*                                                 *
***************************************************/
int Node::getData() const
{
    return data;
}


/******************************************************
*                                                     *
*   setData() method                                  *
*   Sets the data variable of the node.               *
*                                                     *
*   Args: None                                        *
*       newData (type int) - the new data to store    *
*   Returns: None                                     *
*                                                     *
*******************************************************/
void Node::setData(int newData)
{
    data = newData;
}


/**********************************************************
*                                                         *
*   setNextPtr() method                                   *
*   Sets the nextPtr variable of the node.                *
*                                                         *
*   Args:                                                 *
*       newPtr (type Node*) - the new pointer to store    *
*   Returns: None                                         *
*                                                         *
***********************************************************/
void Node::setNextPtr(Node* newPtr)
{
    nextPtr = newPtr;
}


/**********************************************************
*                                                         *
*   setPrevPtr() method                                   *
*   Sets the prevPtr variable of the node.                *
*                                                         *
*   Args: None                                            *
*       newPtr (type Node*) - the new pointer to store    *
*   Returns: None                                         *
*                                                         *
***********************************************************/
void Node::setPrevPtr(Node* newPtr)
{
    prevPtr = newPtr;
}
