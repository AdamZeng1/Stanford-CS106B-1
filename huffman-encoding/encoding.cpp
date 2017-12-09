/* Ali Parsaei (aparsaei) ID:  05950982
 * CS 106B 6th HW Assignment: Huffman Code
 * In this project, we learn how to compress a file to .huf compressed file
 * and also how to decompres a .huf file into afile format readable by user.
 * Sources:
 *  Class Head TA ( very minor issues resolved in OH )
 */

#include "encoding.h"
#include <iostream>
#include "bitstream.h"
#include "filelib.h"
#include "istream"
#include "ostream"
#include "pqueue.h"
#include "filelib.h"

/* This function reads an input and returns the map of char's used in the file
 * and the corresponding occurrence of each char.
 */
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    freqTable[PSEUDO_EOF] = 1; // Adding PSEUDO_EOF to the map.
    int letter;
    while (input) { // While not reached the end of the file, add to map.
        letter = input.get();
        if (letter == -1)
            break;
        freqTable[letter] += 1;
    }
    return freqTable;
}

/* This function uses the table built by buildFrequencyTable() to form an
 * encoding tree to use in next steps. */
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> encodingQueue; // forming a P-queue of HuffmanNode*
    for (int key: freqTable) {
        HuffmanNode* node = new HuffmanNode(key,freqTable[key]);
        encodingQueue.enqueue(node, node->count); // Enqueuing the HuffmanNode* with
    }                                             // priority = its occurrence.

    if (encodingQueue.size() == 1) {
        HuffmanNode* parent = encodingQueue.dequeue();
        return parent;
    } else {
        HuffmanNode* parent;    // This is what will be returned as output
        while (encodingQueue.size() > 1) {  // While there're more than a single node in line
            HuffmanNode* node1 = encodingQueue.dequeue();   // Dequeue 2 successive nodes
            HuffmanNode* node2 = encodingQueue.dequeue();
            parent = new HuffmanNode(NOT_A_CHAR, node1->count + node2->count, node1, node2);
            encodingQueue.enqueue(parent, parent->count);   // form a new node and enqueue it.
        }
        return parent;
    }
}

/* This Helper function is to help buildEncodingMap() by a recursive backtracking path.*/
void encodingMapHelper(HuffmanNode* node, Map<int, string>& encodingMap, string str) {
    if (node == NULL) {
        return;
    } else if (node->isLeaf()) {
        encodingMap[node->character] = str; // if it's a leaf add the key/value to map
    } else {    // Otherwise
        str.append("0");    // go left and add "0" to the string representing value
        encodingMapHelper(node->zero, encodingMap, str);
        str = str.substr(0,str.length()-1); // remove "0" from the string and go right
        str.append("1");    // Then add "1" to the string rpresenting the value.
        encodingMapHelper(node->one, encodingMap, str);
    }
}

/* This function returns the mapping from each char to its binary equivalent string. */
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    if (encodingTree != NULL) {
        string emptyStr = "";    // Running the helper function for the root of encoding tree
        encodingMapHelper(encodingTree, encodingMap, emptyStr);           // and empty string.
    }
    return encodingMap;
}

/* This function writes the binary equivalent of the file in the output of type obitstream& */
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    char letter;
    while (input.get(letter)) {                 // While there's a letter to read, find the
        string binaryEq = encodingMap[letter];  // equivalent binary code (in string format)
        for (char ch: binaryEq) {               // and convert it to "bit" by "bit" output.
            if (ch == '0')
                output.writeBit(0);
            else if (ch == '1')
                output.writeBit(1);
        }
    }

    string binaryEOF = encodingMap[PSEUDO_EOF]; // Add the EOF bit by bit binary equivalent
    for (char ch: binaryEOF) {                  // to the output to indicate the end of file.
        if (ch == '0')
            output.writeBit(0);
        else if (ch == '1')
            output.writeBit(1);
    }
}

/* This helper function is used to execute recursion in decodeData(); it puts the
 * leaf of the tree in the output. */
void decodeDataHelper(ibitstream& input, HuffmanNode* node, ostream& output) {
    if (node == NULL)
        return;
    if (node->isLeaf() && node->character != PSEUDO_EOF) {
        output.put(node->character);    // if it's a leaf, put it in the output
        return;
    } else {    // otherwise, go ahead and try the subtrees
        int digit = input.readBit();
        if (digit == -1)
            return;
        if (digit == 0)     // if the next digit is 0, try left subtree
            decodeDataHelper(input, node->zero, output);
        else                // if the next digit is 1, try right subtree
            decodeDataHelper(input, node->one, output);
    }
}

/* This function decodes the encoded binary data and puts them in output. */
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {

    while (true) {
       int digit = input.readBit();    // getting the input's first digit
        if(digit == -1)             // if it reaches the EOF, it terminates the function
            break;
        if (digit == 0)             // otherwise, if digit == 0, go to left subtree
            decodeDataHelper(input, encodingTree->zero, output);
        else                        // if digit == 1, go to right subtree
            decodeDataHelper(input, encodingTree->one, output);
    }
}

/* This function compresses a file into .huf format. In this function we call the previous
 * functions in the same order created. */
void compress(istream& input, obitstream& output) {
    Map<int, int> freqTable;
    freqTable = buildFrequencyTable(input);
    HuffmanNode* encodingTree;
    encodingTree = buildEncodingTree(freqTable);
    Map<int, string> encodingMap;
    encodingMap = buildEncodingMap(encodingTree);
    output<<freqTable;
    rewindStream(input);
    encodeData(input, encodingMap, output);
    freeTree(encodingTree);
}

/* This function extracts a compressed file. We use freqTable and huffman tree to decode data.*/
void decompress(ibitstream& input, ostream& output) {
    Map<int, int> freqTable;
    input>>freqTable;
    HuffmanNode* encodingTree;
    encodingTree = buildEncodingTree(freqTable);
    decodeData(input, encodingTree, output);
    freeTree(encodingTree);
}

/* this function frees the allocated memory to HuffmanTree after the program ends. */
void freeTree(HuffmanNode* node) {
    if (node == NULL)   // Do nothing in this case. (BASE CASE)
        return;
    else {
        freeTree(node->zero);   // Explore the left subtree
        freeTree(node->one);    // Explore the right subtree
        delete node;    // Delete the node.
    }
}
