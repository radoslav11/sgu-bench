#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int INF = 2147483647;

int n, m;
vector<int> children[100005];
int val[100005];
bool is_leaf[100005];

int sz[100005];
int heavy[100005];

void dfs_sz(int u) {
    sz[u] = 1;
    heavy[u] = 0;
    int max_sub = 0;
    for (int v : children[u]) {
        dfs_sz(v);
        sz[u] += sz[v];
        if (sz[v] > max_sub) {
            max_sub = sz[v];
            heavy[u] = v;
        }
    }
}

int bit_arr[100005];
int M_val;
int current_size = 0;
vector<int> added_elements;
long long global_min_diff = INF;
vector<int> orig_val;

void add_bit(int idx, int delta) {
    for (; idx <= M_val; idx += idx & -idx) {
        bit_arr[idx] += delta;
    }
}

int get_count(int idx) {
    int sum = 0;
    for (; idx > 0; idx -= idx & -idx) {
        sum += bit_arr[idx];
    }
    return sum;
}

int find_kth(int k) {
    int idx = 0;
    for (int i = 17; i >= 0; --i) {
        int next_idx = idx + (1 << i);
        if (next_idx <= M_val && bit_arr[next_idx] < k) {
            idx = next_idx;
            k -= bit_arr[idx];
        }
    }
    return idx + 1;
}

int get_pred(int x) {
    int count = get_count(x - 1);
    if (count == 0) return -1;
    return find_kth(count);
}

int get_succ(int x) {
    int count = get_count(x);
    if (count == current_size) return -1;
    return find_kth(count + 1);
}

void add_val(int x) {
    if (global_min_diff == 0) {
        current_size++;
        add_bit(x, 1);
        added_elements.push_back(x);
        return;
    }
    
    if (get_count(x) - get_count(x - 1) > 0) {
        global_min_diff = 0;
    }
    
    current_size++;
    add_bit(x, 1);
    added_elements.push_back(x);
    
    int pred = get_pred(x);
    if (pred != -1) {
        global_min_diff = min(global_min_diff, (long long)orig_val[x] - orig_val[pred]);
    }
    int succ = get_succ(x);
    if (succ != -1) {
        global_min_diff = min(global_min_diff, (long long)orig_val[succ] - orig_val[x]);
    }
}

void clear_all() {
    for (int x : added_elements) {
        add_bit(x, -1);
    }
    added_elements.clear();
    current_size = 0;
    global_min_diff = INF;
}

long long ans[100005];

void add_subtree(int u) {
    if (is_leaf[u]) {
        add_val(val[u]);
        return;
    }
    for (int v : children[u]) {
        add_subtree(v);
    }
}

void dfs(int u, bool keep) {
    for (int v : children[u]) {
        if (v != heavy[u]) {
            dfs(v, false);
        }
    }
    if (heavy[u]) {
        dfs(heavy[u], true);
    }
    for (int v : children[u]) {
        if (v != heavy[u]) {
            add_subtree(v);
        }
    }
    if (is_leaf[u]) {
        add_val(val[u]);
    }
    ans[u] = global_min_diff;
    
    if (!keep) {
        clear_all();
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    if (!(cin >> n >> m)) return 0;
    
    for (int i = 2; i <= n; ++i) {
        int p;
        cin >> p;
        children[p].push_back(i);
    }
    
    vector<int> all_vals;
    for (int i = n - m + 1; i <= n; ++i) {
        cin >> val[i];
        all_vals.push_back(val[i]);
        is_leaf[i] = true;
    }
    for (int i = 1; i <= n - m; ++i) {
        is_leaf[i] = false;
    }
    
    sort(all_vals.begin(), all_vals.end());
    all_vals.erase(unique(all_vals.begin(), all_vals.end()), all_vals.end());
    M_val = all_vals.size();
    orig_val.assign(M_val + 1, 0);
    for (int i = 0; i < M_val; ++i) {
        orig_val[i + 1] = all_vals[i];
    }
    for (int i = n - m + 1; i <= n; ++i) {
        val[i] = lower_bound(all_vals.begin(), all_vals.end(), val[i]) - all_vals.begin() + 1;
    }
    
    dfs_sz(1);
    
    for (int i = 1; i <= n; ++i) {
        ans[i] = INF;
    }
    
    dfs(1, true);
    
    for (int i = 1; i <= n - m; ++i) {
        cout << ans[i] << (i == n - m ? "" : " ");
    }
    cout << "\n";
    
    return 0;
}
