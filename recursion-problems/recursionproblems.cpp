/* CS 106B 3rd Assignment.
 * Ali Parsaei (aparsaei)   ID: 05950982
 *
 * In this assignment, we're supposed to write 6-7 recursive functions.
 * I have used helper functions for some of the functions. I've included
 * their prototypes above the function in which the helper function is used.
 */

#include "recursionproblems.h"
#include <cmath>
#include <iostream>
#include "hashmap.h"
#include "map.h"
#include "random.h"
#include <string>       // I added this for convertStringToInteger
#include <fstream>      // added for grammarGenerate
#include "strlib.h"     // added for grammarGenerate

using namespace std;

// skipped the optional one.
int countKarelPaths(int street, int avenue) {
    // TODO: write this function
    return 0;
}

/* This function takes a string of integers and converts them
 * to ineteger data type by recursion
 */
int convertStringToInteger(string exp) {
    if (exp == "") { // if input is empty, returns 0
        return 0;
    } else if (exp[0] == '-') { // if input < 0, it returns -(+ part) of the input
        return - convertStringToInteger(exp.erase(0,1));
    // within the following statement, valid characters are set.
    } else if (exp[exp.size()-1]== '0'|| exp[exp.size()-1]== '1'|| exp[exp.size()-1]== '2'||
               exp[exp.size()-1]== '3'|| exp[exp.size()-1]== '4'|| exp[exp.size()-1]== '5'||
               exp[exp.size()-1]== '6'|| exp[exp.size()-1]== '7'|| exp[exp.size()-1]== '8'||
               exp[exp.size()-1]== '9')
    {
        return (exp[exp.size()-1] - '0') +  // taking out the last digit and recursing over
                10*convertStringToInteger(exp.erase(exp.size()-1,1)); // the rest of the number
    } else {    // in case of non-ineteger input, it throws an expression.
        throw "Invalid inetger string! ";
    }
}

/* isBalanced() function checks a valid expression for being
 * balanced in terms of (), <>, {}, and []. for eg. {()[<>]}
 * is balanced while [[()] or {(}) are unbalanced.
 */
bool isBalanced(string exp) {
    if (exp == "") { // This is the BASE CASE. an empty string is balanced.
        return true;
    } else { // procedure: removing balanced adjacent pairs from string up until it's empty or unbalanced
        int indexPar  = exp.rfind('('); // CASE "()"
        if (indexPar != string::npos) { // finds the last '(' in the expression (if existing)
            if (indexPar < exp.size()-1 && exp [indexPar + 1] == ')') {
            // if the following character is ')', it removes () out of expression
                return isBalanced(exp.erase(indexPar, 2)); // Recursive step
            }
        }

        int indexCurl = exp.rfind('{'); // Doing what done in CASE "()" for CASE "{}"
        if (indexCurl != string::npos) {
            if (indexCurl < exp.size()-1 && exp [indexCurl + 1] == '}') {
                return isBalanced(exp.erase(indexCurl, 2));
            }
        }

        int indexBrac = exp.rfind('['); // Doing what done in CASE "()" for CASE "[]"
        if (indexBrac != string::npos) {
            if (indexBrac < exp.size()-1 && exp [indexBrac + 1] == ']') {
                return isBalanced(exp.erase(indexBrac, 2));
            }
        }

        int indexSmal = exp.rfind('<'); // Doing what done in CASE "()" for CASE "<>"
        if (indexSmal != string::npos) {
            if (indexSmal < exp.size()-1 && exp [indexSmal + 1] == '>') {
                return isBalanced(exp.erase(indexSmal, 2));
            }
        }
    }

    return false; // in case the final string is not empty, it's not balanced
}

/* In the following function, it calculates the weights on each person's knees
 * in  a human pyramid by using recursion,
 */
double weightOnKnees(int row, int col, Vector<Vector<double> >& weights) {

    if (row == 0 && col == 0) { // (BASE CASE): The weight on the person on top of
        return weights[0][0];  // the pyramid is exactly his/her own weight.
    } else if (col >=1 && col< weights[row-1].size())
    { // Recursive step for non-corner members (Exactly applying the rule in specs.PDF)
        return weights[row][col] + 0.5*( weightOnKnees(row-1, col-1, weights) +
                                         weightOnKnees(row-1, col, weights));
    } else if (col == 0 || col == weights[row-1].size())
    { // Recursive step for members at corners of pyramid
        return weights[row][col] + 0.5* weightOnKnees(row-1, max(0,col-1), weights);
    }
    return 0.0;
}

/* drawSierpinskiTriangle() drwas a order "order" Sierpinski triangle
 * by recursion. The base case is order = 0, where the function does nothing.
 * in recursive step, it draws 3 lines and find 3 points and calls the function
 * 3 times. up until order == 0.
 */
void drawSierpinskiTriangle(GWindow& gw, double x, double y, int size, int order) {
    if (x < 0 || y < 0 || order < 0 || size < 0) { // exceptions
        throw "Invalid parameters passes! ";
    } else if (order == 0) {
        gw.drawLine(x, y, x+size, y); // drawing the horizontal line
        gw.drawLine(x+size, y, x+0.5*size, y+0.5*sqrt(3)*size); // drwas the right vertex
        gw.drawLine(x+0.5*size, y+0.5*sqrt(3)*size, x, y); // drwas the left vertex
    } else {
        size = size / 2; // transition to next level (smaller triangles)
        // Now, we call the recursive functions 3 times to dorm 3 triangles for each triangle.
        drawSierpinskiTriangle(gw, x, y, size, order-1);
        drawSierpinskiTriangle(gw, x + size, y, size, order-1);
        drawSierpinskiTriangle(gw, x+1*size/2, y+ sqrt(3)*size/2, size,order-1);
    }
}


