/***************************************************************
*                                                              *
*    MyList.h                                                  *
*                                                              *
*    Declares the MyList class and its methods and members     *
*                                                              *
****************************************************************/

#ifndef HW8_MYLIST_H
#define HW8_MYLIST_H


#include "Node.h"



class MyList {
public:
    MyList();                   /*  constructor  */
    ~MyList();                  /*  destructor  */
    void clear();               /*  clears the list  */
    bool isEmpty() const;       /*  returns true if the list is empty  */
    void printAscending();      /*  prints the list in ascending order  */
    void printDescending();     /*  prints the list in descending order  */
    void insert(int value);     /*  inserts a new node with 'value' into the list  */
    void remove(int value);     /*  removes the first node with 'value' from the list  */
private:
    Node* currentPtr;           /*  pointer to the current node  */
};





#endif //HW8_MYLIST_H
