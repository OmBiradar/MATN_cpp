#include <bits/stdc++.h>

using namespace std;

class Graph{
protected:
    int n = 0; // number of nodes
    bool is_connected () {
        // uses dfs to update weather the graph has only 1 connected component
        vector<bool> visited(this->n);
        queue<int> q;
        q.push(0);
        visited[0] = true;
        int cnt = 1;
        while(q.size() != 0) {
            int cur_node = q.front();
            q.pop();

            for (int neighbour : adjacency_list[cur_node]) {
                if (!visited[neighbour]) {
                    visited[neighbour] = true;
                    cnt++;
                    q.push(neighbour);
                }
            }
        }
        cout << "Fully connected" << endl;
        return this->n == cnt;
    }
public:
    vector<vector<int>> adjacency_list; // adjacency list of the graph
    Graph(int num_nodes) : n(num_nodes) {  // Class constructor
        adjacency_list.resize(n);
    }
    void print_adjacency_list() {
        for (int i = 0; i < n; i++) {
            cout << i << " = [";
            for (int j = 0; j < this->adjacency_list[i].size(); j++) {
                cout << this->adjacency_list[i][j];
                if (j != this->adjacency_list[i].size() - 1) {
                    cout << ", ";
                }
            }
            cout << "]" << endl;
        }
    }
    vector<int> shortest_path(int start, int end) {
        if (start >= n || end >= n || start < 0 || end < 0) return {-1};
        
        // Distance, previous node, and visited arrays
        vector<int> distance(n, INT_MAX);
        vector<int> prev(n, -1);
        vector<bool> vis(n, false);

        // Min-heap priority queue: (distance, node)
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        // Initialize start node
        distance[start] = 0;
        pq.push({0, start});

        while (!pq.empty()) {
            int node = pq.top().second;
            int node_dist = pq.top().first;
            pq.pop();

            if (vis[node]) continue; // Skip already visited nodes
            vis[node] = true;

            // Process all neighbors of the current node
            for (int neighbour : adjacency_list[node]) {
                int update_dist = distance[node] + 1; // Assume weight = 1 for unweighted graph
                
                // Relax the edge
                if (update_dist < distance[neighbour]) {
                    distance[neighbour] = update_dist;
                    prev[neighbour] = node;
                    pq.push({distance[neighbour], neighbour});
                }
            }
        }

        // Reconstruct the shortest path
        vector<int> path;
        int node = end;

        if (distance[end] == INT_MAX) return {-1}; // No path exists

        while (node != -1) {
            path.push_back(node);
            node = prev[node];
        }
        reverse(path.begin(), path.end());
        return path;
    }

    const vector<vector<int>>& getAdjacencyList() const {
        return adjacency_list; // read-only access
    }
};

class WattsStrogatzGraph : public Graph {
protected:
    int k = 0;
    int p = 0;

public:
    WattsStrogatzGraph(int n, int neighbours, double rewiring_probability) : Graph(n), k(neighbours), p(rewiring_probability) {
        do {
            // Steps for generating a Watts-Strogatz Graph
            generateRingLattice();
            rewireEdges();
        } while (!is_connected());
    }

private:
    void generateRingLattice() {
        for (int i = 0; i < n; ++i) {
            for (int j = 1; j <= k / 2; ++j) {
                adjacency_list[i].push_back((i + j) % n); // Connect to right neighbor
                adjacency_list[i].push_back((i - j + n) % n); // Connect to left neighbor
            }
        }
    }

    void rewireEdges() {
        srand(time(0)); // Seed random number generator
        for (int i = 0; i < n; ++i) {
            for (int j = 1; j <= k / 2; ++j) {
                int neighbor = (i + j) % n;
                if ((rand() / (double)RAND_MAX) < p) { // Randomly decide to rewire
                    int newNeighbor;
                    do {
                        newNeighbor = rand() % n; // Select a random node
                    } while (newNeighbor == i || 
                             std::find(adjacency_list[i].begin(), adjacency_list[i].end(), newNeighbor) != adjacency_list[i].end());
                    
                    // Remove the old edge and add the new edge
                    adjacency_list[i].erase(std::remove(adjacency_list[i].begin(), adjacency_list[i].end(), neighbor), adjacency_list[i].end());
                    adjacency_list[i].push_back(newNeighbor);
                }
            }
        }
    }
};

class Network : public WattsStrogatzGraph {
protected:
    vector<vector<int>> active_matrix;
public:
    Network(int n, int neighbours, int rewiring_probability) : WattsStrogatzGraph(n, neighbours, rewiring_probability) {
        active_matrix.resize(n, vector<int>(n, -1));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < adjacency_list[i].size(); j++) {
                active_matrix[i][j] = 0;
            }
        }
    }
};