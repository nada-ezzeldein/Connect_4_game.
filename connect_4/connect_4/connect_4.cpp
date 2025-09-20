#include <iostream>
#include <windows.h>
using namespace std;
const int rows = 7;
const int columns = 7;
const char empty_1 = ' ';
const char player_1 = 'X';
const char player_2 = 'O';
const int ai_depth = 7; // the lenth that comuter simulate the game in
const int win_score = 1000000;  
char arr[rows][columns] = {};
//===========================================================
// function to reset the elements of the array
void reset() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            arr[i][j] = empty_1;
        }
    }
}
//===========================================================
// function to print the table 
void print_table() {
    system("cls");
    cout << "     0       1      2       3        4        5       6  \n";
    cout << "  _______________________________________________________\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            cout << " |     " << arr[i][j];
        }
        cout << "|\n  _______________________________________________________\n";
    }
}
//===========================================================
// function to  Drop a piece into a column
bool dropPiece(int column, char piece) {
    if (column < 0 || column >= columns) return false;
    for (int row = rows - 1; row >= 0; row--) {
        if (arr[row][column] == empty_1) {
            arr[row][column] = piece;
            return true;
        }
    }
    return false; // Column is full
}
//========================================================================
// function to check Winner
bool check_winner(char piece) {
    // Horizontal
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col <= columns - 4; col++) {
            if (arr[row][col] == piece && arr[row][col + 1] == piece &&
                arr[row][col + 2] == piece && arr[row][col + 3] == piece)
                return true;
        }
    }
    // Vertical
    for (int col = 0; col < columns; col++) {
        for (int row = 0; row <= rows - 4; row++) {
            if (arr[row][col] == piece && arr[row + 1][col] == piece &&
                arr[row + 2][col] == piece && arr[row + 3][col] == piece)
                return true;
        }
    }
    // Diagonal (main)
    for (int row = 0; row <= rows - 4; row++) {
        for (int col = 0; col <= columns - 4; col++) {
            if (arr[row][col] == piece && arr[row + 1][col + 1] == piece &&
                arr[row + 2][col + 2] == piece && arr[row + 3][col + 3] == piece)
                return true;
        }
    }
    // Diagonal (sec)
    for (int row = 0; row <= rows - 4; row++) {
        for (int col = 3; col < columns; col++) {
            if (arr[row][col] == piece && arr[row + 1][col - 1] == piece &&
                arr[row + 2][col - 2] == piece && arr[row + 3][col - 3] == piece)
                return true;
        }
    }
    return false;
}
//=======================================================================================
// function Check if the board is full
bool board_full() {
    for (int col = 0; col < columns; col++) {
        if (arr[0][col] == empty_1)
            return false;
    }
    return true;
}
//=======================================================================================
// ====================================level_2=========================================
// function to check the winner for the next 4 moves
int check_score(int a, int b, int c, int d, char piece) {
    int score = 0;
    // if piece = player 1 the enemy will be player 2 and if piace = player 2 the enemy will be player 1
    char enemy_piece = (piece == player_1) ? player_2 : player_1;
    
    int count_piece = 0, count_empty = 0, count_enemy = 0;
    char window[4] = { a, b, c, d };
    for (int i = 0; i < 4; ++i) {
        if (window[i] == piece) count_piece++;
        else if (window[i] == enemy_piece) count_enemy++;
        else if (window[i] == empty_1) count_empty++;
    }
    // give the score value according to the state
    if (count_piece == 4) score += 100; // this move make win
    else if (count_piece == 3 && count_empty == 1) score += 10;// this move make us closer to win
    else if (count_piece == 2 && count_empty == 2) score += 5; // this move is good but there is better
    if (count_enemy == 3 && count_empty == 1) score -= 80; // this move make enemy win
    return score;
}

// evaluate the hole board
int evaluate_board(char piece) {
    int score = 0;
    // give bonus to the middle column because it makes win easier
    for (int i = 0; i < rows; i++) {
        if (arr[i][columns / 2] == piece)
            score += 3;
    }
    // evaluate the horizontal lines
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c <= columns - 4; ++c)
    // (columns - 4) => this part to make sure that we don't exceeds the limit of columns
            score += check_score(arr[r][c], arr[r][c + 1], arr[r][c + 2], arr[r][c + 3], piece);
    // evaluate the vertical lines
    for (int c = 0; c < columns; ++c)
        for (int r = 0; r <= rows - 4; ++r)
            score += check_score(arr[r][c], arr[r + 1][c], arr[r + 2][c], arr[r + 3][c], piece);
    // evauate diagonal lines (main)
    for (int r = 3; r < rows; ++r)
        for (int c = 0; c <= columns - 4; ++c)
            score += check_score(arr[r][c], arr[r - 1][c + 1], arr[r - 2][c + 2], arr[r - 3][c + 3], piece);
    // evauate diagonal lines (second)
    for (int r = 0; r <= rows - 4; ++r)
        for (int c = 0; c <= columns - 4; ++c)
            score += check_score(arr[r][c], arr[r + 1][c + 1], arr[r + 2][c + 2], arr[r + 3][c + 3], piece);
    return score;
}

