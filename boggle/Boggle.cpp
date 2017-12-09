/* Stanford CS 106B 4th HW       Summer 2015
 * Ali Parsaei                   ID: 05950982
 * Class Boggle, cpp file
 * Class Boggle provides us with a collection of functions for Boggle object, which
 * is used in Boggleplay.cpp to play the game, BOGGLE with computer.
 * Sources: A friend of mine helped me out in some functions.
 */

#include "Boggle.h"
#include "lexicon.h"
#include "grid.h"
#include "set.h"
#include <string>
#include "bogglegui.h"
#include "shuffle.h"
#include "random.h"

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

int Boggle::ROW_MAX = 4;
int Boggle::COL_MAX = 4;

/* Boggle::Boggle is the constructor of this class. as the Boggle object is made, it runs, fist,
 * it makes a copy of dictionary, to be available for all class functions. Then initialize the
 * GUI board and fill the board elements with either user input or random elements from CUBES.
 */
Boggle::Boggle(Lexicon& dictionary, string boardText) {
    Dict = dictionary;
    board.resize(ROW_MAX,COL_MAX);
    //BoggleGUI::initialize(getBoardLength(), getBoardLength());       // Initializes the board
    BoggleGUI::initialize(ROW_MAX,COL_MAX);             // Initializes the board
    if (!boardText.empty()) {                           // User's string input
        for (int i = 0; i < ROW_MAX; i++)
            for (int j = 0; j < COL_MAX; j++) {
                board[i][j] = toupper(boardText[0]);
                BoggleGUI::labelCube(i,j,board[i][j]);  // Labeling each element of the GUI board
                boardText = boardText.substr(1);        // Moving forward in user's input to complete the board
            }
    } else {
        shuffle(CUBES,16);                 // Shuffling the cubes in order to add Random cubes to the board grid
        for (int i = 0; i < ROW_MAX; i++)
            for (int j = 0; j < COL_MAX; j++) {
                string cubesElem = CUBES[4*i + j];     // Indexing in order to cover all CUBES
                int cubesELemSize= 6;   // Using randomInteger() to pick a random side of the chosen cube
                board[i][j] = cubesElem[randomInteger(0, cubesELemSize - 1)];
                BoggleGUI::labelCube(i,j,board[i][j]); // Labeling each element of the GUI board
            }
    }

}

/* getLetter returns the character that is stored in Boggle board at the given 0-based row and column.
 * If the row and/or column are out of bounds, you should throw an int exception */
char Boggle::getLetter(int row, int col) const {
    if (!board.inBounds(row,col))
        throw "out of boaunds indices";
    else
        return board[row][col];
}

/*  checkWord checks whether the given word string is suitable to search for: that is, whether it is
 * in the dictionary, long enough to be a valid Boggle word, and has not already been used. */
bool Boggle::checkWord(string word) {
    if (word.length() < 4)
        return false;
    else if (humanWordFound.contains(toUpperCase(word)))
        return false;
    else if (Dict.contains(toLowerCase(word)))
        return true;
    else
        return false;
}

/* This Helper function checks if a word caould be formed on the board starting from a certain point,
 * by using recursive backtracking. In order to keep track of marked elements on the board, we define
 * a private Grid of bools,representing the condition of each element. if the elemnt is been visited
 * before, it's true, ow, false. */
bool Boggle::humanWordSearchHelper(int row, int col, string word, string& soFar, Grid<bool>& marked){
    if (!board.inBounds(row,col) || marked[row][col])
        return false;   // BASE CASE: if the board element is marked already or out of bounds, return false.

    soFar += board[row][col];   // Adding the curent board element to saFar
    marked[row][col] = true;              // Set it as marked
    const int DELAY = 100;
    BoggleGUI::setAnimationDelay(DELAY);
    BoggleGUI::setHighlighted(row,col); // Visualizing the highlighting with 100 ms delay. (3 lines)

    if (soFar != word.substr(0,soFar.length())) { // if the word cannot be formed in this path, go a step back
        soFar = soFar.substr(0,soFar.length() - 1);
        marked[row][col] = false;
        BoggleGUI::setHighlighted(row,col,false);
        return false;
    } else if (soFar == word) {   // checks if word has been formed by the followed path on board
        humanWordFound.add(word);
        BoggleGUI::recordWord(word,BoggleGUI::HUMAN);
        return true;
    }

    for (int horz = -1; horz <= 1; horz++)  // Recursing over 8 neighbors of each element
        for (int vert = -1; vert <= 1; vert++) {
            if ((horz !=0 || vert !=0))       // otherwise, ot would be the element itself.
                if (humanWordSearchHelper(row + horz, col + vert, word,soFar, marked))
                    return true; // As soon as the word is found (on any valid path), it returns true.
        }

    soFar = soFar.substr(0,soFar.length() -1); // in case we haven't reached the result after all, returns false
    marked[row][col] = false;
    BoggleGUI::setHighlighted(row,col,false);
    return false;
}


