#include <bits/stdc++.h>
using namespace std;

class Graph {
public:
    unordered_map<int, list<pair<int, pair<int, int>>>> adjList;

    void addEdges(int a, int cost, int heuristic, int b) {
        pair<int, int> h_end = {heuristic, b};
        pair<int, pair<int, int>> p = {cost, h_end};
        adjList[a].push_back(p);
    }

    void print_graph() {
        for (auto &i : adjList) {
            cout << i.first << " -> ";
            for (auto &j : i.second) {
                cout << j.second.second << "(cost=" << j.first << ", h=" << j.second.first << ") , ";
            }
            cout << endl;
        }
    }

    void print_open_list(priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq) {
        cout << "OPEN List: ";
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> temp = pq;
        while (!temp.empty()) {
            auto p = temp.top();
            cout << p.second << "(h=" << p.first << ") ";
            temp.pop();
        }
        cout << endl;
    }

    void print_closed_list(queue<int> CLOSED) {
        cout << "CLOSED List: ";
        queue<int> temp = CLOSED;
        while (!temp.empty()) {
            cout << temp.front() << " ";
            temp.pop();
        }
        cout << endl;
    }

    void print_path(unordered_map<int, int> &parent, int start, int goal) {
        vector<int> path;
        int node = goal;
        while (node != start) {
            path.push_back(node);
            node = parent[node];
        }
        path.push_back(start);

        cout << "Path: ";
        for (int i = path.size() - 1; i >= 0; --i) {
            cout << path[i];
            if (i != 0)
                cout << " -> ";
        }
        cout << endl;
    }

    void bestFirstSearch(int start, int goal) {
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> OPEN;
        unordered_map<int, bool> visited;
        unordered_map<int, int> parent;
        queue<int> CLOSED;

        OPEN.push({99999, start});
        visited[start] = true;

        while (!OPEN.empty()) {
            print_open_list(OPEN);

            auto current = OPEN.top();
            OPEN.pop();
            CLOSED.push(current.second);
            cout << "Visiting Node " << current.second << " with Heuristic value: " << current.first << endl;

            int currNode = current.second;

            if (currNode == goal) {
                cout << "Goal node " << goal << " reached." << endl;
                print_closed_list(CLOSED);
                print_path(parent, start, goal);
                return;
            }

            for (auto &neighbor : adjList[currNode]) {
                int nextNode = neighbor.second.second;
                int nextHeuristic = neighbor.second.first;

                if (!visited[nextNode]) {
                    OPEN.push({nextHeuristic, nextNode});
                    visited[nextNode] = true;
                    parent[nextNode] = currNode; // Track the parent
                }
            }
        }
        cout << "Goal node " << goal << " not reachable from start node " << start << "." << endl;
    }
};

int main() {
    Graph g;
    int nodes, edges;
    cout << "Enter Number of nodes: ";
    cin >> nodes;
    cout << "Enter Number of edges: ";
    cin >> edges;

    int m, n, cost, h;
    for (int i = 0; i < edges; i++) {
        cout << "Enter Starting Node, edge cost, heuristic value of ending node, and Ending Node for each path (start cost heuristic end): " << endl;
        cin >> m >> cost >> h >> n;
        g.addEdges(m, cost, h, n);
    }

    g.print_graph();

    int startNode, goalNode;
    cout << "Enter the start node: ";
    cin >> startNode;
    cout << "Enter the goal node: ";
    cin >> goalNode;

    cout << "Running Best First Search...\n";
    g.bestFirstSearch(startNode, goalNode);

    return 0;
}
