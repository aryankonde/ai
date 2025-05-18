#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <sstream>
#include <algorithm>
#include <queue>  // include queue for BFS

using namespace std;

int goalBoard[3][3];

struct PuzzleState {
    int board[3][3];
    int emptyRow, emptyCol;
    int depth;
    PuzzleState* parent;
    string moveDirection;

    PuzzleState(int b[3][3], int d, PuzzleState* p, string dir) {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j) {
                board[i][j] = b[i][j];
                if (b[i][j] == 0) {
                    emptyRow = i;
                    emptyCol = j;
                }
            }
        depth = d;
        parent = p;
        moveDirection = dir;
    }

    void printBoard() const {
        cout << "---------\n";
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j)
                cout << (board[i][j] == 0 ? "  " : to_string(board[i][j]) + " ");
            cout << "\n";
        }
        cout << "---------\n";
        cout << "Depth: " << depth << "\n";
        if (!moveDirection.empty() && moveDirection != "start")
            cout << "Move: " << moveDirection << "\n\n";
    }

    string getBoardString() const {
        stringstream ss;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                ss << board[i][j];
        return ss.str();
    }
};

bool isGoalReached(int board[3][3]) {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board[i][j] != goalBoard[i][j])
                return false;
    return true;
}

vector<PuzzleState*> getNextMoves(PuzzleState* current) {
    vector<PuzzleState*> nextStates;
    int dirs[4][2] = { {-1,0},{1,0},{0,-1},{0,1} };
    string dirNames[4] = { "up", "down", "left", "right" };

    for (int i = 0; i < 4; ++i) {
        int newRow = current->emptyRow + dirs[i][0];
        int newCol = current->emptyCol + dirs[i][1];

        if (newRow >= 0 && newRow < 3 && newCol >= 0 && newCol < 3) {
            int newBoard[3][3];
            for (int r = 0; r < 3; ++r)
                for (int c = 0; c < 3; ++c)
                    newBoard[r][c] = current->board[r][c];
            swap(newBoard[current->emptyRow][current->emptyCol], newBoard[newRow][newCol]);
            PuzzleState* newState = new PuzzleState(newBoard, current->depth + 1, current, dirNames[i]);
            nextStates.push_back(newState);
        }
    }

    return nextStates;
}

void showSolution(PuzzleState* goalState, int statesExplored) {
    vector<PuzzleState*> path;
    while (goalState) {
        path.push_back(goalState);
        goalState = goalState->parent;
    }
    reverse(path.begin(), path.end());

    cout << "\nSolution requires " << (path.size() - 1) << " moves (explored " << statesExplored << " states).\n";
    for (size_t i = 0; i < path.size(); ++i) {
        cout << "\nStep " << i << ":\n";
        path[i]->printBoard();
    }
}

bool isSolvable(int board[3][3]) {
    vector<int> flat;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            flat.push_back(board[i][j]);

    int inv = 0;
    for (int i = 0; i < 9; ++i) {
        if (flat[i] == 0) continue;
        for (int j = i + 1; j < 9; ++j)
            if (flat[j] != 0 && flat[i] > flat[j])
                inv++;
    }
    return inv % 2 == 0;
}

void readBoard(int board[3][3], const string& name) {
    cout << "Enter " << name << " state (row-wise, use 0 for blank):\n";
    bool used[9] = { false };
    for (int i = 0; i < 3; ++i) {
        cout << "Row " << i + 1 << ": ";
        for (int j = 0; j < 3; ++j) {
            cin >> board[i][j];
            if (board[i][j] < 0 || board[i][j] > 8 || used[board[i][j]]) {
                cout << "Invalid or duplicate number!\n";
                exit(1);
            }
            used[board[i][j]] = true;
        }
    }
}

void solveWithBFS(int initialBoard[3][3]) {
    cout << "Starting BFS search...\n";

    PuzzleState* start = new PuzzleState(initialBoard, 0, nullptr, "start");

    queue<PuzzleState*> queue;
    set<string> visited;

    queue.push(start);
    int statesExplored = 0;

    while (!queue.empty()) {
        PuzzleState* current = queue.front();
        queue.pop();
        statesExplored++;

        if (isGoalReached(current->board)) {
            cout << "\nSolution found!\n";
            showSolution(current, statesExplored);
            return;
        }

        string boardStr = current->getBoardString();
        if (visited.count(boardStr)) continue;
        visited.insert(boardStr);

        vector<PuzzleState*> moves = getNextMoves(current);
        // No reverse here for BFS, we want to explore in natural order

        for (auto next : moves) {
            if (!visited.count(next->getBoardString()))
                queue.push(next);
        }
    }

    cout << "No solution found after exploring " << statesExplored << " states.\n";
}

int main() {
    int initialBoard[3][3];

    readBoard(initialBoard, "initial");
    readBoard(goalBoard, "goal");

    if (!isSolvable(initialBoard)) {
        cout << "This puzzle is not solvable!\n";
        return 0;
    }

    solveWithBFS(initialBoard);
    return 0;
}
