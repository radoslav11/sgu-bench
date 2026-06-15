#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m, k;
    cin >> n >> m >> k;
    int s, t;
    cin >> s >> t;
    
    vector<vector<pair<int,int>>> adj(n+1);
    for (int i = 0; i < m; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        adj[a].push_back({b, c});
    }
    
    vector<int> cnt(n+1, 0);
    priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<>> pq;
    pq.push({0LL, s});
    
    vector<long long> results;
    
    while (!pq.empty() && (int)results.size() < k) {
        auto [d, u] = pq.top(); pq.pop();
        cnt[u]++;
        if (cnt[u] > k) continue;
        if (u == t) results.push_back(d);
        for (auto [v, w] : adj[u]) {
            if (cnt[v] < k) {
                pq.push({d + w, v});
            }
        }
    }
    
    for (int i = 0; i < k; i++) {
        if (i < (int)results.size()) cout << results[i] << "\n";
        else cout << "NO\n";
    }
    
    return 0;
}
