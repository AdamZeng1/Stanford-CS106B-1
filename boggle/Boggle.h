/* Stanford CS 106B 4th HW       Summer 2015
 * Ali Parsaei                   ID: 05950982
 * Class Boggle, header file
 * Class Boggle provides us with a collection of functions for Boggle object, which
 * is used in Boggleplay.cpp to play the game, BOGGLE with computer.
 */

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include "lexicon.h"
#include <string>
#include "grid.h"
#include "set.h"
using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = "");
    char getLetter(int row, int col) const;
    bool checkWord(string word);
    bool humanWordSearch(string word);
    Set<string> computerWordSearch();
    int getScoreHuman() const;
    int getScoreComputer() const;
    int getBoardLength() const;
    Set<string> getHumanWords() const;

    friend ostream& operator<<(ostream& out, const Boggle& boggle);

private:
    static int ROW_MAX;
    static int COL_MAX;
    Lexicon Dict;
    Grid<char> board;
    Set<string> humanWordFound;
    Set<string> computerWordFound;
    bool humanWordSearchHelper(int row, int col, string word, string& soFar, Grid<bool>& marked);
    bool computerWordSearchHelper(int row, int col, string soFar,Set<string>& humanWordFound,
                                    Set<string>& computerWordFound, Lexicon& Dict,Grid<bool>& marked);
};

#endif // _boggle_h
