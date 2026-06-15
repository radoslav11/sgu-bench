#include <iostream>
#include <vector>
#include <cstring>
#include <queue>
#include <functional>
using namespace std;

const int MAXN = 105;

vector<int> g1[MAXN], g2[MAXN];
int n, m1, m2;
int match1[MAXN], match2[MAXN];
bool used[MAXN];

bool dfs1(int v) {
    if (used[v]) return false;
    used[v] = true;
    for (int to : g1[v]) {
        if (match1[to] == -1 || dfs1(match1[to])) {
            match1[to] = v;
            return true;
        }
    }
    return false;
}

bool dfs2_for_AB(int v, bool inB[]) {
    if (used[v]) return false;
    used[v] = true;
    for (int to : g2[v]) {
        if (inB[to] && (match2[to] == -1 || dfs2_for_AB(match2[to], inB))) {
            match2[to] = v;
            return true;
        }
    }
    return false;
}

int main() {
    cin >> n >> m1 >> m2;
    vector<vector<int>> adj(n+1);
    for (int i = 0; i < m1; i++) {
        int a, b;
        cin >> a >> b;
        g1[a].push_back(b);
        g1[b].push_back(a);
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    for (int i = 0; i < m2; i++) {
        int a, b;
        cin >> a >> b;
        g2[a].push_back(b);
        g2[b].push_back(a);
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    vector<int> col(n+1, -1);
    for (int i = 1; i <= n; i++) {
        if (col[i] == -1) {
            queue<int> q;
            q.push(i);
            col[i] = 0;
            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (int v : adj[u]) {
                    if (col[v] == -1) {
                        col[v] = col[u] ^ 1;
                        q.push(v);
                    }
                }
            }
        }
    }

    vector<int> L, R;
    for (int i = 1; i <= n; i++) {
        if (col[i] == 0) 
            L.push_back(i);
        else 
            R.push_back(i);
    }

    int maxk = min((int)L.size(), (int)R.size());
    int ans = 0;
    vector<pair<int, int>> juan, rosa;

    for (int k = maxk; k >= 0; k--) {
        vector<int> A;
        function<bool(int, int)> comb = [&](int i, int j) -> bool {
            if (j == k) {
                memset(match1, -1, sizeof(match1));
                for (int v : A) {
                    memset(used, 0, sizeof(used));
                    dfs1(v);
                }
                int cnt1 = 0;
                for (int v : R) {
                    if (match1[v] != -1) 
                        cnt1++;
                }
                if (cnt1 < k) 
                    return false;

                vector<int> B;
                for (int v : R) {
                    if (match1[v] != -1) {
                        B.push_back(v);
                    }
                }

                bool inB[MAXN] = {false};
                for (int x : B) 
                    inB[x] = true;

                memset(match2, -1, sizeof(match2));
                int cnt2 = 0;
                for (int v : A) {
                    memset(used, 0, sizeof(used));
                    if (dfs2_for_AB(v, inB)) {
                        cnt2++;
                    }
                }
                if (cnt2 == k) {
                    ans = k;
                    juan.clear();
                    rosa.clear();
                    for (int v : R) {
                        if (match1[v] != -1) {
                            juan.push_back(make_pair(match1[v], v));
                        }
                    }
                    for (int v : R) {
                        if (match2[v] != -1) {
                            rosa.push_back(make_pair(match2[v], v));
                        }
                    }
                    return true;
                }
                return false;
            }
            if (i >= (int)L.size()) 
                return false;
            if (comb(i+1, j)) 
                return true;
            A.push_back(L[i]);
            if (comb(i+1, j+1)) 
                return true;
            A.pop_back();
            return false;
        };
        if (comb(0, 0)) {
            break;
        }
    }

    cout << ans << endl;
    for (auto& p : juan) {
        cout << p.first << " " << p.second << endl;
    }
    for (auto& p : rosa) {
        cout << p.first << " " << p.second << endl;
    }
    return 0;
}
