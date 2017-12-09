/*  Ali Parsaei (aparsaei)  ID: 05950982
 * CS 106B 1st assignment: Game of Life Simulation.he game models the life cycle
 * of bacteria using a two-dimensional grid of cells. Given an initial pattern,
 * the game simulates the birth and death of future generations of cells using a
 * set of simple rules. The rules are included in "welcome" function.
 *
 * Sources:
 *  -Course lecture slides
 *
 *
 * This file is the extra assignment. (Wrap-around)
 *
 * The only difference is in checkCell function
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "lifegui.h"
using namespace std;

//function prototypes
//(Description of each function is above the function below main function)
void welcome();
void fileInput(Grid<string>& grid);
void fileUpdate(Grid<string>& gridInit, Grid<string>& gridFin);
void printFile(Grid<string>& gridFile);


int main() {
    Grid<string> inputFile;
    Grid<string> outputFile;

    welcome();
    fileInput(inputFile);

    // This part is related to different commands which can be provided by the
    //operator.
    string com;

    while (com != "q" && com != "Q"){
        cout<<"a)nimate, t)ick, q)uit? ";

        com = getLine();

        if (com == "a" || com == "A"){

            int iterNum = getInteger("Please enter the number of iterations: ");
            //get the # of iterations
            clearConsole();                // it clears the console before animation starts
            setConsoleClearEnabled (true); // it activates clearConsole();

            for (int iteration = 1; iteration <=iterNum; iteration++){
                fileUpdate(inputFile, outputFile);

                clearConsole();         // it clears the console each iteration

                printFile(outputFile);  // prints out the output after one iteration
                pause(50);

                inputFile = outputFile; // updates the input for next generation
            }
        }else if (com == "t" || com == "T"){
            fileUpdate(inputFile, outputFile); // updates the grid only for one generation
            printFile(outputFile);
            inputFile = outputFile;
        } else if (com != "q" && com != "Q"){
            cout<<"Please insert valid command:"<<endl;
        }

    }

    if (com == "q" || com == "Q"){
        cout << "Have a nice Life!" << endl;
    }

    return 0;
}


//Function welcome displays the welcome message
void welcome(){
    // Printing the introductory message:
    cout<<"Welcome to the CS 106B Game of Life"<<endl
        <<"a simulation of the lifecycle of a bacteria colony."<<endl
        <<"Cells (X) live and die by the following rules:"<<endl
        <<"- A cell with 1 or fewer neighbors dies."<<endl
        <<"- Locations with 2 neighbors remain stable."<<endl
        <<"- Locations with 3 neighbors will create life."<<endl
        <<"- A cell with 4 or more neighbors dies."<<endl<<endl;
}


//fileInput gets the file name from operator, and if the file
//exists, prints it and makes a Grid object grid, which is the
//output of this function
void fileInput(Grid<string>& grid){

    ifstream input;
    promptUserForFile(input, "Grid input file name? ");
    string line;
    int count = 1;
    int rowNum;
    int colNum;

    for(count = 1; count <= 2; count ++){
        getline(input, line);

        if (count == 1){
        rowNum = stringToInteger(line);
        }

        if (count == 2){
        colNum = stringToInteger(line);
        }
    }
    grid.resize(rowNum,colNum);

    while (getline(input, line)) {

         if(count>=3 && count <= (2 + rowNum)){
             cout<<line<<endl;
             for(int j = 0; j < colNum; j ++){
                 grid[count-3][j] = line[j];

             }

         }
         count++;

    }
    input.close();
}


// This function Returns the number of living neigbors of each cell.
//Idid not include it in the prototype section sinceI only used it in
// fileUpdate function
int checkCell(int x, int y ,Grid<string>& gridInit){
    int XNum = 0;
    for (int m = x-1; m <=x+1; m++){        //going over all neigbors indices row

        for (int n = y-1; n <=y+1; n++){    //going over all neigbors indices col

            if (gridInit.inBounds(m,n) && (m !=x || n !=y) && gridInit[m][n] == "X"){
                // checking the existence of the neighbor && also living cells
                XNum ++;    // counting the # of neighbors
            }

        }

    }

    if (x == 0){ // This loop is to count extra # neighbors for 1st row
        for (int n = y-1; n <=y+1; n++){
            if (gridInit.inBounds(gridInit.nRows -1,n) &&
                    gridInit[gridInit.nRows -1][n] == "X"){
                    XNum ++; // adding extra # of neighbors
            }
        }
    }

    if (x == gridInit.nRows -1 ){ // This loop is to count extra # neighbors for last row
        for (int n = y-1; n <=y+1; n++){
            if (gridInit.inBounds(0,n) && gridInit[0][n] == "X"){
                    XNum ++; // adding extra # of neighbors
            }
        }
    }

    if ( y == 0 ){ // This loop is to count extra # neighbors for 1st column
        for (int m = x-1; m <=x+1; m++){
            if (gridInit.inBounds(m,gridInit.nCols -1 &&
                                  gridInit[m][gridInit.nCols -1] == "X")){
                    XNum ++; // adding extra # of neighbors
            }
        }
    }

    if ( y == gridInit.nCols -1 ){  // This loop is to count extra # neighbors for last column
        for (int m = x-1; m <=x+1; m++){
            if (gridInit.inBounds(m,0) && gridInit[m][0] == "X"){
                    XNum ++; // adding extra # of neighbors
            }
        }
    }

    return XNum;
}


//fileUpdate will update the input file and turns a
//Grid Object, gridFin as the updated grid at one generation
void fileUpdate(Grid<string>& gridInit, Grid<string>& gridFin){

    gridFin.resize(gridInit.nRows, gridInit.nCols); //resizes the otput grid

    // Checking each element of the grid and applying the rules of Game of Life
    for(int x = 0; x < gridInit.nRows; x++){

        for(int y = 0; y < gridInit.nCols; y++){

            int XN = checkCell(x,y,gridInit);   // XN the # of living neighbors

            switch (XN)           // applying the rules of Game of Life
            {
            case 0:
                gridFin[x][y] = "-";
                break;
            case 1:
                gridFin[x][y] = "-";
                break;
            case 2:
                gridFin[x][y] = gridInit[x][y];
                break;
            case 3:
                gridFin[x][y] = "X";
                break;
            default:
                gridFin[x][y] = "-";
            }

        }

    }

}


// this function is to print the grid in an elegant way.
// cause grid.toString prints a bad format.
void printFile(Grid<string>& gridFile){
    for (int a = 0; a < gridFile.nRows; a++){
        for (int b = 0; b < gridFile.nCols; b++){
            cout<<gridFile[a][b];
        }
        cout<<endl;
    }


}
