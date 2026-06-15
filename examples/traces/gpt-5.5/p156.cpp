#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> p, r;
    DSU(int n=0): p(n), r(n,0) { iota(p.begin(), p.end(), 0); }
    int find(int x){ return p[x]==x?x:p[x]=find(p[x]); }
    void unite(int a,int b){
        a=find(a); b=find(b);
        if(a==b) return;
        if(r[a]<r[b]) swap(a,b);
        p[b]=a;
        if(r[a]==r[b]) r[a]++;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    vector<pair<int,int>> edges;
    vector<vector<int>> adj(N);
    edges.reserve(M);

    for (int i = 0; i < M; i++) {
        int a,b; cin >> a >> b;
        --a; --b;
        edges.push_back({a,b});
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    vector<int> deg(N);
    for (int i = 0; i < N; i++) deg[i] = (int)adj[i].size();

    bool allDeg2 = true;
    for (int i = 0; i < N; i++) if (deg[i] != 2) allDeg2 = false;

    if (allDeg2) {
        vector<int> ans, vis(N,0);
        int cur = 0, prv = -1;
        for (int i = 0; i < N; i++) {
            ans.push_back(cur);
            vis[cur] = 1;
            int nxt = -1;
            for (int to : adj[cur]) if (to != prv) {
                nxt = to;
                if (!vis[to] || (i == N-1 && to == 0)) break;
            }
            prv = cur;
            cur = nxt;
        }
        if (cur != 0) {
            cout << -1 << '\n';
            return 0;
        }
        for (int i = 0; i < N; i++) {
            if (i) cout << ' ';
            cout << ans[i] + 1;
        }
        cout << '\n';
        return 0;
    }

    DSU dsu(N);
    vector<int> isHigh(N,0);
    for (int i = 0; i < N; i++) if (deg[i] > 2) isHigh[i] = 1;

    for (auto [a,b] : edges) {
        if (isHigh[a] && isHigh[b]) dsu.unite(a,b);
    }

    vector<int> compId(N, -1), compRoot;
    unordered_map<int,int> mp;
    for (int i = 0; i < N; i++) if (isHigh[i]) {
        int r = dsu.find(i);
        if (!mp.count(r)) {
            int id = (int)mp.size();
            mp[r] = id;
            compRoot.push_back(r);
        }
        compId[i] = mp[r];
    }

    int C = (int)mp.size();

    vector<vector<int>> compVerts(C), zeroVerts(C);
    for (int i = 0; i < N; i++) if (isHigh[i]) {
        compVerts[compId[i]].push_back(i);
    }

    struct MandEdge {
        int mid;
        int x;
        int y;
        int a;
        int b;
        int ca;
        int cb;
    };

    vector<MandEdge> me;
    vector<vector<pair<int,int>>> byVertex(N); // {mandatory edge id, side 0/1}
    vector<vector<int>> compHalf(C);

    for (int v = 0; v < N; v++) if (deg[v] == 2) {
        int a = adj[v][0], b = adj[v][1];
        if (!isHigh[a] || !isHigh[b]) {
            cout << -1 << '\n';
            return 0;
        }
        int ca = compId[a], cb = compId[b];
        if (ca == cb) {
            cout << -1 << '\n';
            return 0;
        }
        int id = (int)me.size();
        me.push_back({v,a,b,a,b,ca,cb});
        byVertex[a].push_back({id,0});
        byVertex[b].push_back({id,1});
        compHalf[ca].push_back(id*2);
        compHalf[cb].push_back(id*2+1);
    }

    int E = (int)me.size();
    if (E == 0) {
        if (C == 1) {
            vector<int> ans = compVerts[0];
            for (int i = 0; i < (int)ans.size(); i++) {
                if (i) cout << ' ';
                cout << ans[i] + 1;
            }
            cout << '\n';
        } else {
            cout << -1 << '\n';
        }
        return 0;
    }

    for (int v = 0; v < N; v++) if (isHigh[v]) {
        if ((int)byVertex[v].size() > 2) {
            cout << -1 << '\n';
            return 0;
        }
        if (byVertex[v].empty()) zeroVerts[compId[v]].push_back(v);
    }

    for (int c = 0; c < C; c++) {
        if (compHalf[c].size() % 2) {
            cout << -1 << '\n';
            return 0;
        }
    }

    DSU cdsu(C);
    vector<int> compDeg(C,0);
    for (auto &e : me) {
        cdsu.unite(e.ca, e.cb);
        compDeg[e.ca]++;
        compDeg[e.cb]++;
    }
    int root = -1;
    for (int c = 0; c < C; c++) if (compDeg[c]) {
        if (root == -1) root = cdsu.find(c);
        else if (root != cdsu.find(c)) {
            cout << -1 << '\n';
            return 0;
        }
    }

    vector<int> paired(2*E, -1);
    vector<vector<int>> path(2*E);

    auto halfVertex = [&](int h)->int {
        int id = h/2;
        if (h%2==0) return me[id].a;
        else return me[id].b;
    };

    for (int v = 0; v < N; v++) if (isHigh[v] && byVertex[v].size() == 2) {
        int h1 = byVertex[v][0].first*2 + byVertex[v][0].second;
        int h2 = byVertex[v][1].first*2 + byVertex[v][1].second;
        paired[h1] = h2;
        paired[h2] = h1;
        path[h1] = {v};
        path[h2] = {v};
    }

    for (int c = 0; c < C; c++) {
        vector<int> freeH;
        for (int h : compHalf[c]) if (paired[h] == -1) freeH.push_back(h);

        if (freeH.empty()) {
            if (!zeroVerts[c].empty()) {
                cout << -1 << '\n';
                return 0;
            }
            continue;
        }

        for (int i = 0; i < (int)freeH.size(); i += 2) {
            int h1 = freeH[i], h2 = freeH[i+1];
            paired[h1] = h2;
            paired[h2] = h1;

            vector<int> p;
            p.push_back(halfVertex(h1));
            if (i == 0) {
                for (int v : zeroVerts[c]) p.push_back(v);
            }
            p.push_back(halfVertex(h2));

            path[h1] = p;
            reverse(p.begin(), p.end());
            path[h2] = p;
        }
    }

    vector<int> usedHalf(2*E,0), ans;
    int start = 0, h = start;

    while (!usedHalf[h]) {
        usedHalf[h] = 1;
        int ph = paired[h];

        for (int v : path[h]) ans.push_back(v);

        int eid = ph / 2;
        ans.push_back(me[eid].x);

        h = ph ^ 1;
    }

    if (h != start || (int)ans.size() != N) {
        cout << -1 << '\n';
        return 0;
    }

    vector<int> seen(N,0);
    for (int v : ans) {
        if (v < 0 || v >= N || seen[v]) {
            cout << -1 << '\n';
            return 0;
        }
        seen[v] = 1;
    }
    for (int i = 0; i < N; i++) if (!seen[i]) {
        cout << -1 << '\n';
        return 0;
    }

    for (int i = 0; i < N; i++) {
        if (i) cout << ' ';
        cout << ans[i] + 1;
    }
    cout << '\n';
    return 0;
}
