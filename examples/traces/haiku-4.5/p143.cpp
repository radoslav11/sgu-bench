#include <bits/stdc++.h>
using namespace std;

int N;
vector<int> profit;
vector<vector<int>> adj;
int maxProfit = INT_MIN;

int dfs(int node, int parent) {
    int result = profit[node];
    
    for (int child : adj[node]) {
        if (child == parent) continue;
        int childProfit = dfs(child, node);
        if (childProfit > 0) {
            result += childProfit;
        }
    }
    
    maxProfit = max(maxProfit, result);
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N;
    profit.resize(N + 1);
    adj.resize(N + 1);
    
    for (int i = 1; i <= N; i++) {
        cin >> profit[i];
    }
    
    for (int i = 0; i < N - 1; i++) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    
    dfs(1, -1);
    
    cout << maxProfit << endl;
    
    return 0;
}
