#include <bits/stdc++.h>
using namespace std;

struct State {
    map<int, int> go;
    int fail;
    long long base_pen;
    long long total_pen;
    State() : fail(0), base_pen(0), total_pen(0) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    int n, m, r, S, T;
    cin >> n >> m >> r >> S >> T;
    
    vector<tuple<int, int, long long>> edges;
    edges.push_back({0, 0, 0});
    for (int i = 0; i < m; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        edges.push_back({a, b, c});
    }
    
    vector<vector<tuple<int, int, long long>>> graph(n+1);
    for (int i = 1; i <= m; i++) {
        int a = get<0>(edges[i]);
        int b = get<1>(edges[i]);
        long long c = get<2>(edges[i]);
        graph[a].push_back({b, i, c});
    }
    
    vector<vector<int>> patterns;
    vector<long long> pattern_costs;
    
    for (int i = 0; i < r; i++) {
        int k;
        cin >> k;
        vector<int> pat(k);
        for (int j = 0; j < k; j++) {
            cin >> pat[j];
        }
        patterns.push_back(pat);
        long long cost = 0;
        for (int eid : pat) {
            cost += get<2>(edges[eid]);
        }
        pattern_costs.push_back(cost);
    }
    
    vector<State> states;
    states.push_back(State());
    
    for (int i = 0; i < r; i++) {
        int u = 0;
        for (int eid : patterns[i]) {
            if (states[u].go.find(eid) == states[u].go.end()) {
                states.push_back(State());
                int new_state = states.size() - 1;
                states[u].go[eid] = new_state;
                u = new_state;
            } else {
                u = states[u].go[eid];
            }
        }
        states[u].base_pen += pattern_costs[i];
    }
    
    int automaton_size = states.size();
    
    queue<int> q;
    for (auto &p : states[0].go) {
        int v = p.second;
        states[v].fail = 0;
        q.push(v);
    }
    
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (auto &p : states[u].go) {
            int v = p.second;
            int f = states[u].fail;
            while (f != 0 && states[f].go.find(p.first) == states[f].go.end()) {
                f = states[f].fail;
            }
            if (states[f].go.find(p.first) != states[f].go.end()) {
                states[v].fail = states[f].go[p.first];
            } else {
                states[v].fail = 0;
            }
            q.push(v);
        }
    }
    
    vector<int> order;
    queue<int> q2;
    q2.push(0);
    while (!q2.empty()) {
        int u = q2.front(); q2.pop();
        order.push_back(u);
        for (auto &p : states[u].go) {
            q2.push(p.second);
        }
    }
    
    states[0].total_pen = 0;
    for (int i = 1; i < (int)order.size(); i++) {
        int u = order[i];
        states[u].total_pen = states[u].base_pen + states[states[u].fail].total_pen;
    }
    
    vector<unordered_map<int, int>> trans_cache(automaton_size);
    
    int total_states = n * automaton_size;
    vector<long long> dist(total_states, LLONG_MAX);
    vector<int> prev_node(total_states, -1);
    vector<int> prev_edge(total_states, -1);
    vector<int> prev_state_id(total_states, -1);
    
    priority_queue<tuple<long long, int, int>, 
                   vector<tuple<long long, int, int>>, 
                   greater<tuple<long long, int, int>>> pq;
    
    int start_index = S * automaton_size + 0;
    dist[start_index] = 0;
    pq.push({0, S, 0});
    
    int final_node = -1;
    int final_state = -1;
    
    while (!pq.empty()) {
        auto [d, u, state_id] = pq.top();
        pq.pop();
        int idx = u * automaton_size + state_id;
        if (d != dist[idx]) 
            continue;
        if (u == T) {
            final_node = u;
            final_state = state_id;
            break;
        }
        
        for (auto &edge_info : graph[u]) {
            int v = get<0>(edge_info);
            int eid = get<1>(edge_info);
            long long w = get<2>(edge_info);
            
            int cur = state_id;
            auto it = trans_cache[cur].find(eid);
            int next_state_id;
            if (it != trans_cache[cur].end()) {
                next_state_id = it->second;
            } else {
                int temp = cur;
                while (temp != 0 && states[temp].go.find(eid) == states[temp].go.end()) {
                    temp = states[temp].fail;
                }
                if (states[temp].go.find(eid) != states[temp].go.end()) {
                    next_state_id = states[temp].go[eid];
                } else {
                    next_state_id = 0;
                }
                trans_cache[cur][eid] = next_state_id;
            }
            
            long long add = states[next_state_id].total_pen;
            long long new_d = d + w + add;
            
            int next_idx = v * automaton_size + next_state_id;
            if (new_d < dist[next_idx]) {
                dist[next_idx] = new_d;
                prev_node[next_idx] = u;
                prev_edge[next_idx] = eid;
                prev_state_id[next_idx] = state_id;
                pq.push({new_d, v, next_state_id});
            }
        }
    }
    
    if (final_node == -1) {
        cout << -1 << endl;
    } else {
        vector<int> path_edges;
        int cur_node = T;
        int cur_state = final_state;
        while (cur_node != S) {
            int idx = cur_node * automaton_size + cur_state;
            int e = prev_edge[idx];
            path_edges.push_back(e);
            int next_node = prev_node[idx];
            int next_state = prev_state_id[idx];
            cur_node = next_node;
            cur_state = next_state;
        }
        reverse(path_edges.begin(), path_edges.end());
        cout << dist[T * automaton_size + final_state] << endl;
        cout << path_edges.size() << endl;
        for (int i = 0; i < (int)path_edges.size(); i++) {
            if (i > 0) cout << " ";
            cout << path_edges[i];
        }
        cout << endl;
    }
    
    return 0;
}
