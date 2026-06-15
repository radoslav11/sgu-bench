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

struct Edge {
    int u, v, c, t;
};

int n, m;
vector<Edge> edges;
vector<double> dist;
vector<int> par;

void read() {
    cin >> n >> m;
    edges.resize(m);
    for(int i = 0; i < m; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].c >> edges[i].t;
    }
}

pair<bool, vector<int>> check(double x) {
    dist.assign(n + 2, 1e18);
    par.assign(n + 2, -1);

    int source = n + 1;
    dist[source] = 0;

    for(int i = 0; i < n; i++) {
        for(auto& e: edges) {
            double weight = e.t * x - e.c;
            if(dist[e.u] < 1e17 && dist[e.u] + weight < dist[e.v]) {
                dist[e.v] = dist[e.u] + weight;
                par[e.v] = e.u;
            }
        }

        for(int v = 1; v <= n; v++) {
            if(dist[source] < dist[v]) {
                dist[v] = dist[source];
                par[v] = source;
            }
        }
    }

    int cycle_node = -1;
    for(auto& e: edges) {
        double weight = e.t * x - e.c;
        if(dist[e.u] < 1e17 && dist[e.u] + weight < dist[e.v]) {
            cycle_node = e.v;
            break;
        }
    }

    if(cycle_node == -1) {
        return {false, {}};
    }

    for(int i = 0; i < n; i++) {
        cycle_node = par[cycle_node];
    }

    vector<int> cycle;
    int curr = cycle_node;
    do {
        cycle.push_back(curr);
        curr = par[curr];
    } while(curr != cycle_node);

    reverse(cycle.begin(), cycle.end());
    return {true, cycle};
}

void solve() {
    // We maximize the cycle ratio (sum of costs) / (sum of times). Binary search
    // the answer x: a cycle with ratio > x exists iff some cycle has positive
    // sum of (c - t*x), equivalently a negative cycle under edge weight
    // t*x - c. We detect such a cycle with N relaxation rounds of Bellman-Ford
    // from a virtual super-source connected to every node, then one extra round
    // to find an edge that still relaxes; walking N parent steps lands inside the
    // negative cycle, which we trace via the parent array and report.

    double l = 0.0, r = 200.0;
    for(int i = 0; i < 100; i++) {
        double mid = (l + r) / 2;
        auto [has_cycle, cycle] = check(mid);
        if(has_cycle) {
            l = mid;
        } else {
            r = mid;
        }
    }

    auto [has_cycle, cycle] = check(l);
    if(!has_cycle) {
        cout << "0\n";
        return;
    }

    cout << cycle.size() << "\n";
    cout << cycle << "\n";
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
