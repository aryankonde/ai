#include <iostream>
#include <vector>

using namespace std;

vector<vector<char>> board = {
    {'_', '_', '_'},
    {'_', '_', '_'},
    {'_', '_', '_'}
};

// Function to display the board
void display() {
    for (const auto& row : board) {
        for (char cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// Function to check if a player has won
char checkWin() {
    for (int i = 0; i < 3; i++) {
        // Check rows
        if (board[i][0] != '_' && board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return board[i][0];
        // Check columns
        if (board[0][i] != '_' && board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return board[0][i];
    }
    // Check diagonals
    if (board[0][0] != '_' && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return board[0][0];
    if (board[0][2] != '_' && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return board[0][2];

    return '_'; // No winner yet
}

// Function to check if the board is full (tie condition)
bool isBoardFull() {
    for (const auto& row : board) {
        for (char cell : row) {
            if (cell == '_') return false;
        }
    }
    return true;
}

// AI move: Tries to win, then block player, otherwise plays optimally
void computerMove() {
    // Try to win
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == '_') {
                board[i][j] = 'O';
                if (checkWin() == 'O') return;
                board[i][j] = '_'; // Undo move
            }
        }
    }

    // Try to block player's win
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == '_') {
                board[i][j] = 'X';
                if (checkWin() == 'X') {
                    board[i][j] = 'O'; // Block player
                    return;
                }
                board[i][j] = '_'; // Undo move
            }
        }
    }

    // Otherwise, play the best move (center, then corners, then edges)
    if (board[1][1] == '_') {
        board[1][1] = 'O';
        return;
    }

    int moves[4][2] = {{0, 0}, {0, 2}, {2, 0}, {2, 2}};
    for (int k = 0; k < 4; k++) {
        int i = moves[k][0], j = moves[k][1];
        if (board[i][j] == '_') {
            board[i][j] = 'O';
            return;
        }
    }

    int edges[4][2] = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};
    for (int k = 0; k < 4; k++) {
        int i = edges[k][0], j = edges[k][1];
        if (board[i][j] == '_') {
            board[i][j] = 'O';
            return;
        }
    }
}

// Player move with validation
void playerMove() {
    int move;
    while (true) {
        cout << "Enter a position (1-9): ";
        cin >> move;

        if (move < 1 || move > 9) {
            cout << "Invalid input! Enter a number between 1 and 9.\n";
            continue;
        }

        int row = (move - 1) / 3;
        int col = (move - 1) % 3;

        if (board[row][col] == '_') {
            board[row][col] = 'X';
            break;
        } else {
            cout << "Cell already occupied! Choose another position.\n";
        }
    }
}

// Main game function
void playGame() {
    cout << "Welcome to Tic-Tac-Toe!" << endl;
    cout << "1 2 3\n4 5 6\n7 8 9\n";
    char playFirst;

    cout << "Do you want to play first? (y/n): ";
    cin >> playFirst;

    int moveCount = 0;
    if (playFirst == 'n') {
        cout << "Computer's move...\n";
        computerMove();
        display();
        moveCount++;
    }

    while (moveCount < 9) {
        playerMove();
        moveCount++;
        display();

        if (checkWin() == 'X') {
            cout << "You win!" << endl;
            return;
        }
        if (isBoardFull()) {
            cout << "It's a tie!" << endl;
            return;
        }

        cout << "Computer's move...\n";
        computerMove();
        moveCount++;
        display();

        if (checkWin() == 'O') {
            cout << "Computer wins!" << endl;
            return;
        }
        if (isBoardFull()) {
            cout << "It's a tie!" << endl;
            return;
        }
    }
}

int main() {
    playGame();
    return 0;
}