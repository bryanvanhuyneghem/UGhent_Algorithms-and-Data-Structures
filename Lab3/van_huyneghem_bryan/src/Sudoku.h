#ifndef SUDOKU_H
#define SUDOKU_H

#include <fstream>
#include <iostream>
#include <cstdlib>

class Sudoku {
public:
    Sudoku(const std::string &filename);

    bool isSolved();

    bool solve();

    int upperThree();

    friend std::ostream &operator<<(std::ostream &out, const Sudoku &s);

private:
    int grid[9][9];

    bool foundEmptyCell(int &row, int &column);

    bool canPlace(int row, int column, int number);

    void print();


};

std::ostream &operator<<(std::ostream &out, const Sudoku &s) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            out << s.grid[i][j] << " ";

            if ((j + 1) % 3 == 0 && j < 8)
                out << "| ";
        }
        out << std::endl;
        if ((i + 1) % 3 == 0 && i < 8) {
            out << "------+-------+------" << std::endl;
        }
    }
    return out;
}

// Function for debugging
void Sudoku::print() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            std::cout << grid[i][j] << " ";

            if ((j + 1) % 3 == 0 && j < 8)
                std::cout << "| ";
        }
        std::cout << std::endl;
        if ((i + 1) % 3 == 0 && i < 8) {
            std::cout << "------+-------+------" << std::endl;
        }
    }
}

// Constructor
Sudoku::Sudoku(const std::string &filename) {
    std::ifstream infile(filename);
    if (infile) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                infile >> grid[i][j];
            }
        }
    }
}

// Check if a Sudoku is solved
bool Sudoku::isSolved() {
    int numbers[9] = {0};

    // rows
    for (int row = 0; row < 9; row++) {
        for (int column = 0; column < 9; column++) {
            numbers[grid[row][column] - 1]++;
        }
        for (int i = 0; i < 9; i++) {
            if (numbers[i] != 1) {
                return false;
            }
            // Reset the index to 0 for the next iteration of row
            numbers[i] = 0;
        }
    }
    // columns
    for (int column = 0; column < 9; column++) {
        for (int row = 0; row < 9; row++) {
            numbers[grid[row][column] - 1]++;
        }
        for (int i = 0; i < 9; i++) {
            if (numbers[i] != 1) {
                return false;
            }
            // Reset the index to 0 for the next iteration of row
            numbers[i] = 0;
        }
    }
    // cells
    for (int cell_row = 0; cell_row < 3; cell_row++) {
        for (int cell_column = 0; cell_column < 3; cell_column++) {
            for (int row = 0; row < 3; row++) {
                for (int column = 0; column < 3; column++) {
                    numbers[grid[row + cell_row * 3][column + cell_column * 3] - 1]++;
                }
            }
            for (int i = 0; i < 9; i++) {
                if (numbers[i] != 1) {
                    return false;
                }
                // Reset the index to 0 for the next iteration of row
                numbers[i] = 0;
            }
        }
    }
    return true;
}

// Helper function that finds an empty in my Sudoku puzzle
bool Sudoku::foundEmptyCell(int &row, int &column) {
    // Loop through the rows and columns of the puzzle, in search of an empty cell
    for (row = 0; row < 9; row++)
        for (column = 0; column < 9; column++)
            // Is it an empty cell?
            if (grid[row][column] == 0)
                // Yes: I have found my cell with coordinates [row, column]
                return true;
    return false; // No
}

// Helper function that checks whether I can place a number in this specific cell
// Check the three Sudoku rules
// This prunes the tree, because it won't branch when it sees that it cannot place the number
// This vastly limits the size of the state space of the tree
bool Sudoku::canPlace(int rowToCheck, int columnToCheck, int number) {
        // Check if I can place it according to the rules for the row
        for (int row = 0; row < 9; row++) {
            if (grid[row][columnToCheck] == number) {
                return false; // I cannot place my number
            }
        }
        // Check if I can place it according to the rules for the column
        for (int column = 0; column < 9; column++) {
            if (grid[rowToCheck][column] == number) {
                return false; // I cannot place my number
            }
        }
        // Check if I can place it according to the rules for the cell
        int cellRow = (int) (rowToCheck) / 3; // the row number within the cell itself
        int cellColumn = (int) (columnToCheck) / 3; // the column number within the cell itself
        for (int row = 0; row < 3; row++) {
            for (int column = 0; column < 3; column++) {
                if (grid[row + cellRow * 3][column + cellColumn * 3] == number) {
                    return false; // I cannot place my number
                }
            }
        }
    return true; // I can place my number
}

// Recursively solve the Sudoku using Backtracking (specialization of DFS)
bool Sudoku::solve() {
    //print();
    // Find a cell that is empty (i.e. a cell set to 0)
    int row;
    int column;
    // Check if there are unassigned cells.
    // If no, then we are done and we have solved the Sudoku puzzle
    if (!foundEmptyCell(row, column)) {
        return true; // Recursion ends
    }
    // Try to place the numbers 1 through 9 in that cell
    for (int number = 0; number < 9; number++) {
        // Check if we can place that number is this cell according to Sudoku rules
        if (canPlace(row, column, number + 1)) {
            // If we can place the number, then place it in the cell
            grid[row][column] = number + 1;
            // Can I keep solving the Sudoku?
            if (solve()){
                // Continue solving the Sudoku with the new number in place
                return true;
            }
            // If I can't solve it, revert the assignment of the number to the cell (pruning)
            grid[row][column] = 0;
        }
    }
    // If I get here, I have to backtrack, because the solution is not in this branch.
    // This means I return to the previous recursion call to try a different number
    return false;
}

// Sum of the three upper limit corner digits
int Sudoku::upperThree(){
    return grid[0][0]*100 + grid[0][1]*10 + grid[0][2];
}

#endif