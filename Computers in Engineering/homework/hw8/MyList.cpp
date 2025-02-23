/*******************************************************
*                                                      *
*    MyList.cpp                                        *
*                                                      *
*    Defines the MyList class declared in MyList.h     *
*                                                      *
********************************************************/

#include "Node.h"
#include "MyList.h"
#include <iostream>





/************************************************************************
*                                                                       *
*   Constructor                                                         *
*   Prints a creation message and sets the currentPtr member to NULL.   *
*                                                                       *
*   Args: None                                                          *
*   Returns: None                                                       *
*                                                                       *
*************************************************************************/
MyList::MyList()
{
    std::cout << std::endl << std::endl << ">> List created."
              << std::endl << std::endl << std::endl;
    currentPtr = nullptr;
}



/*****************************************************************
*                                                                *
*   Destructor                                                   *
*   Prints a destruction message and calls the clear() method.   *
*                                                                *
*   Args: None                                                   *
*   Returns: None                                                *
*                                                                *
******************************************************************/
MyList::~MyList()
{
    std::cout << std::endl << "------------------" << std::endl
              << "Destructor called:" << std::endl << std::endl;
    clear();
    std::cout << "------------------" << std::endl << std::endl;
}



/************************************************************
*                                                           *
*   isEmpty() method                                        *
*   Checks if the current instance of the class is empty.   *
*                                                           *
*   Args: None                                              *
*   Returns:                                                *
*       True if the currentPtr member is the nullptr.       *
*                                                           *
*************************************************************/
bool MyList::isEmpty() const
{
    return currentPtr == nullptr;
}



/**********************************************************************************
*                                                                                 *
*   printAscending() method                                                       *
*   Prints a the values of the nodes currently in the list, in ascending order.   *
*                                                                                 *
*   Args: None                                                                    *
*   Returns: None                                                                 *
*                                                                                 *
***********************************************************************************/
void MyList::printAscending()
{
    std::cout << std::endl << std::endl << "------------------" << std::endl
              << "Printing list in ascending order:" << std::endl << std::endl;

    if (isEmpty()) {
        std::cout << "List is empty. Nothing to print.";
        std::cout << std::endl << "------------------" << std::endl << std::endl;
        return;
    }

    Node* temp = currentPtr;                    // Create a temp pointer to the current pointer
    while (temp != nullptr) {                   // Loop through the list
        temp->print();                              // Print the current node data
        temp = temp->getNextPtr();                  // Set the temp pointer to the next pointer
    }

    std::cout << std::endl << "------------------" << std::endl << std::endl;
}



/***********************************************************************************
*                                                                                  *
*   printDescending() method                                                       *
*   Prints a the values of the nodes currently in the list, in descending order.   *
*                                                                                  *
*   Args: None                                                                     *
*   Returns: None                                                                  *
*                                                                                  *
************************************************************************************/
void MyList::printDescending()
{
    std::cout << std::endl << "------------------" << std::endl
              << "Printing list in descending order:" << std::endl << std::endl;

    if (isEmpty()) {
        std::cout << "List is empty. Nothing to print.";
        std::cout << std::endl << "------------------" << std::endl << std::endl << std::endl;
        return;
    }

    /* Find the last node in the list */
    Node* temp = currentPtr;
    while (temp->getNextPtr() != nullptr) {
        temp = temp->getNextPtr();
    }

    /* Find the node with the smallest value in the list */
    Node* temp2 = temp;
    Node* smallest = temp2;
    while (temp2 != nullptr) {
        if (temp2->getData() < smallest->getData()) {
            smallest = temp2;
        }
        temp2 = temp2->getPrevPtr();
    }
    int smallestValue = smallest->getData();
    delete temp2;

    /* Now move in reverse order and print the data of each node */
    while (temp != nullptr) {
        if (temp->getPrevPtr() == nullptr && temp->getData() != smallestValue) {
            temp = temp->getPrevPtr();
        }
        else {
            temp->print();
            temp = temp->getPrevPtr();
        }
    }

    std::cout << std::endl << "------------------" << std::endl << std::endl << std::endl;
}



