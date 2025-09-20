#include <iostream>
#include <windows.h>
using namespace std;
const int ROWS = 7;
const int COLUMNS = 7;
const char EMPTY = ' ';
const char PLAYER1 = 'X';
const char PLAYER2 = 'O';
char arr[7][7] = {};
//===========================================================
// function to reset the elements of the array
void reset() {
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            arr[i][j] = ' ';
        }
    }
}
//===========================================================
// function to print the table 
void print_table() {
    system("cls");
    cout << "     0       1      2       3        4        5       6  \n";
    cout << "  _______________________________________________________\n";
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            cout << " |     " << arr[i][j];
        }
       cout << "|\n  _______________________________________________________\n";
    }
}
//===========================================================
// function to  Drop a piece into a column
bool dropPiece(char arr[ROWS][COLUMNS], int column, char piece) {
    if (column < 0 || column >= COLUMNS) return false;
    for (int row = ROWS - 1; row >= 0; row--) {
        if (arr[row][column] == EMPTY) {
            arr[row][column] = piece;
            return true;
        }
    }
    return false; // Column is full
}
//========================================================================
// function to check Winner
bool checkWinner(const char board[ROWS][COLUMNS], char piece) {
    // Horizontal
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col <= COLUMNS - 4; col++) {
            if (board[row][col] == piece && board[row][col + 1] == piece &&
                board[row][col + 2] == piece && board[row][col + 3] == piece)
                return true;
        }
    }
    // Vertical
    for (int col = 0; col < COLUMNS; col++) {
        for (int row = 0; row <= ROWS - 4; row++) {
            if (board[row][col] == piece && board[row + 1][col] == piece &&
                board[row + 2][col] == piece && board[row + 3][col] == piece)
                return true;
        }
    }
    // Diagonal (main)
    for (int row = 0; row <= ROWS - 4; row++) {
        for (int col = 0; col <= COLUMNS - 4; col++) {
            if (board[row][col] == piece && board[row + 1][col + 1] == piece &&
                board[row + 2][col + 2] == piece && board[row + 3][col + 3] == piece)
                return true;
        }
    }
    // Diagonal (sec)
    for (int row = 0; row <= ROWS - 4; row++) {
        for (int col = 3; col < COLUMNS; col++) {
            if (board[row][col] == piece && board[row + 1][col - 1] == piece &&
                board[row + 2][col - 2] == piece && board[row + 3][col - 3] == piece)
                return true;
        }
    }
    return false;
}

//=======================================================================================
// function Check if the board is full
bool isBoardFull(const char board[ROWS][COLUMNS]) {
    for (int col = 0; col < COLUMNS; col++) {
        if (board[0][col] == EMPTY)
            return false;
    }
    return true;
}
//=======================================================================================
//function to cntrol level 1
void level_1() {
    reset();
    char currentPlayer = PLAYER1;
    bool gameOver = false;

    while (!gameOver) {
        print_table();
        int column;

        cout << "Player " << currentPlayer << ", choose a column (0-" << COLUMNS - 1 << "): ";
        cin >> column;

        while (!dropPiece(arr, column, currentPlayer ) || cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid move! Try again.\n";
            cin >> column;
           // continue;
        }

        if (checkWinner(arr, currentPlayer)) {
            print_table();
            cout << "Player " << currentPlayer << " wins! \n";
            gameOver = true;
        }
        else if (isBoardFull(arr)) {
            print_table();
            cout << "It's a draw!\n";
            gameOver = true;
        }
        else {
            currentPlayer = (currentPlayer == PLAYER1) ? PLAYER2 : PLAYER1;
        }
    }
}

//======================================================level 2====================================
// function to simulate the player and computer move
bool simulateDrop(char arr[7][7], int col, char symbol, int& rowPlaced) {
    for (int row = 6; row >= 0; row--) {
        if (arr[row][col] == EMPTY) {
            arr[row][col] = symbol;
            rowPlaced = row;
            return true;
        }
    }
    return false;  // column is full
}

// to reset the index
void undoDrop(char arr[7][7], int row, int col) {
    arr[row][col] = EMPTY;
}

