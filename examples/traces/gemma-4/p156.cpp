#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>
#include <stack>

using namespace std;

const int MAXN = 10005;
vector<int> adj[MAXN];
int deg[MAXN];
bitset<MAXN> neighbors[MAXN];
int special_neighbor[MAXN];
bool is_v2[MAXN];
int block_id[MAXN], block_size[MAXN], num_blocks = 0;
bool visited[MAXN];

struct Edge {
    int to, id, u_to_v;
};
vector<Edge> s_adj[MAXN];
bool used_edge[MAXN * 10];
int edge_to_u[MAXN * 10], edge_to_v[MAXN * 10];
vector<int> path_of_edge[MAXN * 10];
vector<pair<int, int>> circuit;

void hierholzer(int u) {
    stack<int> st;
    st.push(u);
    while (!st.empty()) {
        int v = st.top();
        if (!s_adj[v].empty()) {
            Edge e = s_adj[v].back();
            s_adj[v].pop_back();
            if (used_edge[e.id]) continue;
            used_edge[e.id] = true;
            st.push(e.to);
            // Store edge and direction to reconstruct later
            // Instead of this, let's use the recursive version which is simpler.
        } else {
            st.pop();
        }
    }
}

// Using recursive for simplicity
void hierholzer_rec(int u) {
    while (!s_adj[u].empty()) {
        Edge e = s_adj[u].back();
        s_adj[u].pop_back();
        if (used_edge[e.id]) continue;
        used_edge[e.id] = true;
        hierholzer_rec(e.to);
        circuit.push_back({e.id, e.u_to_v});
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    if (!(cin >> N >> M)) return 0;
    for (int i = 0; i < M; ++i) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v); adj[v].push_back(u);
        neighbors[u].set(v); neighbors[v].set(u);
        deg[u]++; deg[v]++;
    }

    bool all_deg2 = true;
    for (int i = 1; i <= N; ++i) if (deg[i] != 2) { all_deg2 = false; break; }
    if (all_deg2) {
        vector<int> cycle;
        int curr = 1, prev = 0;
        for (int i = 0; i < N; ++i) {
            cycle.push_back(curr);
            int next_node = (adj[curr][0] == prev) ? adj[curr][1] : adj[curr][0];
            prev = curr; curr = next_node;
        }
        for (int i = 0; i < N; ++i) cout << cycle[i] << (i == N - 1 ? "" : " ");
        cout << endl; return 0;
    }

    for (int i = 1; i <= N; ++i) if (deg[i] == 2) is_v2[i] = true;
    for (int i = 1; i <= N; ++i) {
        if (deg[i] > 2) {
            int found_u = -1;
            for (int u : adj[i]) {
                if (is_v2[u]) {
                    bool clique = true;
                    vector<int> others;
                    for (int w : adj[i]) if (w != u) others.push_back(w);
                    for (int j = 0; j < (int)others.size(); ++j) {
                        for (int k = j + 1; k < (int)others.size(); ++k) {
                            if (!neighbors[others[j]].test(others[k])) { clique = false; break; }
                        }
                        if (!clique) break;
                    }
                    if (clique) { found_u = u; break; }
                }
            }
            if (found_u == -1) { cout << -1 << endl; return 0; }
            special_neighbor[i] = found_u;
        }
    }

    for (int i = 1; i <= N; ++i) {
        if (deg[i] > 2 && !visited[i]) {
            num_blocks++;
            vector<int> q = {i}; visited[i] = true;
            int head = 0;
            while (head < (int)q.size()) {
                int u = q[head++];
                block_id[u] = num_blocks; block_size[num_blocks]++;
                for (int v : adj[u]) if (deg[v] > 2 && !visited[v]) { visited[v] = true; q.push_back(v); }
            }
        }
    }

    int edge_count = 0;
    for (int i = 1; i <= N; ++i) {
        if (deg[i] > 2) {
            int u = special_neighbor[i];
            int curr = u, prev = i;
            vector<int> path_v2;
            while (is_v2[curr]) {
                path_v2.push_back(curr);
                int next_node = (adj[curr][0] == prev) ? adj[curr][1] : adj[curr][0];
                prev = curr; curr = next_node;
            }
            edge_count++;
            edge_to_u[edge_count] = i; edge_to_v[edge_count] = curr;
            path_of_edge[edge_count] = path_v2;
        }
    }

    for (int i = 1; i <= edge_count; ++i) {
        int b1 = block_id[edge_to_u[i]], b2 = block_id[edge_to_v[i]];
        s_adj[b1].push_back({b2, i, 1});
        s_adj[b2].push_back({b1, i, 0});
    }

    int start_block = 0;
    for (int i = 1; i <= num_blocks; ++i) {
        if (!s_adj[i].empty()) { start_block = i; break; }
    }
    if (start_block == 0) { cout << -1 << endl; return 0; }

    hierholzer_rec(start_block);
    if (circuit.size() != (size_t)edge_count) { cout << -1 << endl; return 0; }
    reverse(circuit.begin(), circuit.end());

    vector<int> res;
    for (auto& p : circuit) {
        int eid = p.first, dir = p.second;
        int u = (dir == 1) ? edge_to_u[eid] : edge_to_v[eid];
        int v = (dir == 1) ? edge_to_v[eid] : edge_to_u[eid];
        if (res.empty() || res.back() != u) res.push_back(u);
        for (int pv : path_of_edge[eid]) res.push_back(pv);
        if (res.back() != v) res.push_back(v);
    }

    vector<int> clean_res;
    for (int v : res) if (clean_res.empty() || v != clean_res.back()) clean_res.push_back(v);
    if (clean_res.size() > 1 && clean_res.front() == clean_res.back()) clean_res.pop_back();

    vector<int> block_members[MAXN];
    for (int i = 1; i <= N; ++i) if (deg[i] > 2) block_members[block_id[i]].push_back(i);

    vector<int> block_first_idx(num_blocks + 1, -1);
    for (int i = 0; i < (int)clean_res.size(); ++i) {
        int b = block_id[clean_res[i]];
        if (b > 0 && block_first_idx[b] == -1) block_first_idx[b] = i;
    }

    vector<int> final_ham;
    vector<bool> ham_visited(N + 1, false);
    for (int i = 0; i < (int)clean_res.size(); ++i) {
        int v = clean_res[i];
        final_ham.push_back(v);
        ham_visited[v] = true;
        int b = block_id[v];
        if (b > 0 && block_first_idx[b] == i) {
            for (int mv : block_members[b]) {
                if (!ham_visited[mv]) {
                    final_ham.push_back(mv);
                    ham_visited[mv] = true;
                }
            }
        }
    }

    for (int i = 0; i < (int)final_ham.size(); ++i) cout << final_ham[i] << (i == (int)final_ham.size() - 1 ? "" : " ");
    cout << endl;

    return 0;
}