/**********************************************************************************
*                                                                                 *
*   insert() method                                                               *
*   Inserts a new node with a given value into the list, maintaining the order.   *
*                                                                                 *
*   Args:                                                                         *
*       value (type int) - the value to be stored in the new node                 *
*   Returns: None                                                                 *
*                                                                                 *
***********************************************************************************/
void MyList::insert(int value)
{
    Node* newNode = new Node(value);                // Create a new node with the value
    if (newNode != nullptr)                         // If the new node was created
    {
        if (isEmpty()) {                                // If the list is empty
            currentPtr = newNode;                           // Set the current pointer to the new node
        }
        else if (currentPtr->getData() > value) {       // If the given value is less than the current pointer's value
            newNode->setNextPtr(currentPtr);
            currentPtr->setPrevPtr(newNode);
            currentPtr = newNode;
        }
        else {                                          // If the given value is greater than or equal to the current pointer's value
            Node* temp = currentPtr;
            while (temp->getNextPtr() != nullptr  &&  temp->getNextPtr()->getData() < value) {
                temp = temp->getNextPtr();
            }
            newNode->setNextPtr(temp->getNextPtr());        // Set the next pointer of the new node to the next pointer of the temp pointer
            newNode->setPrevPtr(temp);                      // Set the previous pointer of the new node to the temp pointer
            if (temp->getNextPtr() != nullptr) {            // If the temp pointer has a next pointer
                temp->getNextPtr()->setPrevPtr(newNode);        // Set the previous pointer of the next pointer of the temp pointer to the new node
            }
            temp->setNextPtr(newNode);                      // Set the next pointer of the temp pointer to the new node
        }
    }
    else {
        std::cout << "Error: Unable to create new node." << std::endl;
    }
}



/************************************************************************
*                                                                       *
*   remove() method                                                     *
*   Removes a single node with the specified value from the list.       *
*                                                                       *
*   Args:                                                               *
*       value (type int) - the value to be removed from the node        *
*   Returns: None                                                       *
*                                                                       *
*************************************************************************/
void MyList::remove(int value)
{
    if (isEmpty()) {
        std::cout << "> List is empty. Cannot remove any nodes." << std::endl;
        return;
    }
    else if (currentPtr->getData() == value) {                          // If the current pointer has the value to be removed
        Node* temp = currentPtr;                                            // Create a temp pointer to the current pointer
        currentPtr = currentPtr->getNextPtr();                              // Set the current pointer to the next pointer of the temp pointer
        delete temp;                                                        // Delete the temp pointer
        std::cout << "> Node with value " << value << " deleted." << std::endl;
    }
    else
    {
        Node* temp = currentPtr;                                                                // Create a temp pointer to the current pointer
        while (temp->getNextPtr() != nullptr  &&  temp->getNextPtr()->getData() != value) {     // Loop through the list
            temp = temp->getNextPtr();                                                              // Set the temp pointer to the next pointer
        }
        if (temp->getNextPtr() != nullptr)                              // If the temp pointer has a next pointer
        {
            Node* temp2 = temp->getNextPtr();                               // Create a temp2 pointer to the next pointer of the temp pointer
            temp->setNextPtr(temp2->getNextPtr());                          // Set the next pointer of the temp pointer to the next pointer of the temp2 pointer
            if (temp2->getNextPtr() != nullptr) {                           // If the temp2 pointer has a next pointer
                temp2->getNextPtr()->setPrevPtr(temp);                          // Set the previous pointer of the next pointer of the temp2 pointer to the temp pointer
            }
            delete temp2;
            std::cout << "> Node with value " << value << " deleted." << std::endl;
        }
        else {
            std::cout << "> Node with value " << value << " not found. Could not remove." << std::endl;
        }
    }
}



/***********************************************************************************
*                                                                                  *
*   clear() method                                                                 *
*   Clears all nodes from the list, and deletes all of their currentPtr members.   *
*                                                                                  *
*   Args: None                                                                     *
*   Returns: None                                                                  *
*                                                                                  *
************************************************************************************/
void MyList::clear()
{
    if (isEmpty()) {
        std::cout << "List is empty. Nothing to delete.";
        return;
    }
    Node* temp = currentPtr;
    while (temp != nullptr)
    {
        Node* temp2 = temp->getNextPtr();
        std::cout << "> Node with value " << temp->getData()<< " deleted." << std::endl;
        delete temp;
        temp = temp2;
    }
    currentPtr = nullptr;
}
