#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

typedef long long ll;
const ll INF = 1e15;

struct Edge {
    int to; ll cap, flow; int rev, id;
};

vector<vector<Edge>> adj;
vector<int> level, ptr;

void add_edge(int from, int to, ll cap, int id = -1) {
    adj[from].push_back({to, cap, 0, (int)adj[to].size(), id});
    adj[to].push_back({from, 0, 0, (int)adj[from].size() - 1, -1});
}

bool bfs(int s, int t) {
    fill(level.begin(), level.end(), -1);
    level[s] = 0; queue<int> q; q.push(s);
    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (auto& e : adj[v]) if (e.cap - e.flow > 0 && level[e.to] == -1) { level[e.to] = level[v] + 1; q.push(e.to); }
    }
    return level[t] != -1;
}

ll dfs(int v, int t, ll pushed) {
    if (pushed == 0 || v == t) return pushed;
    for (int& cid = ptr[v]; cid < (int)adj[v].size(); ++cid) {
        auto& e = adj[v][cid];
        if (level[v] + 1 != level[e.to] || e.cap - e.flow == 0) continue;
        ll tr = dfs(e.to, t, min(pushed, e.cap - e.flow));
        if (tr == 0) continue;
        e.flow += tr; adj[e.to][e.rev].flow -= tr; return tr;
    }
    return 0;
}

ll dinic(int s, int t) {
    ll flow = 0;
    while (bfs(s, t)) { fill(ptr.begin(), ptr.end(), 0); while (ll p = dfs(s, t, INF)) flow += p; }
    return flow;
}

void push_exact(int s, int t, ll k) {
    while (k > 0) {
        if (!bfs(s, t)) break;
        fill(ptr.begin(), ptr.end(), 0);
        while (ll p = dfs(s, t, k)) k -= p;
    }
}

int main() {
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    int N, M; if (!(cin >> N >> M)) return 0;
    struct In { int u, v, z, c; };
    vector<In> input(M);
    for (int i = 0; i < M; ++i) cin >> input[i].u >> input[i].v >> input[i].z >> input[i].c;
    adj.assign(N + 3, vector<Edge>()); level.resize(N + 3); ptr.resize(N + 3);
    vector<ll> L(M), R(M), D(N + 1, 0);
    for (int i = 0; i < M; ++i) {
        L[i] = (input[i].c == 1 ? input[i].z : 0); R[i] = input[i].z;
        add_edge(input[i].u, input[i].v, R[i] - L[i], i);
        D[input[i].u] -= L[i]; D[input[i].v] += L[i];
    }
    add_edge(N, 1, INF);
    int Sp = 0, Tp = N + 1; ll total_pos = 0;
    for (int i = 1; i <= N; ++i) {
        if (D[i] > 0) { add_edge(Sp, i, D[i]); total_pos += D[i]; }
        else if (D[i] < 0) { add_edge(i, Tp, -D[i]); }
    }
    if (dinic(Sp, Tp) != total_pos) { cout << "Impossible" << endl; return 0; }
    vector<vector<Edge>> circ = adj;
    for (auto& e : circ[N]) if (e.to == 1 && e.id == -1 && e.cap == INF) { e.cap = 0; e.flow = 0; circ[e.to][e.rev].cap = 0; circ[e.to][e.rev].flow = 0; }
    ll flow_N_to_1 = 0;
    for (auto& e : circ[N]) if (e.to == 1 && e.id == -1 && e.cap == INF) flow_N_to_1 = e.flow;
    ll S_net = -flow_N_to_1;
    adj = circ;
    for (auto& e : adj[N]) if (e.to == 1 && e.id == -1 && e.cap == INF) { e.cap = 0; e.flow = 0; adj[e.to][e.rev].cap = 0; adj[e.to][e.rev].flow = 0; }
    ll F_1_N = dinic(1, N), F_N_1 = dinic(N, 1);
    ll S_min = S_net - F_N_1, S_max = S_net + F_1_N;
    if (S_max < 0) { cout << "Impossible" << endl; return 0; }
    ll S_ans = max(0LL, S_min);
    adj = circ;
    for (auto& e : adj[N]) if (e.to == 1 && e.id == -1 && e.cap == INF) { e.cap = 0; e.flow = 0; adj[e.to][e.rev].cap = 0; adj[e.to][e.rev].flow = 0; }
    if (S_ans == 0) {
        if (S_net > 0) push_exact(N, 1, S_net);
        else if (S_net < 0) push_exact(1, N, -S_net);
    } else push_exact(N, 1, S_net - S_ans);
    cout << S_ans << endl;
    vector<ll> final_f(M);
    for (int i = 1; i <= N; ++i) for (auto& e : adj[i]) if (e.id != -1) final_f[e.id] = L[e.id] + e.flow;
    for (int i = 0; i < M; ++i) cout << final_f[i] << (i == M - 1 ? "" : " ");
    cout << endl;
    return 0;
}

