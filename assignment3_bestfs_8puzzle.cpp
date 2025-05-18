#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <cmath>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

vector<vector<int>> goal; //global goal state 

class PuzzleState {
public:
    vector<vector<int>> board;
    int cost;
    PuzzleState* parent;

    PuzzleState(vector<vector<int>> b, PuzzleState* p) {
        board = b;
        parent = p;
        cost = calculateHeuristic();
    }

    // Manhattan Distance Heuristic
    int calculateHeuristic() {
        int distance = 0;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] != 0) {
                    int value = board[i][j];
                    int goalX = -1, goalY = -1;
                    for (int x = 0; x < 3; ++x)
                        for (int y = 0; y < 3; ++y)
                            if (goal[x][y] == value) {
                                goalX = x;
                                goalY = y;
                            }
                    distance += abs(i - goalX) + abs(j - goalY);
                }
            }
        return distance;
    }

    bool isGoal() {
        return board == goal;
    }

    string boardToString() const {
        stringstream ss;
        for (auto& row : board)
            for (int num : row)
                ss << num;
        return ss.str();
    }

    void printBoard() const {
        for (auto& row : board) {
            for (int num : row)
                cout << num << " ";
            cout << endl;
        }
    }

    void printBoardWithCost() const {
        printBoard();
        cout << "---------\n";
        cout << "Cost (Manhattan distance): " << cost << "\n\n";
    }

    vector<PuzzleState*> generateSuccessors() {
        vector<PuzzleState*> successors;
        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};
        int zeroX = -1, zeroY = -1;

        // Find empty space
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                if (board[i][j] == 0)
                    zeroX = i, zeroY = j;

        // Move in 4 directions
        for (int k = 0; k < 4; ++k) {
            int newX = zeroX + dx[k];
            int newY = zeroY + dy[k];
            if (newX >= 0 && newX < 3 && newY >= 0 && newY < 3) {
                vector<vector<int>> newBoard = board;
                swap(newBoard[zeroX][zeroY], newBoard[newX][newY]);
                successors.push_back(new PuzzleState(newBoard, this));
            }
        }
        return successors;
    }
};

// Comparison for priority queue
struct ComparePuzzle {
    bool operator()(const PuzzleState* a, const PuzzleState* b) const {
        return a->cost > b->cost;
    }
};

bool isSolvable(const vector<vector<int>>& board) {
    vector<int> arr;
    for (auto& row : board)
        for (int val : row)
            arr.push_back(val);

    int inv = 0;
    for (int i = 0; i < 9; ++i)
        for (int j = i + 1; j < 9; ++j)
            if (arr[i] && arr[j] && arr[i] > arr[j])
                inv++;

    return inv % 2 == 0;
}

void printSolutionPath(PuzzleState* state) {
    vector<PuzzleState*> path;
    while (state) {
        path.push_back(state);
        state = state->parent;
    }
    reverse(path.begin(), path.end());
    cout << "Solution steps:\n";
    for (int i = 0; i < path.size(); ++i) {
        cout << "Step " << i << ":\n---------\n";
        path[i]->printBoardWithCost();
    }
    cout << "Total moves: " << (path.size() - 1) << endl;
}

void solvePuzzle(const vector<vector<int>>& initialBoard) {
    priority_queue<PuzzleState*, vector<PuzzleState*>, ComparePuzzle> openList;
    set<string> visited;

    PuzzleState* start = new PuzzleState(initialBoard, nullptr);
    openList.push(start);
    visited.insert(start->boardToString());

    int steps = 0;

    while (!openList.empty()) {
        PuzzleState* current = openList.top();
        openList.pop();
        steps++;

        cout << "Exploring state #" << steps << ":\n";
        if (current->parent) {
            cout << "Derived from:\n";
            current->parent->printBoard();
            cout << endl;
        }
        current->printBoardWithCost();

        if (current->isGoal()) {
            cout << "Goal state reached!\n";
            printSolutionPath(current);
            cout << "Total states explored: " << steps << endl;
            return;
        }

        for (PuzzleState* neighbor : current->generateSuccessors()) {
            string hash = neighbor->boardToString();
            if (!visited.count(hash)) {
                visited.insert(hash);
                openList.push(neighbor);
            }
        }
    }
    cout << "No solution found.\n";
}

// Main Function
int main() {
    vector<vector<int>> initial(3, vector<int>(3));
    goal = vector<vector<int>>(3, vector<int>(3));

    cout << "Enter the initial state (0 for empty space):\n";
    for (int i = 0; i < 3; ++i) {
        cout << "Row " << i + 1 << ": ";
        for (int j = 0; j < 3; ++j)
            cin >> initial[i][j];
    }

    cout << "\nEnter the goal state:\n";
    for (int i = 0; i < 3; ++i) {
        cout << "Row " << i + 1 << ": ";
        for (int j = 0; j < 3; ++j)
            cin >> goal[i][j];
    }

    if (!isSolvable(initial)) {
        cout << "This puzzle is unsolvable.\n";
        return 0;
    }

    cout << "\nInitial board:\n";
    PuzzleState initialState(initial, nullptr);
    initialState.printBoardWithCost();

    cout << "Solving using Best-First Search...\n\n";
    solvePuzzle(initial);

    return 0;
}