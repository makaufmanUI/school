/************************************************************************
*                                                                       *
*    Functions.cpp                                                      *
*                                                                       *
*    Defines the Functions declared in Functions.h (no longer used)     *
*                                                                       *
*************************************************************************/

#include <iostream>
#include "Functions.h"





/************************************************************************
*                                                                       *
*   printNodes()                                                        *
*   Prints the contents of the linked list.                             *
*                                                                       *
*   Args:                                                               *
*       startPtr (type Node*) - pointer to the first node in the list   *
*                                                                       *
*   Returns:                                                            *
*       None                                                            *
*                                                                       *
*************************************************************************/
void printNodes(NodePtr startPtr)
{
    if (startPtr == nullptr)
        std::cout << "List is empty." << std::endl;
    else {
        for (NodePtr currentPtr = startPtr; currentPtr != nullptr; currentPtr = currentPtr->getNextPtr())
            std::cout << currentPtr->getData() << " " << std::endl;
    }
}



/********************************************************************************
*                                                                               *
*   insertNodeInLinkedList()                                                    *
*   Inserts a node with the given value into the linked list.                   *
*                                                                               *
*   Args:                                                                       *
*       ptr2startPtr (type NodePtr*) - pointer to the first node in the list    *
*       value (type int) - value to insert into the list                        *
*                                                                               *
*   Returns:                                                                    *
*       None                                                                    *
*                                                                               *
*********************************************************************************/
void insertNodeInLinkedList(NodePtr* ptr2startPtr, int value)
{
    NodePtr newNodePtr;                         // will point to newly created node
    NodePtr beforeNodePtr, afterNodePtr;

    newNodePtr = new Node;                      // dynamically allocate memory for (create) new node


    /* insert node in list */
    if (newNodePtr != nullptr) {                // if successfully allocated memory
        newNodePtr->setData(value);             // set data in new node
        newNodePtr->setNextPtr(nullptr);        // set next pointer to null

        beforeNodePtr = nullptr;                // determine where node should be inserted
        afterNodePtr = *ptr2startPtr;           // by defining beforeNodePtr and afterNodePtr

        // not executed on the first call to insertNodeInLinkedList()
        while (afterNodePtr != nullptr  &&  afterNodePtr->getData() < value) {      /* find position */
            beforeNodePtr = afterNodePtr;                                               // before node
            afterNodePtr = afterNodePtr->getNextPtr();                                  // after node
        }

        // executed on the first call to insertNodeInLinkedList()
        if (beforeNodePtr == nullptr) {                 /* case 1: insert at beginning of list (beforeNodePtr is still NULL) */
            newNodePtr->setNextPtr(*ptr2startPtr);          // set link to prior start
            *ptr2startPtr = newNodePtr;                     // set new value for the startPtr
        }
        else {                                          /* case 2: insert in middle or at end of list */
            beforeNodePtr->setNextPtr(newNodePtr);          // set link to prior node
            newNodePtr->setNextPtr(afterNodePtr);           // set link to next node
        }
    }
    else {
        std::cout << "Error allocating memory for new node of linked list." << std::endl ;
    }
}



/********************************************************************************
*                                                                               *
*   deleteNodeFromLinkedList()                                                  *
*   Deletes the node with the given value from the linked list.                 *
*                                                                               *
*   Args:                                                                       *
*       ptr2startPtr (type NodePtr*) - pointer to the first node in the list    *
*       value (type int) - value to delete from the list                        *
*                                                                               *
*   Returns:                                                                    *
*       None                                                                    *
*                                                                               *
*********************************************************************************/
void deleteNodeFromLinkedList(NodePtr* ptr2startPtr, int value)
{
    NodePtr nodeToDeletePtr; /* will point to node to be deleted */
    NodePtr priorNodePtr;    /* will point to node immediately preceding node to be deleted */


    /* 1. determine which node should be deleted by defining nodeToDeletePtr and priorNodePtr */
    nodeToDeletePtr = *ptr2startPtr;
    priorNodePtr = nullptr;
    while (nodeToDeletePtr != nullptr && nodeToDeletePtr->getData() != value)       /* find node to delete */
    {
        priorNodePtr = nodeToDeletePtr;                                             /* before node */
        nodeToDeletePtr = nodeToDeletePtr->getNextPtr();                            /* after node */
    }


    /* 2. delete node */
    if (nodeToDeletePtr == nullptr) return;                 /* empty list or node not found */

    else if (priorNodePtr == nullptr) {                     /* first node should be deleted */
        *ptr2startPtr = (*ptr2startPtr)->getNextPtr();      /* move startPtr to second node in list */
        delete nodeToDeletePtr;                             /* free memory */
    }
    else {                                                          /* node to be deleted is not first node */  /* middle or end node should be deleted */
        priorNodePtr->setNextPtr(nodeToDeletePtr->getNextPtr());    /* skip nodeToDelete in linked list */
        delete nodeToDeletePtr;                                     /* free memory */
    }
}