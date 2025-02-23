/*************************************************************
*                                                            *
*    Node.h                                                  *
*                                                            *
*    Declares the Node class and its methods and members     *
*                                                            *
**************************************************************/

#ifndef HW8_NODE_H
#define HW8_NODE_H



class Node {
public:
    Node();                                     /* Default constructor. */
    ~Node();                                    /* Destructor. */
    explicit Node(int data);                    /* Explicit constructor with data parameter. */

    void print() const;                         /* Prints the node's data. */
    int getData() const;                        /* Returns the node's data. */
    void setData(int newData);                  /* Sets the node's data. */
    void setNextPtr(Node* newPtr);              /* Sets the node's next pointer. */
    void setPrevPtr(Node* newPtr);              /* Sets the node's previous pointer. */
    Node* getNextPtr() { return nextPtr; }      /* Returns the node's next pointer. */
    Node* getPrevPtr() { return prevPtr; }      /* Returns the node's previous pointer. */

private:
    int data;               /* payload */
    Node* nextPtr;          /* pointer to the next Node pointer */
    Node* prevPtr;          /* pointer to the previous Node pointer */
};




#endif //HW8_NODE_H
