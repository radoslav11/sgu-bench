#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXN = 16005;
int head[MAXN];
int to_arr[2 * MAXN];
int nxt[2 * MAXN];
int edge_cnt = 0;

void add_edge(int u, int v) {
    to_arr[++edge_cnt] = v;
    nxt[edge_cnt] = head[u];
    head[u] = edge_cnt;
}

int sz[MAXN];
int max_child[MAXN];
int parent_node[MAXN];
int order[MAXN];
int stack_arr[MAXN];

void compute_sizes(int n) {
    int top = 0;
    int order_sz = 0;
    
    stack_arr[++top] = 1;
    parent_node[1] = 0;
    
    while (top > 0) {
        int u = stack_arr[top--];
        order[++order_sz] = u;
        for (int e = head[u]; e; e = nxt[e]) {
            int v = to_arr[e];
            if (v != parent_node[u]) {
                parent_node[v] = u;
                stack_arr[++top] = v;
            }
        }
    }
    
    for (int i = 1; i <= n; ++i) {
        sz[i] = 1;
        max_child[i] = 0;
    }
    
    for (int i = order_sz; i >= 1; --i) {
        int u = order[i];
        for (int e = head[u]; e; e = nxt[e]) {
            int v = to_arr[e];
            if (v != parent_node[u]) {
                sz[u] += sz[v];
                if (sz[v] > max_child[u]) {
                    max_child[u] = sz[v];
                }
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    while (cin >> n) {
        edge_cnt = 0;
        for (int i = 1; i <= n; ++i) {
            head[i] = 0;
        }
        
        for (int i = 0; i < n - 1; ++i) {
            int u, v;
            cin >> u >> v;
            add_edge(u, v);
            add_edge(v, u);
        }
        
        compute_sizes(n);
        
        int min_val = n + 1;
        vector<int> centroids;
        
        for (int i = 1; i <= n; ++i) {
            int val = max(max_child[i], n - sz[i]);
            if (val < min_val) {
                min_val = val;
                centroids.clear();
                centroids.push_back(i);
            } else if (val == min_val) {
                centroids.push_back(i);
            }
        }
        
        cout << min_val << " " << centroids.size() << "\n";
        for (size_t i = 0; i < centroids.size(); ++i) {
            cout << centroids[i] << (i + 1 == centroids.size() ? "" : " ");
        }
        cout << "\n";
    }
    
    return 0;
}
