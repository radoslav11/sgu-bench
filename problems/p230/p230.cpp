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

int n, m;
vector<vector<int>> adj;

void read() {
    cin >> n >> m;
    adj.assign(n, vector<int>());
    for(int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        adj[u].push_back(v);
    }
}

vector<int> top_sort() {
    vector<int> order;
    vector<int> deg(n);
    for(int u = 0; u < n; u++) {
        for(int v: adj[u]) {
            deg[v]++;
        }
    }

    queue<int> q;
    for(int u = 0; u < n; u++) {
        if(deg[u] == 0) {
            q.push(u);
        }
    }

    while(!q.empty()) {
        int u = q.front();
        q.pop();
        order.push_back(u);
        for(int v: adj[u]) {
            deg[v]--;
            if(deg[v] == 0) {
                q.push(v);
            }
        }
    }

    return order;
}

void solve() {
    // Each weighing "box P lighter than box Q" is a directed edge P -> Q in a
    // DAG whose vertices are the boxes. A valid assignment of coin values is a
    // linear extension of this partial order: run Kahn's topological sort, and
    // the position of a box in the resulting order (1-indexed) is the coin value
    // placed in it. If the topological order does not cover all boxes there is a
    // cycle and no assignment exists.

    vector<int> order = top_sort();
    if((int)order.size() != n) {
        cout << "No solution\n";
        return;
    }

    vector<int> pos(n);
    for(int i = 0; i < n; i++) {
        pos[order[i]] = i;
    }

    for(int i = 0; i < n; i++) {
        cout << pos[i] + 1 << ' ';
    }

    cout << '\n';
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
