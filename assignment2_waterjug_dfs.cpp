#include <iostream>
#include <set>
#include <vector>
#include <algorithm> // for shuffle
#include <ctime> // for time() to seed random number generator

using namespace std;

struct State {
    int four, three; // Represents the water level in 4L and 3L jugs

    bool operator<(const State& other) const {
        return (four < other.four) || (four == other.four && three < other.three);
    }
};

// Global variables to track visited states and solution path
set<State> visited;
vector<State> solutionPath;

// DFS function to explore states
bool dfsWaterJug(State current) {
    if (visited.count(current)) return false; // Avoid cycles
    visited.insert(current);
    solutionPath.push_back(current);

    // Goal condition: 4L jug contains exactly 2L
    if (current.four == 2) {
        return true;
    }

    // Generate possible next moves
    vector<State> nextMoves = {
        {4, current.three}, // Fill 4L jug
        {current.four, 3},  // Fill 3L jug
        {0, current.three}, // Empty 4L jug
        {current.four, 0},  // Empty 3L jug
        {max(0, current.four - (3 - current.three)), min(3, current.three + current.four)}, // Pour 4L → 3L
        {min(4, current.four + current.three), max(0, current.three - (4 - current.four))}  // Pour 3L → 4L
    };

    // Shuffle the next moves to explore in a random order
    srand(time(0)); // Seed random number generator
    random_shuffle(nextMoves.begin(), nextMoves.end()); 

    // Try each next state after shuffling
    for (auto next : nextMoves) {
        if (dfsWaterJug(next)) return true; // If solution found, stop recursion
    }

    solutionPath.pop_back(); // Backtrack if this path doesn't lead to a solution
    return false;
}

int main() {
    State initial = {0, 0}; // Start from (0,0)
    
    if (dfsWaterJug(initial)) {
        cout << "Solution found! Path:\n";
        for (const auto& state : solutionPath) {
            cout << "(" << state.four << ", " << state.three << ")\n";
        }
    } else {
        cout << "No solution found.\n";
    }

    return 0;
}