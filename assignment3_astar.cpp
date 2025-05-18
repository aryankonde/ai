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
            cout << p.second << "(f=" << p.first << ") ";
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

    void AStarSearch(int start, int goal, unordered_map<int, int> &g) {
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> OPEN;
        unordered_map<int, bool> visited;
        unordered_map<int, int> f, parent;
        queue<int> CLOSED;

        OPEN.push({g[start], start});
        visited[start] = true;
        f[start] = g[start];

        while (!OPEN.empty()) {
            print_open_list(OPEN);

            auto current = OPEN.top();
            OPEN.pop();
            CLOSED.push(current.second);
            cout << "Visiting Node " << current.second << " with f value: " << current.first << endl;

            int currNode = current.second;

            if (currNode == goal) {
                cout << "Goal node " << goal << " reached." << endl;
                print_closed_list(CLOSED);
                print_path(parent, start, goal);
                return;
            }

            for (auto &neighbor : adjList[currNode]) {
                int nextNode = neighbor.second.second;
                int heuristic = neighbor.second.first;
                int edgeCost = neighbor.first;

                int tentative_g = g[currNode] + edgeCost;
                if (!visited[nextNode] || tentative_g < g[nextNode]) {
                    parent[nextNode] = currNode;
                    g[nextNode] = tentative_g;
                    f[nextNode] = g[nextNode] + heuristic; // f(n) = g(n) + h(n)
                    OPEN.push({f[nextNode], nextNode});
                    visited[nextNode] = true;
                }
            }
        }
        cout << "Goal node " << goal << " not reachable from start node " << start << "." << endl;
    }
};

int main() {
    Graph g;
    unordered_map<int, int> g_value;
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
        g_value[m] = INT_MAX;
        g_value[n] = INT_MAX;
    }

    g.print_graph();

    int startNode, goalNode;
    cout << "Enter the start node: ";
    cin >> startNode;
    cout << "Enter the goal node: ";
    cin >> goalNode;

    cout << "Running A* Search...\n";
    g_value[startNode] = 0;
    g.AStarSearch(startNode, goalNode, g_value);

    return 0;
}
