/* CS 106B 2nd HW: Word Ladder
 * Ali Parsaei (aparsaei)  ID: 05950982
 *
 * A word ladder is a connection from one word to another formed by changing one letter
 * at a time with the constraint that at each step the sequence of letters still forms
 * a valid word. For example, here is a word ladder connecting the word "code" to the
 * word "data" . Each changed letter is underlined as an illustration:
        code -> cade -> cate -> date -> data
 * There are many other word ladders that connect these two words, but this one is the
 * shortest. That is, there might be others of the same length, but none with fewer steps
 * than this one.
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "lexicon.h"
#include "queue.h"
#include "hashset.h"
#include "stack.h"
#include "error.h"

using namespace std;

//fucntion prototypes:
void welcome ();
Lexicon getDictionary ();
void getWords(string& word1, string& word2);
void printStack (Stack<string> stackFile);
void buildLadder (string word1, string word2, Lexicon dict);

int main() {
    Lexicon Dict; // the file that the dictionary will be copied to
    string w1;  // word 1
    string w2;  // word 2

    welcome ();
    Dict = getDictionary ();

    while (true) {          // Constantly asking the user to insert words
        getWords(w1,w2);    // until they insert 2 valid words
        if (w1=="" || w2=="") {
            break; // If user enters "", it exits the loop.
        }
        if (w1.length() != w2.length()) {
            cout<<"The two words must be the same length."<<endl;
        } else if (w1 == w2) {
            cout<<"The two words must be different."<<endl;
        } else if (!Dict.contains(w1) || !Dict.contains(w2)) {
            cout<<"The two words must be found in the dictionary"<<endl;
        } else {
            buildLadder(w1,w2,Dict);
        }
    }

    cout << "Have a nice day." << endl;
    return 0;
}


/* prints out the welcome message */
void welcome(){
    cout<<"Welcome to CS 106B Word Ladder"<<endl<<
          "Please give me two English words, and I will change the"<<endl<<
          "first into the second by changing one letter at a time."<<endl<<endl;
}


/* This function prompts for valid dictionary and returns the
 * lexicon file including all words of the dictionary. */
Lexicon getDictionary () {

    ifstream dictName;
    promptUserForFile(dictName, "Dictionary file name?");
    cout<<endl;
    Lexicon dictionary;
    dictionary.addWordsFromFile(dictName);
    return dictionary; // Dictionary is returned as a Lexicon file
}

/* getWords prompts user for 2 words and converts them to lowercase. If
 *  the user press Enter without typing a word, it will quit the program. */
void getWords(string& word1, string& word2) {
    word1 = toLowerCase(getLine("Word #1 (or Enter to quit): "));
    if (word1 == "") {
        return; // in case w1="", it jumps out of function.
    }
    word2 = toLowerCase(getLine("Word #2 (or Enter to quit): "));
    // I did not use the if-statement here, cause, I will use "break" in
    //the body of main function
}

/* This function uses the algorithm in the spec.pdf to find the
 * possible next words for each partial ladder. */
string findNeighbor (string& lastWord, int i, char letter) {
    string newWord = lastWord;
    newWord[i]=letter;
    return newWord;
}


/* This function checks if the neighbors found are valid or not.
 * they must be in the dictionary. BTW, they must not be already used. */
bool wordAllowed (string word, Lexicon& dict, HashSet<string>& wordSet){
    if (!wordSet.contains(word) && dict.contains(word)) {
        wordSet.add(word);
        return true;        // if the word is in Dict and also not used beforehand,
          // it will know it as valid and also add it to the wordSet
    }
    return false;
}


/* This function tries to form a queue of stacks (parial paths)
 * As soon as it reaches the path, it will turn it in.
 *  The algorithm makes sure that the solution is the shortest path */
void buildLadder (string word1, string word2, Lexicon dict){
    Queue<Stack<string> > ladderQue; // Que including all incomplete ladders
    Stack<string> baseLadder;
    baseLadder.push (word1);
    ladderQue.add(baseLadder);
    HashSet<string> wordSet;  // I used HashSet ,since it's faster.
    string neighbor; // The possible next word in a ladder
    string alphabet = "abcdefghijklmnopqrstuvwxyz";

    while (!ladderQue.isEmpty()){
        int size = word1.length();
        Stack<string> currentLadder = ladderQue.dequeue();
        string lastWord = currentLadder.peek(); // this copies the last word of the partial path to lastWord

        for (int i = 0; i < size; i++) { // forming a loop like the algorithm in specs.pdf
            for (int j = 0; j < alphabet.length(); j++) {
                neighbor= findNeighbor(lastWord, i, alphabet[j]);
                 //Previous line replaces the i'th element with j'th letter of alhabet

                if(wordAllowed(neighbor, dict, wordSet)) { // checks validity of the generated word

                    if (neighbor == word2){   // if neighbor = w2 -> we've reached the goal
                        currentLadder.push(neighbor); // completing the ladder
                        cout <<"A ladder from " << word2<<" back to "<<word1<<": "<<endl;
                        printStack(currentLadder); // printing it out
                        return;
                    } else { // Otherwise, we have to add it to a copy of partial ladder
                        Stack<string> copy = currentLadder;
                        copy.push(neighbor);
                        ladderQue.enqueue(copy); // add the copy to the end of the queue
                    }

                }

            }
        }

    }

    if (neighbor != word2) {
        cout <<"No word ladder found from " << word2<<" back to "<<word1<<endl<<endl;
    }
}


/* This function is to print the ladder nicely. */
void printStack (Stack<string> stackFile) {
    while (!stackFile.isEmpty()) {
        cout << stackFile.pop()<< " ";
    }
    cout<<endl<<endl;
}
