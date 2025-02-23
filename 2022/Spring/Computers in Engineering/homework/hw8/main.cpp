/****************************************************************************************
*                                                                                       *
*    05/05/2022                                                                         *
*    Matt Kaufman                                                                       *
*                                                                                       *
*    HW8 - Linked and doubly-linked lists.                                              *
*    Functions.h and Functions.cpp could be removed, but keeping for now just in case.  *
*                                                                                       *
*****************************************************************************************/

#include "Node.h"
#include "MyList.h"

typedef Node* NodePtr;




int main()
{
    MyList myList;

    myList.insert(3);
    myList.insert(2);
    myList.insert(1);
    myList.printAscending();
    myList.printDescending();
    myList.remove(2);
    myList.printAscending();
    myList.printDescending();
    myList.clear();
    myList.printAscending();
    myList.printDescending();
    myList.insert(12);
    myList.insert(19);
    myList.insert(6);
    myList.insert(75);
    myList.remove(18);
    myList.printAscending();
    myList.printDescending();
    myList.remove(75);
    myList.remove(6);
    myList.printAscending();
    myList.printDescending();


    return 0;
}
