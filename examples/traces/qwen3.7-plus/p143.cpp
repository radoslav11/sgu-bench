#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXN = 16005;
vector<int> adj[MAXN];
int profit_val[MAXN];
int parent_arr[MAXN];
int order[MAXN];
int dp[MAXN];

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N;
    if (!(cin >> N)) return 0;
    
    for (int i = 1; i <= N; i++) {
        cin >> profit_val[i];
    }
    
    for (int i = 0; i < N - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    // BFS to establish parent-child relationships and a reverse-topological order
    int head = 0, tail = 0;
    order[tail++] = 1;
    parent_arr[1] = 0;
    
    while (head < tail) {
        int u = order[head++];
        for (int v : adj[u]) {
            if (v != parent_arr[u]) {
                parent_arr[v] = u;
                order[tail++] = v;
            }
        }
    }
    
    // dp[u] will store the max profit of a connected subset in the subtree rooted at u
    // such that the subset includes the node u itself.
    for (int i = 1; i <= N; i++) {
        dp[i] = profit_val[i];
    }
    
    int max_profit = -2000000000;
    
    // Process nodes in reverse BFS order (bottom-up)
    for (int i = N - 1; i >= 0; i--) {
        int u = order[i];
        
        // Update the global maximum profit
        if (dp[u] > max_profit) {
            max_profit = dp[u];
        }
        
        // If the current node has a parent and its dp value is positive, 
        // it's beneficial to include it in the parent's subset.
        if (parent_arr[u] != 0) {
            if (dp[u] > 0) {
                dp[parent_arr[u]] += dp[u];
            }
        }
    }
    
    cout << max_profit << "\n";
    return 0;
}
