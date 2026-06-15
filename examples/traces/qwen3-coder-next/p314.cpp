#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    int n, m, k;
    cin >> n >> m >> k;
    int s, t;
    cin >> s >> t;
    
    vector<vector<pair<int, int>>> graph(n + 1);
    for (int i = 0; i < m; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        graph[a].push_back({b, c});
    }
    
    vector<int> cnt(n + 1, 0);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
    pq.push({0, s});
    vector<ll> ans;
    
    while (!pq.empty() && (int)ans.size() < k) {
        auto [d, u] = pq.top();
        pq.pop();
        
        if (cnt[u] == k) {
            continue;
        }
        cnt[u]++;
        
        if (u == t) {
            ans.push_back(d);
        }
        
        for (auto [v, w] : graph[u]) {
            if (cnt[v] < k) {
                pq.push({d + w, v});
            }
        }
    }
    
    for (int i = 0; i < k; i++) {
        if (i < (int)ans.size()) {
            cout << ans[i] << '\n';
        } else {
            cout << "NO\n";
        }
    }
    
    return 0;
}
