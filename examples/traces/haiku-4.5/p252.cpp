#include<bits/stdc++.h>
using namespace std;

int N, M;
vector<pair<int,int>> adj[105]; // {next_node, cost}
vector<tuple<vector<int>, int>> paths; // {path, cost}
bool visited[105];
int best_cost;
vector<vector<int>> best_paths;

void findAllPaths(int u, vector<int>& current_path, int current_cost, set<pair<int,int>>& used_edges) {
    current_path.push_back(u);
    
    bool found_next = false;
    for(auto [v, cost] : adj[u]) {
        if(used_edges.find({u, v}) == used_edges.end()) {
            found_next = true;
            used_edges.insert({u, v});
            findAllPaths(v, current_path, current_cost + cost, used_edges);
            used_edges.erase({u, v});
        }
    }
    
    if(!found_next) {
        // End of path - store it
        paths.push_back({current_path, current_cost});
    }
    
    current_path.pop_back();
}

void solve(int path_idx, vector<bool>& covered, int num_paths, int total_cost, 
           vector<vector<int>>& current_solution, set<pair<int,int>>& used_edges) {
    
    // Check if all vertices are covered
    bool all_covered = true;
    int first_uncovered = -1;
    for(int i = 1; i <= N; i++) {
        if(!covered[i]) {
            all_covered = false;
            first_uncovered = i;
            break;
        }
    }
    
    if(all_covered) {
        if(num_paths < best_paths.size() || 
           (num_paths == best_paths.size() && total_cost < best_cost)) {
            best_paths = current_solution;
            best_cost = total_cost;
        }
        return;
    }
    
    // Pruning
    if(num_paths >= best_paths.size()) return;
    
    // Try to extend from first uncovered vertex
    vector<int> path;
    set<pair<int,int>> local_used = used_edges;
    findAllPaths(first_uncovered, path, 0, local_used);
    
    for(auto [p, cost] : paths) {
        if(p[0] != first_uncovered) continue;
        
        // Check if this path uses available edges and unvisited nodes
        bool valid = true;
        for(int i = 0; i < p.size() - 1; i++) {
            if(used_edges.find({p[i], p[i+1]}) != used_edges.end()) {
                valid = false;
                break;
            }
        }
        for(int i = 1; i < p.size(); i++) {
            if(covered[p[i]]) {
                valid = false;
                break;
            }
        }
        
        if(valid) {
            vector<bool> new_covered = covered;
            for(int node : p) new_covered[node] = true;
            
            set<pair<int,int>> new_used = used_edges;
            for(int i = 0; i < p.size() - 1; i++) {
                new_used.insert({p[i], p[i+1]});
            }
            
            current_solution.push_back(p);
            solve(path_idx + 1, new_covered, num_paths + 1, total_cost + cost, current_solution, new_used);
            current_solution.pop_back();
        }
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N >> M;
    
    for(int i = 0; i < M; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        adj[a].push_back({b, c});
    }
    
    best_cost = INT_MAX;
    vector<bool> covered(N + 1, false);
    vector<vector<int>> current;
    set<pair<int,int>> used;
    
    // Find all possible paths first
    for(int start = 1; start <= N; start++) {
        vector<int> path;
        set<pair<int,int>> temp_used;
        findAllPaths(start, path, 0, temp_used);
    }
    
    solve(0, covered, 0, 0, current, used);
    
    cout << best_paths.size() << " " << best_cost << "\n";
    for(auto& path : best_paths) {
        cout << path.size();
        for(int node : path) cout << " " << node;
        cout << "\n";
    }
    
    return 0;
}
