#include <iostream>
#include "Sudoku.h"
#include <ctime>
#include <chrono>
#include <unistd.h>

int main() {
    int sum = 0;
    // solve all Sudokus
    for(int i = 1; i <= 50; i++){
        Sudoku s("sudokus\\" + std::to_string(i) + ".txt");
        s.solve();
        sum += s.upperThree();
        std::cout << "#" << i << " " << s.isSolved() << std::endl;
    }

    std::cout << "Sum: " << sum << std::endl;
    if(sum == 24702){
        std::cout << "yay!" << std::endl;
    }
}
