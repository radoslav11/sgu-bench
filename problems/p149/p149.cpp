#include <bits/stdc++.h>

using namespace std;

template<typename T1, typename T2>
ostream& operator<<(ostream &out, const pair<T1, T2>& x) {
    return out << x.first << ' ' << x.second;
}

template<typename T1, typename T2>
istream& operator>>(istream &in, pair<T1, T2>& x) {
    return in >> x.first >> x.second;
}

template<typename T>
istream& operator>>(istream &in, vector<T>& a) {
    for(auto& x: a) {
        in >> x;
    }
    return in;
};

template<typename T>
ostream& operator<<(ostream &out, const vector<T>& a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
};

int n;
vector<vector<pair<int, int>>> adj;

void read() {
    cin >> n;
    adj.assign(n, {});
    for(int i = 1; i < n; i++) {
        int u, w;
        cin >> u >> w;
        u--;
        adj[i].push_back({u, w});
        adj[u].push_back({i, w});
    }
}

vector<int> get_distances(int src) {
    vector<int> dist(n, -1);

    function<void(int, int)> dfs = [&](int u, int p) {
        for(auto [v, w]: adj[u]) {
            if(v == p) {
                continue;
            }
            dist[v] = dist[u] + w;
            dfs(v, u);
        }
    };

    dist[src] = 0;
    dfs(src, -1);
    return dist;
}

void solve() {
    // For each node we need the eccentricity: the distance to its farthest
    // node in the tree. The farthest node from any vertex is always one of the
    // two endpoints of the tree's diameter. We find the diameter with the
    // classic two passes: a DFS from node 0 gives the first endpoint (the
    // farthest from 0), a DFS from there gives the second endpoint, and a DFS
    // from that second endpoint gives the third distance array. For every node
    // the answer is the larger of its distances to the two diameter endpoints.

    vector<int> dist0 = get_distances(0);
    int diam_end_1 = max_element(dist0.begin(), dist0.end()) - dist0.begin();
    vector<int> dist1 = get_distances(diam_end_1);
    int diam_end_2 = max_element(dist1.begin(), dist1.end()) - dist1.begin();
    vector<int> dist2 = get_distances(diam_end_2);

    for(int i = 0; i < n; i++) {
        cout << max(dist1[i], dist2[i]) << '\n';
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
