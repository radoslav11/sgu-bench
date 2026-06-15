#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAX_NODES = 300005;
const int MAX_N = 100005;

vector<int> children[MAX_NODES];
int in_time[MAX_NODES];
int out_time[MAX_NODES];
bool is_child[MAX_NODES];

int timer_dfs = 0;

void dfs(int u) {
    if (children[u].empty()) {
        in_time[u] = ++timer_dfs;
        out_time[u] = timer_dfs;
        return;
    }
    in_time[u] = 1e9;
    out_time[u] = -1e9;
    for (int v : children[u]) {
        dfs(v);
        in_time[u] = min(in_time[u], in_time[v]);
        out_time[u] = max(out_time[u], out_time[v]);
    }
}

struct QueryEvent {
    int y, type, qid;
};

vector<QueryEvent> events[MAX_N];
vector<int> points_at_x[MAX_N];

int bit[MAX_N];

void add(int idx, int val) {
    for (; idx < MAX_N; idx += idx & -idx) {
        bit[idx] += val;
    }
}

int query(int idx) {
    int sum = 0;
    for (; idx > 0; idx -= idx & -idx) {
        sum += bit[idx];
    }
    return sum;
}

struct QueryInfo {
    int type, a, b;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    vector<pair<int, int>> edges(M);
    for (int i = 0; i < M; ++i) {
        cin >> edges[i].first >> edges[i].second;
    }

    sort(edges.begin(), edges.end());
    edges.erase(unique(edges.begin(), edges.end()), edges.end());

    int Q;
    cin >> Q;

    int next_id = N;
    vector<QueryInfo> query_list(Q);

    for (int i = 0; i < Q; ++i) {
        cin >> query_list[i].type >> query_list[i].a >> query_list[i].b;
        if (query_list[i].type == 2) {
            next_id++;
            children[next_id].push_back(query_list[i].a);
            children[next_id].push_back(query_list[i].b);
            is_child[query_list[i].a] = true;
            is_child[query_list[i].b] = true;
        }
    }

    for (int i = 1; i <= next_id; ++i) {
        if (!is_child[i]) {
            dfs(i);
        }
    }

    vector<int> ans;
    for (int i = 0; i < Q; ++i) {
        if (query_list[i].type == 1) {
            ans.push_back(0);
            int id = ans.size() - 1;
            int a = query_list[i].a;
            int b = query_list[i].b;
            
            events[out_time[a]].push_back({out_time[b], 1, id});
            if (in_time[a] > 1) {
                events[in_time[a] - 1].push_back({out_time[b], -1, id});
            }
            if (in_time[b] > 1) {
                events[out_time[a]].push_back({in_time[b] - 1, -1, id});
            }
            if (in_time[a] > 1 && in_time[b] > 1) {
                events[in_time[a] - 1].push_back({in_time[b] - 1, 1, id});
            }
        }
    }

    for (const auto& edge : edges) {
        int u = edge.first;
        int v = edge.second;
        points_at_x[in_time[u]].push_back(in_time[v]);
    }

    for (int x = 1; x <= N; ++x) {
        for (int y : points_at_x[x]) {
            add(y, 1);
        }
        for (const auto& ev : events[x]) {
            ans[ev.qid] += ev.type * query(ev.y);
        }
    }

    for (int a : ans) {
        if (a > 0) {
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }

    return 0;
}
