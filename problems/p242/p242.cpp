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

class Dinic {
  public:
    struct Edge {
        int to, rev;
        int cap;
    };

    int n;
    vector<vector<Edge>> adj;
    vector<int> level;
    vector<int> it;

    Dinic(int _n = 0) {
        n = _n;
        adj.assign(n, {});
        level.assign(n, -1);
        it.assign(n, 0);
    }

    void add_edge(int u, int v, int c) {
        adj[u].push_back({v, (int)adj[v].size(), c});
        adj[v].push_back({u, (int)adj[u].size() - 1, 0});
    }

    bool bfs(int s, int t) {
        level.assign(n, -1);
        queue<int> q;
        q.push(s);
        level[s] = 0;
        while(!q.empty()) {
            int u = q.front();
            q.pop();
            for(auto& e: adj[u]) {
                if(e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }

        return level[t] >= 0;
    }

    int dfs(int u, int t, int f) {
        if(u == t) {
            return f;
        }

        for(; it[u] < (int)adj[u].size(); it[u]++) {
            Edge& e = adj[u][it[u]];
            if(e.cap > 0 && level[e.to] == level[u] + 1) {
                int pushed = dfs(e.to, t, min(f, e.cap));
                if(pushed) {
                    e.cap -= pushed;
                    adj[e.to][e.rev].cap += pushed;
                    return pushed;
                }
            }
        }

        return 0;
    }

    int maxflow(int s, int t) {
        int flow = 0;
        const int INF = 1e9;
        while(bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            while(int pushed = dfs(s, t, INF)) {
                flow += pushed;
            }
        }

        return flow;
    }
};

int n, k;
vector<vector<int>> pref;

void read() {
    cin >> n >> k;
    pref.assign(n, {});
    for(int i = 0; i < n; i++) {
        int cnt;
        cin >> cnt;
        pref[i].resize(cnt);
        cin >> pref[i];
    }
}

void solve() {
    // Model the assignment as a max-flow problem. Each selected university must
    // be visited by at least two students, and since exactly two suffice we make
    // each university demand exactly two:
    //
    // - source -> each university with capacity 2,
    // - university -> student with capacity 1 when the student lists it,
    // - student -> sink with capacity 1 (each student joins at most one company).
    //
    // A valid assignment exists iff the maximum flow equals 2*K (every
    // university filled). We recover each company by scanning a university's
    // outgoing edges: a saturated edge (residual cap 0) to a student means that
    // student was routed through this university.

    int S = 0;
    int uni_offset = 1;
    int stu_offset = uni_offset + k;
    int T = stu_offset + n;
    int V = T + 1;

    Dinic din(V);
    for(int u = 1; u <= k; u++) {
        din.add_edge(S, uni_offset + (u - 1), 2);
    }

    for(int s = 0; s < n; s++) {
        for(int u: pref[s]) {
            if(1 <= u && u <= k) {
                din.add_edge(uni_offset + (u - 1), stu_offset + s, 1);
            }
        }
    }

    for(int s = 0; s < n; s++) {
        din.add_edge(stu_offset + s, T, 1);
    }

    int flow = din.maxflow(S, T);
    if(flow != 2 * k) {
        cout << "NO\n";
        return;
    }

    cout << "YES\n";
    for(int u = 0; u < k; u++) {
        int uni_node = uni_offset + u;
        vector<int> assigned;
        for(auto& e: din.adj[uni_node]) {
            if(stu_offset <= e.to && e.to < stu_offset + n && e.cap == 0) {
                assigned.push_back(e.to - stu_offset + 1);
            }
        }

        cout << assigned.size();
        for(int x: assigned) {
            cout << ' ' << x;
        }

        cout << '\n';
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
