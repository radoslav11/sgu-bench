#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

int main() {
    int N;
    cin >> N;
    
    vector<int> w(N), r(N);
    for (int i = 0; i < N; i++) {
        cin >> w[i];
    }
    for (int i = 0; i < N; i++) {
        cin >> r[i];
    }
    
    vector<vector<int>> a(N, vector<int>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> a[i][j];
        }
    }
    
    // Target wins for team 1 (index 0)
    int target = w[0] + r[0];
    
    // Check if any team already has more wins than target
    for (int i = 1; i < N; i++) {
        if (w[i] > target) {
            cout << "NO" << endl;
            return 0;
        }
    }
    
    // Calculate available wins for other teams (indices 1 to N-1)
    vector<int> cap(N, 0); // cap[i] for team i
    for (int i = 1; i < N; i++) {
        int games_with_team1 = a[0][i];
        int remaining_games = r[i] - games_with_team1;
        int max_additional = target - w[i];
        cap[i] = min(remaining_games, max_additional);
        if (cap[i] < 0) {
            cout << "NO" << endl;
            return 0;
        }
    }
    
    // Calculate total games among teams 2..N (indices 1 to N-1)
    int total_games = 0;
    for (int i = 1; i < N; i++) {
        for (int j = i+1; j < N; j++) {
            total_games += a[i][j];
        }
    }
    
    // If no games among others, then it's possible if all caps are non-negative (which they are)
    if (total_games == 0) {
        cout << "YES" << endl;
        return 0;
    }
    
    // Build list of pairs (i, j) with 1<=i<j<=N-1 and a[i][j] > 0
    vector<pair<int, int>> pairs;
    for (int i = 1; i < N; i++) {
        for (int j = i+1; j < N; j++) {
            if (a[i][j] > 0) {
                pairs.push_back({i, j});
            }
        }
    }
    int num_pairs = pairs.size();
    
    // Total nodes: 1 (source) + num_pairs (game nodes) + (N-1) (team nodes) + 1 (sink)
    int total_nodes = 1 + num_pairs + (N-1) + 1;
    int source = 0;
    int sink = total_nodes - 1;
    
    // Create capacity matrix
    vector<vector<int>> capacity(total_nodes, vector<int>(total_nodes, 0));
    
    // Map:
    // source: 0
    // game nodes: 1 to num_pairs
    // team nodes: num_pairs+1 to num_pairs + (N-1)
    // sink: total_nodes-1
    
    for (int k = 0; k < num_pairs; k++) {
        int i = pairs[k].first;
        int j = pairs[k].second;
        int game_node = 1 + k;
        int team_i_node = 1 + num_pairs + (i - 1);
        int team_j_node = 1 + num_pairs + (j - 1);
        
        // Source to game node
        capacity[source][game_node] = a[i][j];
        // Game node to teams
        capacity[game_node][team_i_node] = a[i][j];
        capacity[game_node][team_j_node] = a[i][j];
        // Teams to sink
        capacity[team_i_node][sink] = cap[i];
        capacity[team_j_node][sink] = cap[j];
    }
    
    // Edmonds-Karp algorithm
    int max_flow = 0;
    
    while (true) {
        // BFS to find augmenting path
        vector<int> parent(total_nodes, -1);
        queue<int> q;
        q.push(source);
        parent[source] = -2;
        
        while (!q.empty() && parent[sink] == -1) {
            int u = q.front();
            q.pop();
            for (int v = 0; v < total_nodes; v++) {
                if (parent[v] == -1 && capacity[u][v] > 0) {
                    parent[v] = u;
                    q.push(v);
                }
            }
        }
        
        if (parent[sink] == -1) {
            break; // no more augmenting paths
        }
        
        // Find min capacity on the path
        int path_flow = INT_MAX;
        int s = sink;
        while (s != source) {
            int prev = parent[s];
            path_flow = min(path_flow, capacity[prev][s]);
            s = prev;
        }
        
        // Update residual capacities
        s = sink;
        while (s != source) {
            int prev = parent[s];
            capacity[prev][s] -= path_flow;
            capacity[s][prev] += path_flow;
            s = prev;
        }
        
        max_flow += path_flow;
    }
    
    if (max_flow == total_games) {
        cout << "YES" << endl;
    } else {
        cout << "NO" << endl;
    }
    
    return 0;
}