// Count number of winning moves for player after simulating drop
int countWinningMoves(char arr[7][7], char player) {
    int count = 0;
    int tempRow;
    for (int col = 0; col < 7; col++) {
        if (simulateDrop(arr, col, player, tempRow)) {
            if (checkWinner(arr, player)) {
                count++;
            }
            undoDrop(arr, tempRow, col);
        }
    }
    return count;
}

// function to control level 2
void level_2(char arr[7][7]) {
    int rowPlaced;

    // 1. Try to win
    for (int col = 0; col < 7; col++) {
        if (simulateDrop(arr, col, PLAYER2, rowPlaced)) {
            if (checkWinner(arr, PLAYER2)) {
                return; // keep it if it makes computer wins
            }
            undoDrop(arr, rowPlaced, col); // undo if not winning
        }
    }

    // 2. Try to block player from winning
    for (int col = 0; col < 7; col++) {
        if (simulateDrop(arr, col, PLAYER1, rowPlaced)) {
            if (checkWinner(arr, PLAYER1)) {
                arr[rowPlaced][col] = PLAYER2; // block player
                return;
            }
            undoDrop(arr, rowPlaced, col);
        }
    }

    // 3. Block future dual threats from player (if player would have two ways to win)
    for (int col = 0; col < 7; col++) {
        if (simulateDrop(arr, col, PLAYER2, rowPlaced)) {
            int threats = countWinningMoves(arr, PLAYER1);
            if (threats >= 2) {
                undoDrop(arr, rowPlaced, col);
                continue; // skip this move if it allows player to create dual threat
            }
            // otherwise play here
            return;
        }
    }

    // 4. Choose first available column
    for (int col = 0; col < 7; col++) {
        if (dropPiece(arr, col+3, PLAYER2)) {
            return;
        }
    }
}


int main() {
    //to ask the user if he want to play again
    bool again = 1;
    while (again) {
        cout << "                                 _________________________________________" << endl;
        cout << "                                       (welcome to Connect 4 GAME)                                   " << endl;
        cout << "                                 _________________________________________" << endl;
        cout << "\n\n Which level do you want to play?\n";
        cout << "1) Level 1  [player vs player]\n";
        cout << "2) Level 2  [player vs Computer]\n";
        cout << "********************************************************\n";
        int level;
        cin >> level;
        //to make sure that the input is valid
        while (level > 2 || level < 1 || cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. please enter another number\n";
            cin >> level;
            cin.ignore(10000, '\n');
        }
        switch (level)
        {
        case 1:
        {
            cout << "                                   Level 1  [player vs player]\n";
            cout << "                             ***************************************\n\n";
            level_1();
            break;
        }
        //===================================================================================================
        case 2:
        {
            cout << "                                   Level 2  [player vs Computer]\n";
            cout << "                             ***************************************\n\n";

            reset();
            char currentPlayer = PLAYER1; // player 1 is x
            bool gameOver = false;

            while (!gameOver) {
                print_table();
                int column;

                if (currentPlayer == PLAYER1) {
                    cout << "Your turn. Choose a column (0-" << COLUMNS - 1 << "): ";
                    cin >> column;

                    while (!dropPiece(arr, column, PLAYER1) || cin.fail()) {
                        cin.clear();
                        cin.ignore(10000, '\n');
                        cout << "Invalid move! Try again.\n";
                        cin >> column;
                    }
                }
                else {
                    cout << "\nComputer is thinking...\n";
                    Sleep(1000);
                    level_2(arr); // computer plays
                }

                if (checkWinner(arr, currentPlayer)) {
                    print_table();
                    if (currentPlayer == PLAYER1)
                        cout << "You win! \n";
                    else
                        cout << "Computer wins! \n";
                    gameOver = true;
                }
                else if (isBoardFull(arr)) {
                    print_table();
                    cout << "It's a draw!\n";
                    gameOver = true;
                }
                else {
                    currentPlayer = (currentPlayer == PLAYER1) ? PLAYER2 : PLAYER1;
                }
            }

            break;
        }
        }
        cout << "Do you want to play again?\n\ Press 1 to play again and 0 to exit:";
        cin >> again;
        while (again > 1 || again < 0 || cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid number. please enter another number\n";
            cin >> again;
        }
    }
    cout << "\nThank you for using our programme\n";
    return 0;
}