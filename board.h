#pragma once
#include <iostream>
#include<vector>
#include <random>
using namespace std;

// class for the game board printed in the terminal
class Board{
    int _numRows = 0;
    int _numCols = 0;
    int _numMines = 0;
    vector<vector<int>> boardArr;

public:

    // constructor
    Board(int rows, int cols, int mines){
        _numRows = rows;
        _numCols = cols;
        _numMines = mines;
        for (int row = 0; row < _numRows; ++row) {
            vector<int> tmp;
            for (int col = 0; col < _numCols; ++col){
                tmp.push_back(0);
            }
            boardArr.push_back(tmp);
        }
    }

    vector<vector<int>> getBoard(){
        return boardArr;
    }

    void printBoard(){
        for (int row = 0; row < _numRows; ++row) {
            for (int col = 0; col < _numCols; ++col){
                cout << boardArr[row][col] << "   ";
            }
            cout << endl;
        }
    }

    // randomly adds mines to the board
    void placeMines(){
        for (int i=0; i<_numMines; i++){
            random_device dev;
            mt19937 rng(dev());
            uniform_int_distribution<mt19937::result_type> rowNum(0,_numRows - 1);
            uniform_int_distribution<mt19937::result_type> colNum(0,_numCols - 1);

            unsigned int placeRow = rowNum(rng);
            unsigned int placeCol = colNum(rng);

            bool alreadyBomb = true;
            while (alreadyBomb){
                if (boardArr[placeRow][placeCol] == 9){
                    placeRow = rowNum(rng);
                    placeCol = colNum(rng);
                }
                else{alreadyBomb = false;}
            }
            boardArr[placeRow][placeCol] = 9;
        }
    }

    // checks the number of mines touching the particular square
    int checkForNearbyMines(int row, int col){
        int numNearbyMines = 0;
        if (boardArr.at(row).at(col) == 9){return 9;} // if bomb

        try{if (boardArr.at(row).at(col-1) == 9){numNearbyMines++;}} // middle left
        catch(...){}
        try{if (boardArr.at(row).at(col+1) == 9){numNearbyMines++;}} // middle right
        catch(...){}
        try{if (boardArr.at(row-1).at(col) == 9){numNearbyMines++;}} // middle top
        catch(...){}
        try{if (boardArr.at(row+1).at(col) == 9){numNearbyMines++;}} // middle bottom
        catch(...){}
        try{if (boardArr.at(row-1).at(col-1) == 9){numNearbyMines++;}} // top left corner
        catch(...){}
        try{if (boardArr.at(row-1).at(col+1) == 9){numNearbyMines++;}} // top right corner
        catch(...){}
        try{if (boardArr.at(row+1).at(col-1) == 9){numNearbyMines++;}} // bottom left corner
        catch(...){}
        try{if (boardArr.at(row+1).at(col+1) == 9){numNearbyMines++;}} // bottom right corner
        catch(...){}
        return numNearbyMines;
    }

    // adds the number of mines that are touching the square to the game board
    void addNumNearbyMines(){
        for (int i=0; i<_numRows; i++){
            for (int j=0; j<_numCols; j++){
                boardArr[i][j] = checkForNearbyMines(i, j);
            }
        }
    }

};

