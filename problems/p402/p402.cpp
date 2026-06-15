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

template<class FlowT>
class MaxFlow {
  public:
    const static FlowT finf = 1e9 + 42 + 17;

    struct Edge {
        FlowT flow, cap;
        int idx, rev, to;
        Edge() : flow(0), cap(0), idx(0), rev(0), to(0) {}
        Edge(int _to, int _rev, FlowT _flow, FlowT _cap, int _idx)
            : flow(_flow), cap(_cap), idx(_idx), rev(_rev), to(_to) {}
    };

    int n;
    vector<vector<Edge>> g;
    vector<int> dist, po;
    vector<char> used_cut;

    void init(int _n) {
        n = _n;
        g.assign(n + 1, {});
        dist.assign(n + 1, -1);
        po.assign(n + 1, 0);
        used_cut.assign(n + 1, 0);
    }

    void add_edge(int u, int v, FlowT w, int idx = -1) {
        g[u].push_back(Edge(v, g[v].size(), 0, w, idx));
        g[v].push_back(Edge(u, g[u].size() - 1, 0, 0, -1));
    }

    bool bfs(int s, int t) {
        for(int v = 0; v <= n; v++) {
            dist[v] = -1;
            po[v] = 0;
        }

        queue<int> q;
        q.push(s);
        dist[s] = 0;
        while(!q.empty()) {
            int u = q.front();
            q.pop();
            for(Edge e: g[u]) {
                if(dist[e.to] == -1 && e.flow < e.cap) {
                    dist[e.to] = dist[u] + 1;
                    q.push(e.to);
                }
            }
        }

        return dist[t] != -1;
    }

    FlowT dfs(int u, int t, FlowT fl = finf) {
        if(u == t) {
            return fl;
        }

        for(; po[u] < (int)g[u].size(); po[u]++) {
            auto& e = g[u][po[u]];
            if(dist[e.to] == dist[u] + 1 && e.flow < e.cap) {
                FlowT f = dfs(e.to, t, min(fl, e.cap - e.flow));
                e.flow += f;
                g[e.to][e.rev].flow -= f;
                if(f > 0) {
                    return f;
                }
            }
        }

        return 0;
    }

    FlowT flow(int s, int t) {
        if(s == t) {
            return finf;
        }

        FlowT ret = 0, to_add;
        while(bfs(s, t)) {
            while((to_add = dfs(s, t))) {
                ret += to_add;
            }
        }

        return ret;
    }

    void dfs_min_cut(int u) {
        used_cut[u] = 1;
        for(auto e: g[u]) {
            if(!used_cut[e.to] && e.cap > e.flow) {
                dfs_min_cut(e.to);
            }
        }
    }

    void find_cut(int s) {
        for(int i = 0; i <= n; i++) {
            used_cut[i] = 0;
        }
        dfs_min_cut(s);
    }
};

int n, m;
vector<pair<int, pair<int, int>>> ed;
vector<vector<int>> adj;

void read() {
    cin >> n >> m;
    ed.assign(m + 1, {});
    adj.assign(n + 1, {});
    for(int i = 1; i <= m; i++) {
        cin >> ed[i].first >> ed[i].second.first >> ed[i].second.second;
        adj[ed[i].first].push_back(ed[i].second.first);
        adj[ed[i].second.first].push_back(ed[i].first);
    }
}

int answer;
vector<int> ans;
MaxFlow<int> mf;

void check(int t, int s, int sink) {
    mf.init(n);
    for(int i = 1; i <= m; i++) {
        if(ed[i].first != t && ed[i].second.first != t) {
            mf.add_edge(
                ed[i].first, ed[i].second.first, ed[i].second.second, i
            );
            mf.add_edge(
                ed[i].second.first, ed[i].first, ed[i].second.second, i
            );
        }
    }

    int curr = mf.flow(s, sink);
    if(answer == -1 || curr < answer) {
        mf.find_cut(s);
        answer = curr;
        ans.clear();
        for(int i = 0; i <= n; i++) {
            for(auto it: mf.g[i]) {
                if(mf.used_cut[i] && !mf.used_cut[it.to] && it.idx != -1) {
                    ans.push_back(it.idx);
                }
            }
        }

        sort(ans.begin(), ans.end());
        ans.erase(unique(ans.begin(), ans.end()), ans.end());
    }
}

void solve_captured(int r) {
    for(int u: adj[r]) {
        for(int v: adj[r]) {
            if(u < v && r != u && r != v) {
                check(r, u, v);
            }
        }
    }
}

void solve() {
    // The invaders capture one city t (Berland troops cannot route through it)
    // and then need at least two surviving cities disconnected from each other.
    // Cutting roads to separate a fixed pair s, sink is exactly a min cut in
    // the graph with t removed, where each undirected road becomes a pair of
    // directed edges with capacity equal to its destruction cost.
    //
    // We brute force the captured city t (including a virtual city 0 adjacent
    // to all cities, which covers the case where no city is captured) and the
    // ordered pair s < sink among neighbours of t. Both s and sink must lie on
    // either side of the eventual cut, and any separating cut must split some
    // such neighbour pair, so this enumeration is sufficient. For each choice
    // we run Dinic, take the cheapest cut overall, and recover the cut edges by
    // a reachability DFS from s over residual capacity: edges leaving the
    // reachable set with a valid input index form the plan.

    adj.resize(n + 1);
    for(int i = 1; i <= n; i++) {
        adj[0].push_back(i);
    }

    answer = -1;
    for(int i = 0; i <= n; i++) {
        solve_captured(i);
    }

    cout << answer << '\n';
    cout << ans.size() << '\n';
    cout << ans;
    if(!ans.empty()) {
        cout << '\n';
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    read();
    solve();

    return 0;
}
