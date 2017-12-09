/* CS 106B 2nd HW: N-Gram
 * Ali Parsaei (aparsaei)  ID: 05950982
 *
 * This program will read an input file and uses it to build
 * a large data structure of word groups called "N-grams" as
 * a basis for randomly generating new text that sounds like
 * it came from the same author as that file.
 *
 * In 1st part of program, we construct a map, of which keys are
 * N-1 consecutive words in the original text and the values are
 * possible Nth word. Then, we generate the random text by using
 * the map. Actually, we choose a starting point (window = N-1 words)
 * and then choose the Nth word randomly based on the map and the
 * # of repetition of each possible Nth word. Then, we move the
 * window to the last N-1 words of the curent output and so on.
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "vector.h"
#include "map.h"
#include "filelib.h"
#include "simpio.h"
#include "random.h"

using namespace std;

// Function prototypes:
void welcome ();
void makeList(int& N, Vector<string>& allWords);
void buildMap (int& N, Vector<string>& allWords,
               Map<Vector<string>,Vector<string> >& wordCol);
void generateText (int& N, Map<Vector<string>,Vector<string> >& wordCol,
                   Vector<string>& output, int wordNum);
void printStr (Vector<string> fileStr);
int getNumber (int N) ;


int main() {
    int N;
    Vector<string> allWords; // all words copied from text file to Vector
    Map<Vector<string>,Vector<string> > wordCol;        // The map formed similar to the
    Vector<string> output;// output text in vector      //assignment guidelines

    welcome ();
    makeList(N, allWords);
    buildMap (N, allWords, wordCol);
    int wordNum =1 ;
    while (wordNum !=0) { // This loop prompts for the # of words
        wordNum = getNumber(N);
        if (wordNum ==0) { // breaks the loop in case user inserts 0
            break;
        }
        generateText (N, wordCol,output, wordNum);
        printStr (output);
        output.clear();
    }

    cout << "Exiting." << endl;
    return 0;
}


/* This function prints out the welcome message */
void welcome () {
   cout<<"Welcome to CS 106B Random Writer ('N-Grams')."<< endl
         <<"This program makes random text based on a document."<<endl
         <<"Give me an input file and an 'N' value for groups"<<endl
         <<"of words, and I'll create random text for you."<<endl<<endl;
}


/* This function provides us with a Vector, including all
 * words of the text file in the original order. */
void makeList(int& N, Vector<string>& allWords) {
     ifstream fileName;
     string word;
     promptUserForFile(fileName, "Input file name? ");
     N = 0;
     while (N < 2) { // prompting for valid N > 1
         N = getInteger("Value of N? ");
         if (N<2) {
             cout<<"N must be 2 or greater." << endl;
         }
     }
     cout<<endl;

     while (fileName >> word) { // copying all words into the vector allwords
         allWords.add(word);
     }
}


/* builMap function gets N, and the Vector, including the text file
 * words in order, and builds a map from N-1 words to the Nth word.
 * the map s the output of this function */
void buildMap (int& N, Vector<string>& allWords,
               Map<Vector<string>,Vector<string> >& wordCol) {

    int fileSize = allWords.size();
    Vector<string> keyVec;  // the vector including the possible windows
    string valStr;  // the vector including possible following word
    // Next for loop goes over each element of the vector,
    for (int i = 0; i < fileSize; i++){  // including all words of the textfile in order.
        for (int k = 0; k < N-1; k++) {  // This loop picks N-1 words as window

            if (i+k < fileSize) {        // The  if-else statement is for indexing issues .
                keyVec.add(allWords[i+k]);
            } else {    // if i+k >filesize, the word will be read from the beginning of text
                keyVec.add(allWords[i+k-fileSize]); // constructing the window by adding elements
            }

        }

        if (i+N <= fileSize) {// the value for key (window) is the word after the last word
            valStr = allWords[i+N-1];
        } else {
            valStr = allWords[i+N-fileSize -1];
        }

        wordCol[keyVec] += valStr;  // forming the map pair
        keyVec.clear();             //clearing the window for next window
    }
}


/* This function is only to conviniently get the # of words to generate the text */
int getNumber (int N) {
    int wordNum = 1;

    while (wordNum < N && wordNum !=0) {
        wordNum = getInteger("# of random words to generate (0 to quit)? ");
        if (wordNum < N && wordNum !=0) {
            cout<<"Must be at least "<<N<<" words. " <<endl<<endl;
        }
    }

    return wordNum;
}

/* This function generates the new text. (The output is Vector, output.) */
void generateText (int& N, Map<Vector<string>,Vector<string> >& wordCol,
                   Vector<string>& output, int wordNum) {

    // Following lines will produce random starting point
    int mapSize = wordCol.size();
    int randIndex = randomInteger(0, mapSize-1); // Generating valid random index
    Vector<Vector<string> > mapKey = wordCol.keys(); // all keys to use in the future
    Vector<string> window;  //initializing the window by using the random index
    window = mapKey[randIndex];
    output = window;       //initializing the output

    for (int i = 0; i< wordNum - N + 1; i++) {
        int valSize = wordCol[window].size(); // generating valid random index
        int randIndex2 = randomInteger(0, valSize-1);
        Vector<string> wordVec = wordCol[window]; //wordVec: all words in the vector
        Vector<string> wordChose;       //corresponding to window and wordChose, the
        wordChose += wordVec[randIndex2]; // random word chosen out of that vector
        output += wordChose;    // updating the output
        window.clear();         // clearing window and Nth word for the future steps
        wordChose.clear();
        int outSize = output.size();

        for (int j = outSize - N + 1 ; j < outSize; j++) {
            window += output[j]; // forming the new Window = last (N-1)th words of output
        }

    }
}


/* printStr is only for printing the output vector nicely */
void printStr (Vector<string> fileStr) {
    cout<<"... ";
    for (int n = 0; n < fileStr.size(); n++) {
        cout << fileStr[n] << " " ;
    }
    cout<<"..."<<endl<<endl;
}
