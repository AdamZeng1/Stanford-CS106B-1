/* CS 106B Stanford/    Summer 2015
 * Ali Parsaei          ID: 05950982
 * Sources:
 * ArrayList.cpp from CS 106B Lectures
 *
 */

#include <iostream>
#include <string>
#include "ArrayPriorityQueue.h"
#include "PQEntry.h"

ArrayPriorityQueue::ArrayPriorityQueue() {
    myElements = new PQEntry[100]();    // creating a new array
    mySize = 0;                         // There's no element in it yet
    myCapacity = 100;                   // Picking a capacity to begin with
}

ArrayPriorityQueue::~ArrayPriorityQueue() {
    delete[] myElements;                // in order not to leak memory.
}

/* This function is to find the index of a certain value (Tjhe 1st one, if multiple exist)
 */
int ArrayPriorityQueue::findIndexbyValue(string value) const{
    if (mySize != 0)
        for (int i = 0; i < mySize; i++)
            if (myElements[i].value == value)
                return i;
    throw "The desired value cannot be found";  // in  case of not finding the value or empty queue.
}

/* It uses the findIndexbyValue() function to find the element and chenges the priority
 * in case the new priority is more urgent than the old one. */
void ArrayPriorityQueue::changePriority(string value, int newPriority) {
    int index = findIndexbyValue(value);
    int currentPriority = myElements[index].priority ;
    // I could use the operators define for PQEntry too, but I prefered to write the code myself.
    if (currentPriority < newPriority)
        throw "The new priority is less urgent than the old one";
    else
        myElements[index].priority = newPriority;   // Substituting the new priority.
}

/* Clears the whole array. */
void ArrayPriorityQueue::clear() {
    mySize = 0;                         // empties the array.
}

/* This function finds the most urgent element's index */
int ArrayPriorityQueue::findIndexUrgent() const {
    if (mySize == 0)
        throw "The queue is Empty";
    int maxPriority = myElements[0].priority;
    int indexUrgent = 0;
    for (int i = 0; i < mySize; i++) {
        if (myElements[i].priority < maxPriority ||         // if it's more urgent  OR, equally urgent &
                (myElements[i].priority == maxPriority &&   //smaller in case of string comparison
                 myElements[i].value <= myElements[indexUrgent].value)) {
            indexUrgent = i;
            maxPriority = myElements[i].priority;
        }
    }
    return indexUrgent;
}

/* This function uses findIndexUrgent() to spot the most urgent element and return its value. */
string ArrayPriorityQueue::dequeue() {
    int indexUrgent = findIndexUrgent();
    PQEntry elementUrgent = myElements[indexUrgent];
    for (int i = indexUrgent; i < mySize; i++) {
        myElements[i] = myElements[i + 1];      // Removing the most urgent element and filling the
    }                                           // empty space by moving elements backward
    mySize--;                                   // decreasing size.
    return elementUrgent.value;     // Returning the most urgent value
}

/* Inserting new element in the queue. */
void ArrayPriorityQueue::enqueue(string value, int priority) {
    checkResize();      // insering new element at the end of the queue regardless of priority
    myElements[mySize] = PQEntry(value, priority);
    mySize++;           // increasing size
}
/* returns true if it's empty and false otherwise. */
bool ArrayPriorityQueue::isEmpty() const {
    return mySize == 0;     // checks if the size is zero.
}

/* Uses function findIndexUrgent() to find the urgent index and return its value */
string ArrayPriorityQueue::peek() const {
    return myElements[findIndexUrgent()].value;
}

/* Uses function findIndexUrgent() to find the urgent index and return it's priority. */
int ArrayPriorityQueue::peekPriority() const {
    int indexUrgent = findIndexUrgent();
    return myElements[indexUrgent].priority;
}

/* returns the size */
int ArrayPriorityQueue::size() const {
    return mySize;
}

/* Prints out the array in a nice way, showing the priority of each string as well. */
ostream& operator<<(ostream& out, const ArrayPriorityQueue& queue) {
    out << "{";
    if (!queue.isEmpty()) {         // I use the prinitng operator from PQEntry.h for PQEntry elemns.
        out << queue.myElements[0]; // In order to print nicely, I separated this element.
        for (int i = 1; i < queue.size(); i++)
            out << ", " << queue.myElements[i];
    }
    out << "}";
    return out;
}

/* In order to avoid crashing the program because of reaching the end of array, we create a
 * bigger array with twice capacity of the original array as soon as we reach the end of array
 * and replace the old array with the new one.
 */
void ArrayPriorityQueue::checkResize() {
    if (mySize == myCapacity) {
        // copy all elements into a bigger array
        PQEntry* bigger = new PQEntry[2 * myCapacity];
        for (int i = 0; i < mySize; i++) {
            bigger[i] = myElements[i];
        }
        // swap in the new bigger array for the old one
        delete[] myElements;
        myElements = bigger;
        myCapacity *= 2;
    }
}
