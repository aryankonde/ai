#include <iostream>
#include <queue>
#include <set>
#include <tuple> 

using namespace std;

struct State {
    int four, three;

    bool operator<(const State& other) const {
        return tie(four, three) < tie(other.four, other.three);
    }
};

void bfsWaterJug() {
    queue<State> q;
    set<State> visited;

    q.push({0, 0}); // Initial state (0,0)

    while (!q.empty()) {
        State current = q.front();
        q.pop();

        if (visited.count(current)) continue;
        visited.insert(current);

        cout << "(" << current.four << ", " << current.three << ")\n";

        // Goal condition
        if (current.four == 2) {
            cout << "Solution found!\n";
            return;
        }

        // Possible moves:
        vector<State> nextMoves = {
            {4, current.three}, 
            {current.four, 3},  
            {0, current.three}, 
            {current.four, 0},  
            {max(0, current.four - (3 - current.three)), min(3, current.three + current.four)}, 
            {min(4, current.four + current.three), max(0, current.three - (4 - current.four))}  
        };

        for (auto next : nextMoves) {
            if (!visited.count(next)) q.push(next);
        }
    }

    cout << "No solution found.\n";
}

int main() {
    bfsWaterJug();
    return 0;
}