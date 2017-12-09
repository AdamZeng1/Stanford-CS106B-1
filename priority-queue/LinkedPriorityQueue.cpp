/* CS 106B Stanford/    Summer 2015
 * Ali Parsaei          ID: 05950982
 * Sources:
 *  LinkedList.cpp from CS 106B Lectures
 *
 */

#include <iostream>
#include <string>
#include "LinkedPriorityQueue.h"
#include "ListNode.h"

/* Ititializes the class as the objectis made. */
LinkedPriorityQueue::LinkedPriorityQueue() {
    front = NULL;   // There's nothing in the queue yet.
}

/* Frees the dynamic memory allocated to the object. */
LinkedPriorityQueue::~LinkedPriorityQueue() {
    clear();
}

/* This function removes a specific ListNode; Used inside changePriority. */
void LinkedPriorityQueue::remove(string value) {
    ListNode* trash;
    if (front->value == value) {
        trash = front;  // removing the first element.
        front = front->next;
    } else {
        ListNode* current = front;
        while (current->next->value != value) {
            current = current->next;
        }
        trash = current->next;
        current->next = current->next->next;
    }
    delete trash;
}

/* This function changes the priority of a chosen element to the new one in case the new
 * priority is more urgent than the old one. */
void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    ListNode* current = front;
    while (current != NULL) {
        if (current->value == value) {
            if (current->priority < newPriority) {
                throw "The new priority is less urgent than the old one";
            } else {                                
                remove(current->value);
                break;
            }
        }
        current = current->next;
    }
    enqueue(value, newPriority);
}

/* Deletes all elements in the PQueue */
void LinkedPriorityQueue::clear() {
    while (front != NULL) {
        ListNode* temp = front->next;
        delete front;   // front is deleted
        front = temp;   // front is moved ahead one by one.
    }
    front = NULL;       // finally, front is null.
}

/* Returns the most urgent element's value in the Pque in case it's not empty. */
string LinkedPriorityQueue::dequeue() {
    if (front != NULL) {
        ListNode* firstElmnt = front;
        front = front->next;
        string firstElmntValue = firstElmnt->value;
        delete firstElmnt;
        return firstElmntValue;
    }
    else
        throw "The queue is Empty";
}

/* This function inserts the new element in order among other elements. */
void LinkedPriorityQueue::enqueue(string value, int priority) {
    ListNode* newElmnt = new ListNode(value, priority);
    if (front == NULL) {
        front = newElmnt;
        return;
    } else if (front->priority > priority ||
               ((front->priority == priority) && front->value > value)) {
        newElmnt->next = front;
        front = newElmnt;
        return;
    } else {
        ListNode* current = front;
        while (current->next != NULL) {
            if (current->next->priority > priority ||
                    ((current->next->priority == priority) && current->next->value > value)) {
                newElmnt->next = current->next;
                current->next = newElmnt;
                return;
            }
            current = current->next;
        }
        current->next = newElmnt;
    }
}

/* returns true if the Pqueue is empty */
bool LinkedPriorityQueue::isEmpty() const {
    return front == NULL;
}

/* returns the value of the most urgent element */
string LinkedPriorityQueue::peek() const {
    if (front != NULL)
        return front->value;
    else
        throw "The Queue does not contain any elements.";
}

/* returns the priority of the most urgent element */
int LinkedPriorityQueue::peekPriority() const {
    if (front != NULL)
        return front->priority;
    else
        throw "The Queue does not contain any elements.";
}

/* This function returns the size of PQueue */
int LinkedPriorityQueue::size() const {
    int count = 0;
    ListNode* current = front;
    while (current != NULL) {
        count += 1;
        current = current->next;
    }
    return count;
}
/* This operator prints the list of elements in the queue and shows their priorities in front
 * of each of elements. */
ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    out << "{";
    ListNode* current = queue.front;
    if (current != NULL) {
        out << *current;
        while (current->next != NULL) {
            current = current->next;
            out << ", " << *current;
        }
    }
    out << "}";
    return out;
}
