#include<bits/stdc++.h>
using namespace std;

int N, B;
vector<int> adj[10005];
int subtree_size[10005];
int province[10005];
int num_provinces = 0;
vector<int> capitals;

int dfs_size(int u, int parent) {
    subtree_size[u] = 1;
    for(int v : adj[u]) {
        if(v != parent) {
            subtree_size[u] += dfs_size(v, u);
        }
    }
    return subtree_size[u];
}

void dfs_assign(int u, int parent) {
    int cur_size = 1;
    
    for(int v : adj[u]) {
        if(v != parent && province[v] == 0) {
            if(subtree_size[v] >= B && subtree_size[v] <= 3*B) {
                // Create new province from this subtree
                num_provinces++;
                int prov = num_provinces;
                capitals.push_back(v);
                function<void(int,int)> mark = [&](int x, int p) {
                    province[x] = prov;
                    for(int y : adj[x]) {
                        if(y != p && province[y] == 0) {
                            mark(y, x);
                        }
                    }
                };
                mark(v, u);
            } else if(subtree_size[v] < B) {
                // Include in current province
                cur_size += subtree_size[v];
                function<void(int,int)> mark = [&](int x, int p) {
                    province[x] = -1; // Mark for later assignment
                    for(int y : adj[x]) {
                        if(y != p && province[y] == 0) {
                            mark(y, x);
                        }
                    }
                };
                mark(v, u);
            } else {
                // Recursively process
                dfs_assign(v, u);
            }
        }
    }
}

int main(){
    cin >> N >> B;
    
    for(int i = 0; i < N-1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    dfs_size(1, -1);
    
    if(subtree_size[1] > 3*B) {
        cout << 0 << endl;
        return 0;
    }
    
    memset(province, 0, sizeof(province));
    num_provinces = 1;
    capitals.push_back(1);
    
    function<void(int,int,int)> dfs = [&](int u, int p, int prov) {
        province[u] = prov;
        for(int v : adj[u]) {
            if(v != p) {
                if(subtree_size[v] >= B && subtree_size[v] <= 3*B) {
                    num_provinces++;
                    capitals.push_back(v);
                    dfs(v, u, num_provinces);
                } else {
                    dfs(v, u, prov);
                }
            }
        }
    };
    
    dfs(1, -1, 1);
    
    vector<int> prov_size(num_provinces + 1, 0);
    for(int i = 1; i <= N; i++) {
        prov_size[province[i]]++;
    }
    
    for(int i = 1; i <= num_provinces; i++) {
        if(prov_size[i] < B || prov_size[i] > 3*B) {
            cout << 0 << endl;
            return 0;
        }
    }
    
    cout << num_provinces << endl;
    for(int i = 1; i <= N; i++) {
        cout << province[i];
        if(i < N) cout << " ";
    }
    cout << endl;
    for(int i = 0; i < capitals.size(); i++) {
        cout << capitals[i];
        if(i < capitals.size() - 1) cout << " ";
    }
    cout << endl;
    
    return 0;
}
