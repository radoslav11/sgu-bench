#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Query {
    int id;
    int w;
};

struct Edge {
    int u, v, w, id;
    bool operator<(const Edge& other) const {
        if (w != other.w) return w < other.w;
        return id < other.id;
    }
};

int parent_dsu[40005];

inline void init_dsu(int n) {
    for (int i = 1; i <= n; ++i) {
        parent_dsu[i] = i;
    }
}

inline int find_set(int i) {
    int root = i;
    while (root != parent_dsu[root]) root = parent_dsu[root];
    int curr = i;
    while (curr != root) {
        int nxt = parent_dsu[curr];
        parent_dsu[curr] = root;
        curr = nxt;
    }
    return root;
}

inline bool unite(int i, int j) {
    int root_i = find_set(i);
    int root_j = find_set(j);
    if (root_i != root_j) {
        parent_dsu[root_i] = root_j;
        return true;
    }
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    if (!(cin >> n >> m)) return 0;
    
    vector<int> orig_u(m + 1), orig_v(m + 1), current_weight(m + 1);
    for (int i = 1; i <= m; ++i) {
        cin >> orig_u[i] >> orig_v[i] >> current_weight[i];
    }
    
    int t;
    cin >> t;
    vector<Query> queries(t + 1);
    for (int i = 1; i <= t; ++i) {
        cin >> queries[i].id >> queries[i].w;
    }
    
    int B = max(1, min(t, 300));
    
    vector<long long> ans(t + 1, 0);
    vector<bool> is_dynamic(m + 1, false);
    
    vector<Edge> E_static;
    E_static.reserve(m);
    vector<Edge> T_static;
    T_static.reserve(n);
    vector<int> dyn_ids;
    dyn_ids.reserve(B);
    
    vector<Edge> cur_dyn;
    cur_dyn.reserve(B);
    
    vector<Edge> merged_edges;
    merged_edges.reserve(n + B);
    
    for (int L = 1; L <= t; L += B) {
        int R = min(t, L + B - 1);
        
        fill(is_dynamic.begin(), is_dynamic.end(), false);
        dyn_ids.clear();
        for (int i = L; i <= R; ++i) {
            int eid = queries[i].id;
            if (!is_dynamic[eid]) {
                is_dynamic[eid] = true;
                dyn_ids.push_back(eid);
            }
        }
        
        E_static.clear();
        for (int i = 1; i <= m; ++i) {
            if (!is_dynamic[i]) {
                E_static.push_back({orig_u[i], orig_v[i], current_weight[i], i});
            }
        }
        
        sort(E_static.begin(), E_static.end());
        
        init_dsu(n);
        T_static.clear();
        int cnt = 0;
        for (const auto& edge : E_static) {
            if (unite(edge.u, edge.v)) {
                T_static.push_back(edge);
                cnt++;
                if (cnt == n - 1) break;
            }
        }
        
        for (int i = L; i <= R; ++i) {
            int q_id = queries[i].id;
            int q_w = queries[i].w;
            
            cur_dyn.clear();
            for (int id : dyn_ids) {
                int w = (id == q_id) ? q_w : current_weight[id];
                cur_dyn.push_back({orig_u[id], orig_v[id], w, id});
            }
            
            sort(cur_dyn.begin(), cur_dyn.end());
            
            merged_edges.clear();
            merge(T_static.begin(), T_static.end(), cur_dyn.begin(), cur_dyn.end(), back_inserter(merged_edges));
            
            init_dsu(n);
            long long current_ans = 0;
            int edges_used = 0;
            for (const auto& edge : merged_edges) {
                if (unite(edge.u, edge.v)) {
                    current_ans += edge.w;
                    edges_used++;
                    if (edges_used == n - 1) break;
                }
            }
            ans[i] = current_ans;
        }
        
        for (int i = L; i <= R; ++i) {
            current_weight[queries[i].id] = queries[i].w;
        }
    }
    
    for (int i = 1; i <= t; ++i) {
        cout << ans[i] << "\n";
    }
    
    return 0;
}
