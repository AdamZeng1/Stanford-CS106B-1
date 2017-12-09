/* CS 106B Stanford/    Summer 2015
 * Ali Parsaei          ID: 05950982
 */

#ifndef _heappriorityqueue_h
#define _heappriorityqueue_h

#include <iostream>
#include <string>
#include "PQEntry.h"
using namespace std;

/*
 * For documentation of each member, see VectorPriorityQueue.h.
 */
class HeapPriorityQueue {
public:
    HeapPriorityQueue();
    ~HeapPriorityQueue();
    void changePriority(string value, int newPriority);
    void clear();
    string dequeue();
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const HeapPriorityQueue& queue);

private:
    PQEntry* myElements;
    int myCapacity;
    int mySize;
    void checkResize();
    void swapElements(int i, int j);
    void bubbleDown(int startPoint);
    void bubbleUp(int startPoint);
    void removeElement(string value, int newPriority);
};

#endif
