/* Stanford CS 106B 4th HW       Summer 2015
 * Ali Parsaei                   ID: 05950982
 * boggleplay
 * boggleplay includes codes related to getting inputs and printing output and provides
 * an interface in order to use CLASS Boggle and provide an interactive game to play with.
 * first, human plays the game and writes the word they found. Then computer, finds ALL
 * possible words that are not found by human on the board by exhaustive searching and recursion.
 * The one with higher scores wins the game. 
 * Sources: Ariend of mine helped me out with some parts
 */

#include "lexicon.h"
#include <string>
#include <iostream>
#include "simpio.h"
#include "console.h"
#include "strlib.h"
#include "Boggle.h"
#include "bogglegui.h"

/* This function checks if the input string by user is valid to form a boggle board or not. It is valid
 * if its length == 16 and all characters are in alphabet. */
bool isInputValid(string inputString) {
    const int inputValidLength = 16;
    if (inputString.size() != inputValidLength)
        return false;
    for (int i = 0; i < inputValidLength; i++)
        if (!isalpha(inputString[i]))
            return false;
    return true;
}

/* setBoardText() sets the boardText we need for constructing a Boggle object by prompting
 * the user to insert valid input string. */
string setBoardText() {
    string boardText = "";
    if (getYesOrNo("Do you want to generate a random board? ")) {
        clearConsole(); // in case, random board is chosen.
        return boardText;
    } else {
        while (true) {  // I use trim to ignore spaces around the user's input.
            boardText = toUpperCase(trim(getLine("Type the 16 letters to appear on the board: ")));
            if (isInputValid(boardText)) {
                clearConsole();
                return boardText;
            } else
                cout << "That is not a valid 16-letter board string. Try again." << endl;
        }
    }
}

/* This function implements the details of getting and displaying the information about human's turn
 * in Boggle game. */
void humanTurnGame(Boggle& boggle) {
    cout << "It's your turn! "<< endl;
    cout<< boggle<< endl;
    cout << "Your words (0): {}" << endl;
    cout << "Your score: 0" << endl;
    BoggleGUI::setStatusMessage("It's your turn!");

    while (true) {
        string newWord = toUpperCase(getLine("Type a word (or Enter to stop): "));
        if (newWord.empty())
            break;

        clearConsole();
        if (!boggle.checkWord(newWord))
            cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
        else if (!boggle.humanWordSearch(newWord)) {
            string message = "That word can't be formed on this board.";
            cout << message << endl;
            BoggleGUI::setStatusMessage(message);
        } else {
            string message = "You found a new word! \"" +  newWord + '\"';
            cout << message << endl;
            BoggleGUI::setStatusMessage(message);
        }

        cout << boggle << endl;
        cout << "Your words (" << (boggle.getHumanWords()).size()<< "): " << boggle.getHumanWords() << endl;
        cout << "Your score: " << boggle.getScoreHuman() << endl;
        BoggleGUI::setScore(boggle.getScoreHuman(), BoggleGUI::HUMAN);
    }
    cout << endl;
}

/* This function implements the details of displaying the information about computer's turn
 * in Boggle game. It also displays the final messages.*/
void computerTurnGame(Boggle& boggle) {
    cout << "It's my turn! " << endl;
    cout << "My words (" << (boggle.computerWordSearch()).size() << "): " << boggle.computerWordSearch() << endl;
    cout << "My score: " << boggle.getScoreComputer() << endl;
    BoggleGUI::setScore(boggle.getScoreComputer(), BoggleGUI::COMPUTER);

    if(boggle.getScoreHuman() >= boggle.getScoreComputer()){
        string message = "WOW, you defeated me! Congratulations!";
        cout << message << endl;
        BoggleGUI::setStatusMessage(message);
    } else {
        string message = "Ha ha ha, I destroyed you. Better luck next time, puny human! ";
        cout << message << endl;
        BoggleGUI::setStatusMessage(message);
    }
}

/* playOneGame() uses the above-defined functions in order to play the game once. */
void playOneGame(Lexicon& dictionary) {
    string boardText = setBoardText();
    Boggle boggle(dictionary,boardText);
    humanTurnGame(boggle);
    BoggleGUI::clearHighlighting();
    computerTurnGame(boggle);
}

