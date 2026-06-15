#include <bits/stdc++.h>

using namespace std;

template<typename T1, typename T2>
ostream& operator<<(ostream& out, const pair<T1, T2>& x) {
    return out << x.first << ' ' << x.second;
}

template<typename T1, typename T2>
istream& operator>>(istream& in, pair<T1, T2>& x) {
    return in >> x.first >> x.second;
}

template<typename T>
istream& operator>>(istream& in, vector<T>& a) {
    for(auto& x: a) {
        in >> x;
    }
    return in;
};

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
};

int n, k;
vector<vector<int>> adj;
vector<int> dp;

void read() {
    cin >> n >> k;
    adj.assign(n + 1, {});
    dp.assign(n + 1, 0);
    for(int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
}

int dfs(int u, int par, vector<int>& ans) {
    vector<int> child_dps;

    for(int v: adj[u]) {
        if(v == par) {
            continue;
        }
        dfs(v, u, ans);
        child_dps.push_back(dp[v]);
    }

    if(!child_dps.empty()) {
        int min_dist = *min_element(child_dps.begin(), child_dps.end());
        int max_dist = *max_element(child_dps.begin(), child_dps.end());

        if(min_dist + max_dist + 2 <= 2 * k + 1) {
            dp[u] = min_dist + 1;
        } else {
            dp[u] = max_dist + 1;
        }
    } else {
        dp[u] = k + 1;
    }

    if(dp[u] == 2 * k + 1) {
        dp[u] = 0;
        ans.push_back(u);
    }

    return dp[u];
}

void solve() {
    // Pick the minimum set of centers so every vertex of the tree is within
    // distance k of some center. Root anywhere (vertex 1): the answer is
    // invariant to the root because rooting at any vertex between two optimal
    // centers gives the same greedy outcome.
    //
    // - Run a post-order DFS where dp[u] is the distance from u up to the
    //   closest center in u's processed subtree. A leaf has no covered subtree,
    //   so it behaves as if a center sits k+1 below it (dp = k+1).
    //
    // - At an internal node take the closest child center distance min_dist and
    //   the farthest still-uncovered one max_dist. If min_dist + max_dist + 2
    //   fits within 2k+1, the closest center also covers the farthest branch,
    //   so dp[u] = min_dist + 1; otherwise the farthest branch is the critical
    //   path and dp[u] = max_dist + 1.
    //
    // - When dp[u] reaches 2k+1 a vertex on the critical path would go
    //   uncovered, so we must place a center at u, resetting dp[u] to 0. After
    //   the DFS, if dp[root] still exceeds k the root subtree is uncovered, so
    //   add the root as a final center.

    vector<int> ans;
    dfs(1, -1, ans);

    if(dp[1] > k) {
        ans.push_back(1);
    }

    sort(ans.begin(), ans.end());

    cout << ans.size() << "\n";
    for(int center: ans) {
        cout << center << "\n";
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
