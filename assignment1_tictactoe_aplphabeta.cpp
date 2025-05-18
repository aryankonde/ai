#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// The Tic-Tac-Toe board
char board[3][3] = {
    {' ', ' ', ' '},
    {' ', ' ', ' '},
    {' ', ' ', ' '}
};

// Function to print the board
void printBoard() {
    cout << "-------------" << endl;
    for (int i = 0; i < 3; i++) {
        cout << "| ";
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                cout << (i * 3 + j) << " | ";
            } else {
                cout << board[i][j] << " | ";
            }
        }
        cout << endl << "-------------" << endl;
    }
}

// Function to check if the game is a draw
bool checkDraw() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                return false;
            }
        }
    }
    return true;
}

// Evaluate the board for the Minimax algorithm
int evaluate() {
    // Check rows and columns for winning condition
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            if (board[i][0] == 'O') return 1;
            if (board[i][0] == 'X') return -1;
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            if (board[0][i] == 'O') return 1;
            if (board[0][i] == 'X') return -1;
        }
    }

    // Check diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == 'O') return 1;
        if (board[0][0] == 'X') return -1;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == 'O') return 1;
        if (board[0][2] == 'X') return -1;
    }

    return 0;
}

// Alpha-Beta Pruning for the Minimax algorithm
int alphaBeta(int depth, bool isMax, int alpha, int beta) {
    int score = evaluate();

    // If the AI wins
    if (score == 1) return score;

    // If the player wins
    if (score == -1) return score;

    // If the board is full and no winner
    if (checkDraw()) return 0;

    // Maximizing for AI (O)
    if (isMax) {
        int best = -1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'O';
                    best = max(best, alphaBeta(depth + 1, false, alpha, beta));
                    board[i][j] = ' ';
                    alpha = max(alpha, best);
                    if (beta <= alpha) break;
                }
            }
        }
        return best;
    }
    // Minimizing for player (X)
    else {
        int best = 1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'X';
                    best = min(best, alphaBeta(depth + 1, true, alpha, beta));
                    board[i][j] = ' ';
                    beta = min(beta, best);
                    if (beta <= alpha) break;
                }
            }
        }
        return best;
    }
}

// Function to find the best move for AI
pair<int, int> findBestMove() {
    int bestVal = -1000;
    pair<int, int> bestMove = {-1, -1};

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = 'O';
                int moveVal = alphaBeta(0, false, -1000, 1000);
                board[i][j] = ' ';
                if (moveVal > bestVal) {
                    bestMove.first = i;
                    bestMove.second = j;
                    bestVal = moveVal;
                }
            }
        }
    }
    return bestMove;
}

// Function to convert block number (0-8) to row and column
pair<int, int> blockToCoordinates(int block) {
    return {block / 3, block % 3};
}

int main() {
    int block;
    cout << "Welcome to Tic Tac Toe!" << endl;
    cout << "Board positions are numbered 0-8 as follows:" << endl;
    printBoard();

    while (true) {
        // Player move
        cout << "Enter your move (block number 0-8): ";
        cin >> block;

        if (block < 0 || block > 8) {
            cout << "Invalid block number. Please enter a number between 0-8." << endl;
            continue;
        }

        pair<int, int> coords = blockToCoordinates(block);
        int row = coords.first;
        int col = coords.second;

        if (board[row][col] != ' ') {
            cout << "Invalid move. This position is already taken. Try again." << endl;
            continue;
        }

        board[row][col] = 'X';  // Player's move
        printBoard();

        // Check for player win or draw
        if (evaluate() == -1) {
            cout << "You win!" << endl;
            break;
        }
        if (checkDraw()) {
            cout << "It's a draw!" << endl;
            break;
        }

        // AI move
        cout << "AI is making a move..." << endl;
        pair<int, int> bestMove = findBestMove();
        board[bestMove.first][bestMove.second] = 'O';
        printBoard();

        // Check for AI win or draw
        if (evaluate() == 1) {
            cout << "AI wins!" << endl;
            break;
        }
        if (checkDraw()) {
            cout << "It's a draw!" << endl;
            break;
        }
    }

    return 0;
}