// mini_max function for computer move
int minimax(int depth, int max_c, int max_p, bool maximizingPlayer) {
    // max_c is the maximum value that the computer has reached
    // max_p is the maximum valus that the player has reached
    if (check_winner(player_2)) return win_score; // computer wins
    if (check_winner(player_1)) return -win_score; // player wins
    //(-win_score) => to give small value to score which mean that this move isn't good for computer
    if (board_full()) return 0; // draw
    if (depth == 0) return evaluate_board(player_2); // reach maximum depth

    if (maximizingPlayer) {
        int value = -10000000;
        for (int col = 0; col < columns; col++) {
            if (arr[0][col] == empty_1) { // if the coloumn isn't full
                int row;
                for (row = rows - 1; row >= 0; row--) {
                    if (arr[row][col] == empty_1) {
                        arr[row][col] = player_2;  // make the computer move
                        break;
                    }
                }
                int score = minimax(depth - 1, max_c, max_p, false);
                arr[row][col] = empty_1; // undo the move
                if (score > value) value = score;
                if (value > max_c) max_c = value;
                if (max_c >= max_p) break; // if the computer have greater value than player which make it wins break (3lshan keda keda hiksap)
            }
        }
        return value;
    }
    else {
        int value = 10000000;
        for (int col = 0; col < columns; col++) {
            if (arr[0][col] == empty_1) { // if the column isn't full
                int row;
                for (row = rows - 1; row >= 0; row--) {
                    if (arr[row][col] == empty_1) {
                        arr[row][col] = player_1; // make player move
                        break;
                    }
                }
                int score = minimax(depth - 1, max_c, max_p, true);
                arr[row][col] = empty_1; // remove the move 
                if (score < value) value = score;
                if (value < max_p) max_p = value;
                if (max_c >= max_p) break; //  if the computer have greater value than player which make it wins break (3lshan keda keda hiksap)
            }
        }
        return value;
    }
}

// do the best move
int do_best_move() {
    int best_score = -10000000;
    int best_coloumn = 3; // the middle column as a defult choose

    for (int col = 0; col < columns; col++) {
        if (arr[0][col] == empty_1) { // if the coloumn isn't full
            int row;
            for (row = rows - 1; row >= 0; row--) {
                if (arr[row][col] == empty_1) {
                    arr[row][col] = player_2; // make computer move
                    break;
                }
            }
            int score = minimax(ai_depth - 1, -10000000, 10000000, false);
            arr[row][col] = empty_1; // remove the move

            if (score > best_score) {
                best_score = score;
                best_coloumn = col;
            }
        }
    }
    return best_coloumn;
}

//=======================================================================================
//function to cntrol level 1
void level_1() {
    reset();
    char currentPlayer = player_1;
    bool gameOver = false;

    while (!gameOver) {
        print_table();
        int column;

        cout << "Player " << currentPlayer << ", choose a column (0-" << columns - 1 << "): ";
        cin >> column;

        while (!dropPiece(column, currentPlayer) || cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid move! Try again.\n";
            cin >> column;
        }

        if (check_winner(currentPlayer)) {
            print_table();
            cout << "Player " << currentPlayer << " wins! \n";
            gameOver = true;
        }
        else if (board_full()) {
            print_table();
            cout << "It's a draw!\n";
            gameOver = true;
        }
        else {
            currentPlayer = (currentPlayer == player_1) ? player_2 : player_1;
        }
    }
}

//=======================================================================================
//function to cntrol level 2 (Player vs Computer)
void level_2() {
    reset();
    bool gameOver = false;

    while (!gameOver) {
        // Player's turn
        print_table();
        int column;
        cout << "Your turn (X), choose a column (0-" << columns - 1 << "): ";
        cin >> column;

        while (!dropPiece(column, player_1) || cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid move! Try again.\n";
            cin >> column;
        }

        if (check_winner(player_1)) {
            print_table();
            cout << "Congratulations! You win!\n";
            gameOver = true;
            continue;
        }
        else if (board_full()) {
            print_table();
            cout << "It's a draw!\n";
            gameOver = true;
            continue;
        }

        // Computer's turn
        cout << "Computer is thinking... ";
        Sleep(500);
        int computerColumn = do_best_move();
        dropPiece(computerColumn, player_2);
        
        cout << "Computer chooses column " << computerColumn << endl;
         

        if (check_winner(player_2)) {
            print_table();
            cout << "Computer wins! Better luck next time.\n";
            gameOver = true;
        }
        else if (board_full()) {
            print_table();
            cout << "It's a draw!\n";
            gameOver = true;
        }
    }
}

//=======================================================================================
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
        case 2:
        {
            cout << "                                   Level 2  [player vs Computer]\n";
            cout << "                             ***************************************\n\n";
            level_2();
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