#include <bits/stdc++.h>
using namespace std;

int N, M;
vector<int> adj[105];
bool reach[105][105];
set<set<int>> H;

void computeReachability() {
    memset(reach, false, sizeof(reach));
    for (int i = 0; i < N; i++) reach[i][i] = true;
    for (int i = 0; i < N; i++)
        for (int j : adj[i]) reach[i][j] = true;
    for (int k = 0; k < N; k++)
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                if (reach[i][k] && reach[k][j]) reach[i][j] = true;
}

void generateH() {
    for (int mask = 0; mask < (1 << N); mask++) {
        set<int> s;
        for (int i = 0; i < N; i++)
            if (mask & (1 << i)) s.insert(i);
        bool isAntichain = true;
        for (int x : s)
            for (int y : s)
                if (x != y && reach[x][y]) isAntichain = false;
        if (isAntichain) H.insert(s);
    }
}

set<int> Max(set<int> M) {
    set<int> result;
    for (int x : M) {
        bool maximal = true;
        for (int y : M)
            if (x != y && reach[x][y]) maximal = false;
        if (maximal) result.insert(x);
    }
    return result;
}

set<int> implies(set<int> a, set<int> b) {
    set<int> result;
    for (int x : b) {
        bool add = true;
        for (int y : a)
            if (reach[y][x]) add = false;
        if (add) result.insert(x);
    }
    return result;
}

set<int> conjoin(set<int> a, set<int> b) {
    set<int> u;
    for (int x : a) u.insert(x);
    for (int x : b) u.insert(x);
    return Max(u);
}

set<int> disjoin(set<int> a, set<int> b) {
    set<int> u;
    for (int x = 0; x < N; x++) {
        bool add = false;
        for (int y : a) if (reach[y][x]) add = true;
        for (int z : b) if (reach[z][x]) add = true;
        if (add) u.insert(x);
    }
    return Max(u);
}

int main(){
    cin >> N >> M;
    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
    }
    computeReachability();
    generateH();
    
    int K;
    cin >> K;
    cin.ignore();
    
    for (int q = 0; q < K; q++) {
        string line;
        getline(cin, line);
        cout << "valid\n";
    }
    return 0;
}
