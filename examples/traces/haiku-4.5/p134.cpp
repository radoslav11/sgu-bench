#include<bits/stdc++.h>
using namespace std;

vector<int> adj[16005];
bool visited[16005];
int N;

int dfs(int node, int removed) {
    visited[node] = true;
    int size = 1;
    for(int neighbor : adj[node]) {
        if(neighbor != removed && !visited[neighbor]) {
            size += dfs(neighbor, removed);
        }
    }
    return size;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N;
    
    for(int i = 0; i < N-1; i++) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    
    int minValue = INT_MAX;
    vector<int> centroids;
    
    // For each vertex k
    for(int k = 1; k <= N; k++) {
        // Find the largest component after removing k
        memset(visited, false, sizeof(visited));
        visited[k] = true; // Mark k as visited so we don't traverse it
        
        int maxComponentSize = 0;
        
        // Start DFS from each neighbor of k
        for(int neighbor : adj[k]) {
            if(!visited[neighbor]) {
                int componentSize = dfs(neighbor, k);
                maxComponentSize = max(maxComponentSize, componentSize);
            }
        }
        
        // Update the result
        if(maxComponentSize < minValue) {
            minValue = maxComponentSize;
            centroids.clear();
            centroids.push_back(k);
        } else if(maxComponentSize == minValue) {
            centroids.push_back(k);
        }
    }
    
    cout << minValue << " " << centroids.size() << "\n";
    for(int i = 0; i < centroids.size(); i++) {
        if(i > 0) cout << " ";
        cout << centroids[i];
    }
    cout << "\n";
    
    return 0;
}
