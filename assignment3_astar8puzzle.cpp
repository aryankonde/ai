#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <cmath>
#include <sstream>
using namespace std;

int goalBoard[3][3];

struct PuzzleState {
    int board[3][3];
    int emptyRow, emptyCol;
    int depth; // g(n) = depth of the node
    int heuristic; // h(n) = Manhattan distance
    int totalCost; // f(n) = g(n) + h(n)
    PuzzleState* parent;
    string moveDirection;

    PuzzleState(int b[3][3], int d, int h, PuzzleState* p, string dir) {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++) {
                board[i][j] = b[i][j];
                if (board[i][j] == 0) {
                    emptyRow = i;
                    emptyCol = j;
                }
            }
        depth = d;
        heuristic = h;
        totalCost = depth + heuristic;
        parent = p;
        moveDirection = dir;
    }

    void printBoardWithCost() const {
        cout << "---------\n";
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++)
                cout << (board[i][j] == 0 ? "  " : to_string(board[i][j]) + " ");
            cout << "\n";
        }
        cout << "---------\n";
        cout << "Depth (g(n)): " << depth << "\n";
        cout << "Heuristic (h(n)): " << heuristic << "\n";
        cout << "Total cost (f(n) = g + h): " << totalCost << "\n";
        if (!moveDirection.empty() && moveDirection != "start")
            cout << "Move: " << moveDirection << "\n";
        cout << "\n";
    }

    string getBoardString() const {
        stringstream ss;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                ss << board[i][j];
        return ss.str();
    }
};

struct CompareCost {
    bool operator()(const PuzzleState* a, const PuzzleState* b) {
        return a->totalCost > b->totalCost;
    }
};

int getManhattanDistance(int board[3][3]) {
    int distance = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            int value = board[i][j];
            if (value == 0) continue;

            for (int gi = 0; gi < 3; gi++) {
                for (int gj = 0; gj < 3; gj++) {
                    if (goalBoard[gi][gj] == value) {
                        distance += abs(i - gi) + abs(j - gj);
                        break;
                    }
                }
            }
        }
    return distance;
}

bool isGoalReached(int board[3][3]) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
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
            for (int r = 0; r < 3; r++)
                for (int c = 0; c < 3; c++)
                    newBoard[r][c] = current->board[r][c];

            swap(newBoard[current->emptyRow][current->emptyCol], newBoard[newRow][newCol]);

            int h = getManhattanDistance(newBoard);
            PuzzleState* newState = new PuzzleState(newBoard, current->depth + 1, h, current, dirNames[i]);
            nextStates.push_back(newState);
        }
    }

    return nextStates;
}

bool isSolvable(int board[3][3]) {
    vector<int> flat;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            flat.push_back(board[i][j]);

    int inv = 0;
    for (int i = 0; i < 9; i++) {
        if (flat[i] == 0) continue;
        for (int j = i + 1; j < 9; j++)
            if (flat[j] != 0 && flat[i] > flat[j])
                inv++;
    }

    return inv % 2 == 0;
}

void showSolution(PuzzleState* goalState, int statesExplored) {
    vector<PuzzleState*> path;
    PuzzleState* current = goalState;
    while (current) {
        path.push_back(current);
        current = current->parent;
    }
    reverse(path.begin(), path.end());

    cout << "\nSolution requires " << (path.size() - 1) << " moves (explored " << statesExplored << " states).\n";
    for (size_t i = 0; i < path.size(); ++i) {
        cout << "\nStep " << i << ":\n";
        path[i]->printBoardWithCost();
    }
}

void solveWithAStar(int initialBoard[3][3]) {
    cout << "Starting A* search...\n";

    int h = getManhattanDistance(initialBoard);
    PuzzleState* start = new PuzzleState(initialBoard, 0, h, nullptr, "start");

    priority_queue<PuzzleState*, vector<PuzzleState*>, CompareCost> openSet;
    set<string> visited;

    openSet.push(start);
    int statesExplored = 0;

    while (!openSet.empty()) {
        PuzzleState* current = openSet.top();
        openSet.pop();
        statesExplored++;

        cout << "Exploring state #" << statesExplored << ":\n";
        if (current->parent)
            cout << "Derived from:\n", current->parent->printBoardWithCost();
        current->printBoardWithCost();

        if (isGoalReached(current->board)) {
            cout << "\nSolution found!\n";
            showSolution(current, statesExplored);
            return;
        }

        string boardStr = current->getBoardString();

        if (visited.count(boardStr)) continue;
        
        visited.insert(boardStr);

        for (auto next : getNextMoves(current)) {
            string nextStr = next->getBoardString();
            if (!visited.count(nextStr))
                openSet.push(next);
        }
    }

    cout << "No solution found after exploring " << statesExplored << " states.\n";
}

void readBoard(int board[3][3], const string& name) {
    cout << "Enter " << name << " state (row-wise, use 0 for blank):\n";
    bool used[9] = { false };
    for (int i = 0; i < 3; i++) {
        cout << "Row " << i + 1 << ": ";
        for (int j = 0; j < 3; j++) {
            cin >> board[i][j];
            if (board[i][j] < 0 || board[i][j] > 8 || used[board[i][j]]) {
                cout << "Invalid or duplicate number!\n";
                exit(1);
            }
            used[board[i][j]] = true;
        }
    }
}

int main() {
    int initialBoard[3][3];

    readBoard(initialBoard, "initial");
    readBoard(goalBoard, "goal");

    if (!isSolvable(initialBoard)) {
        cout << "This puzzle is not solvable!\n";
        return 0;
    }

    solveWithAStar(initialBoard);
    return 0;
}
