/* CS 106B Stanford/    Summer 2015
 * Ali Parsaei          ID: 05950982
 * Sources:
 * Lecture slides (mostly Heap), from CS 106B Lectures
 */

#include <iostream>
#include <string>
#include "PQEntry.h"
#include "HeapPriorityQueue.h"

HeapPriorityQueue::HeapPriorityQueue() {
    myElements = new PQEntry[100]();    // creating a new array
    mySize = 1;                         // There's no element in it yet
    myCapacity = 100;                   // Picking a capacity to begin with
}

HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] myElements;                // in order not to leak memory.
}

/* This function swaps two elements. */
void HeapPriorityQueue::swapElements(int i, int j) {
    PQEntry temp = myElements[i];
    myElements[i] = myElements[j];
    myElements[j] = temp;
}

/* This function is to organize a newly added element in the heap. */
void HeapPriorityQueue::bubbleUp(int startPoint) {
    for (int i = startPoint; i/2 >= 1; i /= 2) {
        if (myElements[i] < myElements[i/2])
            swapElements(i, i/2);
        else
            break;
    }
}

/* I use this private function to remove an element and re-organize the heap array again. */
void HeapPriorityQueue::removeElement(string value, int newPriority) {
    int myIndex = 1;
    for (int i = 1; i <= mySize; i++) { // finds the index corresponding to the value.
        if (i == mySize) // it means it has searched the list and has not found it.
            throw " the value is not in the list";
        if (myElements[i].value == value) {
            myIndex = i;
            if (myElements[myIndex].priority < newPriority)
                throw "The new priority is less urgent than the old one";
            break;
        }
    }

    for (int i = myIndex; i < mySize - 1; i++) {
        myElements[i] = myElements[i + 1]; // shifts the elements after the removed element.
        bubbleUp(i);                       // organizes them at each step.
    }
    mySize--;
}

/* It removes the element and enqueues the element's value with a new priority. */
void HeapPriorityQueue::changePriority(string value, int newPriority) {
    removeElement(value,newPriority);
    enqueue(value, newPriority);
}
/* This function clears the elements */
void HeapPriorityQueue::clear() {
    mySize = 1;                         // empties the array.
}

/* This function is to bubble down in dequeueing step.  It swaps the parrent with
 * more urgent child. in case of 2 urgent children, it swaps it with the more urgent one.
 */
void HeapPriorityQueue::bubbleDown(int startPoint) {
    int i = startPoint;  // Sets the starting element
    while (2*i < mySize) {      // Checks if the parent has any children
        if (2*i+1 < mySize) {   // checks if the parent has 2 children.
            if (myElements[i] > myElements[2*i] || myElements[i] > myElements[2*i+1]) {
                if (myElements[2*i] <= myElements[2*i+1]) {
                    swapElements(i,2*i);    // Inner if-else statement Swaps the parent with
                    i = 2*i;                // the more urgent child.
                } else {
                    swapElements(i,2*i+1);  // by indexing, we change the parent's index to
                    i = 2*i + 1;            // the index of swapped child.
                }
            } else {
                break;
            }
        } else {    // if it only has a child, swaps with it in case the child is more urgent
            if (myElements[i] > myElements[2*i]) {
                swapElements(i,2*i);
                i = 2*i;
            } else {
                break;
            }
        }
    }

}

/* dequeue takes out the most urgent element's value, then puts the last urgent element in
 * the queue at the front of the queue, then bubbles it down in the queue. */
string HeapPriorityQueue::dequeue() {
    if (mySize == 1)
        throw "The queue is empty";
    string out = myElements[1].value;
    if (mySize > 2) {
        myElements[1] = myElements[mySize - 1];
    }
    mySize--;
    bubbleDown(1); // reorganize the heap
    return out;
}

/* inserts the newly added element in the right place. */
void HeapPriorityQueue::enqueue(string value, int priority) {
    checkResize();      // insering new element at the end of the queue regardless of priority
    myElements[mySize] = PQEntry(value, priority);
    mySize++;           // increasing size
    bubbleUp(mySize - 1); // organize the heap
}

/* Returns true if the array is empty. */
bool HeapPriorityQueue::isEmpty() const {
    return mySize == 1;
}

/* Returns the most urgent element's value */
string HeapPriorityQueue::peek() const {
    if (mySize == 1)
        throw "The list is empty";
    else
        return myElements[1].value;
}

/* Returns the most urgent element's priority. */
int HeapPriorityQueue::peekPriority() const {
    if (mySize == 1)
        throw "The list is empty";
    else
        return myElements[1].priority;
}

/* returns the size of the heap array. */
int HeapPriorityQueue::size() const {
    return mySize - 1;
}

/* Prints out the array in a nice way, showing the priority of each string as well. */
ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out << "{";
    if (!queue.isEmpty()) {         // I use the prinitng operator from PQEntry.h for PQEntry elemns.
        out << queue.myElements[1]; // In order to print nicely, I separated this element.
        for (int i = 2; i < queue.mySize; i++)
            out << ", " << queue.myElements[i];
    }
    out << "}";
    return out;
}

/* In order to avoid crashing the program because of reaching the end of array, we create a
 * bigger array with twice capacity of the original array as soon as we reach the end of array
 * and replace the old array with the new one.
 */
void HeapPriorityQueue::checkResize() {
    if (mySize == myCapacity) {
        // copy all elements into a bigger array
        PQEntry* bigger = new PQEntry[2 * myCapacity]();
        for (int i = 1; i < mySize; i++) {
            bigger[i] = myElements[i];
        }
        // swap in the new bigger array for the old one
        delete[] myElements;
        myElements = bigger;
        myCapacity *= 2;
    }
}