// Prototype for helper function for floodFill()
int floodFillHelper(GBufferedImage& image, int x, int y, int color,
                     int originalColor, int& counter);

/* floodFill function tries to change the color of a block to a chosen
 * color. if the color chosen is the set is the same as the riginal color,
 * it won't change. if the chosen pixel on secreen is not in bounds, it will
 * trow an exception. Otherwise, it will turn in the number of pixels which
 * changed colors.
 */
int floodFill(GBufferedImage& image, int x, int y, int color) {
    if (x < 0 || y < 0 || !image.inBounds(x,y)) { // throw an exception
        throw "Invalid Parameters! ";             // if invalid pixel is chosen
    }
    int counter = 0;    // the number of pixels changed
    if (image.getRGB(x,y)== color) { // if the color to be set is the same
        return counter;             // as the original color, counter =0
    }
    int originalColor = image.getRGB(x,y); // getting the pixel color
    return floodFillHelper(image, x, y, color, originalColor, counter); // recursion
}

/* this helper function is for floodFill(). it will check the color of
 * a pixel, in case it is not the current color, it will change the
 * color of the pixel and iterate it for all 4 directions.
 */
int floodFillHelper(GBufferedImage& image, int x, int y, int color,
                     int originalColor, int& counter) {
    if (image.getRGB(x,y) != originalColor || !image.inBounds(x,y)) {
        return counter; // if it reches the boundary of the block or out of the
    }                   // image boundary, it will return the value.
    if (image.getRGB(x,y) != color) {
        image.setRGB(x,y, color); // each time setting a pixel color, it will add
        counter ++;               // to counter
    }
    // Now, doing 4 recursion for 4 directions
    if (image.inBounds(x+1,y)) { // going right and doing the function
        floodFillHelper(image, x+1,y,color,originalColor, counter);
    }
    if (image.inBounds(x-1,y)) { // going left and doing the function
        floodFillHelper(image, x-1,y,color,originalColor, counter);
    }
    if (image.inBounds(x,y+1)) { // going up and doing the function
        floodFillHelper(image, x,y+1,color,originalColor, counter);
    }
    if (image.inBounds(x,y-1)) { // going down and doing the function
        floodFillHelper(image, x,y-1,color,originalColor, counter);
    }

    return counter; // finally, returns the number of pixels changed color
}


// Function prootypes for grammarGenerate
Map<string, Vector<string> > readInput(istream& input);
string elementExpand(Map<string, Vector<string> >& grammarMap, string symbol,
                             string outputStr);

/* grammarGenerate() generates a random grammar structure inserted by user (symbol)
 * by using the file including the grammar non-terminals and rules (input).
 * the random structure is made for "times" # of times given by user.
 */
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    Vector<string> v; // The output is a Vector string, including "times" strings
    string outputStr; // output string of elementExpand function defined here.
    Map<string, Vector<string> > grammarMap = readInput(input);
    if (symbol == "") {     // If the input is empty, it throws an expression
        throw "Empty symbol is not alowed";
    } else if (times <= 0) { // if times <=0, it returns empty string as output vector.
        v.add("");
    } else {    // Otherwise, it generates random structures for "times" # of times.
        for (int i = 0; i < times; i++) {
            v += elementExpand(grammarMap, symbol,outputStr);
        }
    }
    return v;   // The output incluses "times" random structures of type of the symbol.
}

/* readInput function reads the input file and stores them into
 * a map, of which each key is the non-terminal and values are
 * the rules for each non-terminal.
*/
Map<string, Vector<string> > readInput(istream& input) {
    string line;
    Map<string, Vector<string> > grammarMap;

    while (getline(input,line)) { // reading the file line-by-line
        // lineSplit splits the line into a non-ternimal and its rules
        Vector<string> lineSplit = stringSplit(trim(line), "::=");
        // ruleSplit splits each non-terminal's rules into individual rules
        Vector<string> rulesSplit = stringSplit(trim(lineSplit[1]), "|");
        if (grammarMap.containsKey(lineSplit[0])) {
            throw "More than one line for the same non-terminal! ";
        } else {
            grammarMap[lineSplit[0]] = rulesSplit; // Forming the grammar map
        }
    }
    return grammarMap; // The output of function
}

/* elementExpand function utilizes the map provided by readInput
 * to expand the non-terminal by picking random options
 * If symbol != non-terminal, it returns symbol. (Base case)
 * Otherwise, it repeats the function until reaches a terminal (Recursion).
 */
string elementExpand(Map<string, Vector<string> >& grammarMap, string symbol,
                             string outputStr) {
    if (!grammarMap.containsKey(symbol)) {  // (BASE CASE)
        outputStr += " ";   // If symbol = terminal, it'll be added to the output string.
        outputStr.append(trim(symbol)); // " " is added to seperate words
    } else // (RECURSION)
    { // If symbol is a non-terminal, it will pick a random rule from the vector
        Vector<string> choseRules = grammarMap[symbol]; // of rules in grammarMap
        int randIndex = randomInteger(0,grammarMap[symbol].size()-1);
        // following line seperates each rule into smaller parts
        Vector<string> splitRule = stringSplit(choseRules[randIndex], " ");
        for (string newSym : splitRule) { // Looping over all elements of the chosen rule:
            outputStr = elementExpand(grammarMap, newSym, outputStr);
        }   // Recursively repeating the process untill it reaches a terminal
    }
    return outputStr; // Output of the function
}