/* humanWordSearch() uses humanWordSearchHelper() for all elements (cubes) on the board and
 * checks if the word human inserted is foundon the board. */
bool Boggle::humanWordSearch(string word) {
    Grid<bool> marked;
    marked.resize(ROW_MAX,COL_MAX);

    for (int i = 0; i < ROW_MAX; i++)           // traversing over all rows
        for (int j = 0; j < COL_MAX; j++) {     // traversing over all columns
            char firstLetter = word[0];
            if (firstLetter == board[i][j]) {   // checking if element(i,j) is the 1st letter of word
                marked.fill(false);             // At first, no element of board is marked.
                BoggleGUI::clearHighlighting();
                string emptyStr = "";
                if (humanWordSearchHelper(i, j, word, emptyStr, marked))
                    return true;
            }
        }

    return false; // finally, if the word is not found, return false.
}

/* getScoreHuman adds up the score of human player. for each word of length >=4, score is
 * proportional to the word length. */
int Boggle::getScoreHuman() const {
    int humanScore = 0;
    int minLength = 3;
    if (! humanWordFound.isEmpty())
        for (string s : humanWordFound)
            humanScore += (s.length() - minLength);
    return humanScore;
}

/* function computerWordSearchHelper() is pretty much similar to humanWordSearchHelper(). The difference is
 * actually in validation check. in previous function, we were given a calid word and we tried to find it on
 * the board. Here, we start from a certain point and in case, we can find a word owning soFar as prefix in
 * the dictionary, we continue searching for the words. OW, we do not dollow that path anymore. This gives
 * us a fast trial and error algorithm. */
bool Boggle::computerWordSearchHelper(int row, int col, string soFar,Set<string>& humanWordFound,
                                Set<string>& computerWordFound, Lexicon& Dict,Grid<bool>& marked){
    if (!board.inBounds(row,col) || marked[row][col])
        return false;   // BASE CASE: if the board element is marked already or out of bounds, return false.
    soFar += board[row][col];   // Adding the curent board element to saFar
    marked[row][col] = true;              // Set it as marked
    int minLength = 4;

    if (!Dict.containsPrefix(soFar)) {
        marked[row][col] = false;
        return false;
    } else if (!humanWordFound.contains(soFar) && !computerWordFound.contains(soFar) &&
                soFar.length() >= minLength && Dict.contains(soFar) ) {
            computerWordFound.add(soFar);
            BoggleGUI::recordWord(soFar,BoggleGUI::COMPUTER);
    }

    for (int horz = -1; horz <= 1; horz++)  // Recursing over 8 neighbors of each element
        for (int vert = -1; vert <= 1; vert++)
            if ((horz !=0 || vert !=0))      // otherwise, ot would be the element itself.
                computerWordSearchHelper(row+horz,col+vert,soFar,humanWordFound,computerWordFound,Dict,marked);

    marked[row][col] = false;
    return false;
}

/* computerWordSearch() uses computerWordSearchHelper() for all elements (cubes) on the board and
 * checks if the letters on the adjacent cubes can form valid words */
Set<string> Boggle::computerWordSearch() {
    Grid<bool> marked;
    marked.resize(ROW_MAX,COL_MAX);

    for (int i = 0; i < ROW_MAX; i++)
        for (int j = 0; j < COL_MAX; j++) {
            marked.fill(false);
            string emptyStr = "";
            computerWordSearchHelper(i, j, emptyStr, humanWordFound, computerWordFound, Dict, marked);
        }

    return computerWordFound;
}

/* returns computer's score in the same way done for human's score. (checks also the word found is not
 * among words found by human) */
int Boggle::getScoreComputer() const {
    int computerScore = 0;
    int minLength = 3;
        for (string s : computerWordFound)
            computerScore += (s.length() - minLength);
    return computerScore;
}

/* In order to generalize the code, I use getBoardLength() for different sizes. (for possible extensions
 * in the future) */
ostream& operator<<(ostream& out, const Boggle& boggle) { // const added not to change the boggle
    for (int i = 0; i < boggle.getBoardLength(); i++) {
        for (int j = 0; j < boggle.getBoardLength(); j++)
            out << boggle.getLetter(i,j);
        cout << endl;
    }
    return out;
}

/* This function returns the length of the edge of boggle, which is 4 here. */
int Boggle::getBoardLength() const {
    return board.numCols();
}

/* This function returns the set of words found by human. (Used in boggleplay)*/
Set<string> Boggle::getHumanWords() const {
    return humanWordFound;
}
