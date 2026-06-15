#include <bits/stdc++.h>
using namespace std;

const int MAXN = 205;
const int INF = 1e9;

int n;
int D[2], Cc[2];
int p[MAXN];
vector<int> g[MAXN];
int dista[MAXN][MAXN];
bool adjm[MAXN][MAXN];

struct Res {
    int val = -1;
    bitset<MAXN> mask;
};

int sumMask(const bitset<MAXN>& m) {
    int s = 0;
    for (int i = 1; i <= n; i++) if (m[i]) s += p[i];
    return s;
}

vector<int> selectTwoGroups(const vector<int>& verts, const vector<int>& group, int K) {
    int m = (int)verts.size();
    if (K > m) K = m;
    if (K < 2) return {};

    vector<int> ord(m);
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&](int a, int b) {
        return p[verts[a]] > p[verts[b]];
    });

    vector<int> take;
    map<int, int> cnt;
    int sum = 0;
    for (int i = 0; i < K; i++) {
        take.push_back(ord[i]);
        cnt[group[ord[i]]]++;
        sum += p[verts[ord[i]]];
    }

    if ((int)cnt.size() >= 2) {
        vector<int> res;
        for (int id : take) res.push_back(verts[id]);
        return res;
    }

    int dominant = group[take[0]];
    int bestSum = -1;
    vector<int> best;

    for (int forced = 0; forced < m; forced++) {
        if (group[forced] == dominant) continue;

        vector<int> curIdx;
        curIdx.push_back(forced);
        int curSum = p[verts[forced]];

        for (int id : ord) {
            if (id == forced) continue;
            if ((int)curIdx.size() == K) break;
            curIdx.push_back(id);
            curSum += p[verts[id]];
        }

        if ((int)curIdx.size() == K && curSum > bestSum) {
            bestSum = curSum;
            best.clear();
            for (int id : curIdx) best.push_back(verts[id]);
        }
    }

    return best;
}

vector<int> selectTwoSides(const vector<int>& A, const vector<int>& B, int K) {
    int total = (int)A.size() + (int)B.size();
    if (K > total) K = total;
    if (K < 2 || A.empty() || B.empty()) return {};

    struct Item {
        int v, side;
    };

    vector<Item> all;
    for (int x : A) all.push_back({x, 0});
    for (int x : B) all.push_back({x, 1});

    sort(all.begin(), all.end(), [&](const Item& x, const Item& y) {
        return p[x.v] > p[y.v];
    });

    vector<Item> take;
    int cnt[2] = {0, 0};
    int sum = 0;

    for (int i = 0; i < K; i++) {
        take.push_back(all[i]);
        cnt[all[i].side]++;
        sum += p[all[i].v];
    }

    if (cnt[0] > 0 && cnt[1] > 0) {
        vector<int> res;
        for (auto it : take) res.push_back(it.v);
        return res;
    }

    int missing = cnt[0] == 0 ? 0 : 1;
    int bestMissing = -1;
    for (auto it : all) {
        if (it.side == missing) {
            bestMissing = it.v;
            break;
        }
    }

    if (bestMissing == -1) return {};

    int worstPos = -1;
    for (int i = 0; i < K; i++) {
        if (worstPos == -1 || p[take[i].v] < p[take[worstPos].v])
            worstPos = i;
    }

    take[worstPos] = {bestMissing, missing};

    vector<int> res;
    for (auto it : take) res.push_back(it.v);
    return res;
}

Res solveComponent(const vector<int>& comp, int DD, int CC) {
    vector<char> in(n + 1, 0);
    for (int v : comp) in[v] = 1;

    Res best;

    if (DD == 0) {
        for (int v : comp) {
            if (p[v] > best.val) {
                best.val = p[v];
                best.mask.reset();
                best.mask[v] = 1;
            }
        }
        return best;
    }

    if (DD % 2 == 0) {
        int r = DD / 2;
        if (CC < 2) return best;

        for (int c : comp) {
            bitset<MAXN> cur;
            int val = 0;

            for (int v : comp) {
                if (dista[c][v] < r) {
                    cur[v] = 1;
                    val += p[v];
                }
            }

            vector<int> boundary, groups;

            for (int v : comp) {
                if (dista[c][v] == r) {
                    int gr = -1;
                    for (int nb : g[c]) {
                        if (dista[nb][v] == dista[c][v] - 1) {
                            gr = nb;
                            break;
                        }
                    }
                    if (gr != -1) {
                        boundary.push_back(v);
                        groups.push_back(gr);
                    }
                }
            }

            vector<int> sel = selectTwoGroups(boundary, groups, CC);
            if (sel.empty()) continue;

            for (int v : sel) {
                cur[v] = 1;
                val += p[v];
            }

            if (val > best.val) {
                best.val = val;
                best.mask = cur;
            }
        }
    } else {
        int r = DD / 2;
        if (CC < 2) return best;

        for (int u = 1; u <= n; u++) {
            for (int v : g[u]) {
                if (u > v) continue;
                if (!in[u] || !in[v]) continue;

                bitset<MAXN> cur;
                int val = 0;
                vector<int> A, B;

                for (int x : comp) {
                    if (dista[u][x] < dista[v][x]) {
                        if (dista[u][x] < r) {
                            cur[x] = 1;
                            val += p[x];
                        } else if (dista[u][x] == r) {
                            A.push_back(x);
                        }
                    } else if (dista[v][x] < dista[u][x]) {
                        if (dista[v][x] < r) {
                            cur[x] = 1;
                            val += p[x];
                        } else if (dista[v][x] == r) {
                            B.push_back(x);
                        }
                    }
                }

                vector<int> sel = selectTwoSides(A, B, CC);
                if (sel.empty()) continue;

                for (int x : sel) {
                    cur[x] = 1;
                    val += p[x];
                }

                if (val > best.val) {
                    best.val = val;
                    best.mask = cur;
                }
            }
        }
    }

    return best;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    cin >> D[0] >> Cc[0] >> D[1] >> Cc[1];

    int totalCost = 0;
    for (int i = 1; i <= n; i++) {
        cin >> p[i];
        totalCost += p[i];
    }

    for (int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;
        g[a].push_back(b);
        g[b].push_back(a);
        adjm[a][b] = adjm[b][a] = true;
    }

    for (int s = 1; s <= n; s++) {
        for (int i = 1; i <= n; i++) dista[s][i] = INF;
        queue<int> q;
        dista[s][s] = 0;
        q.push(s);

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (int to : g[v]) {
                if (dista[s][to] == INF) {
                    dista[s][to] = dista[s][v] + 1;
                    q.push(to);
                }
            }
        }
    }

    static Res best[2][MAXN][MAXN];

    for (int x = 1; x <= n; x++) {
        for (int y : g[x]) {
            vector<int> comp;
            for (int v = 1; v <= n; v++) {
                if (dista[y][v] < dista[x][v]) comp.push_back(v);
            }

            for (int t = 0; t < 2; t++) {
                best[t][x][y] = solveComponent(comp, D[t], Cc[t]);
            }
        }
    }

    int bestKeep = -1;
    bitset<MAXN> bestKept;

    for (int sep = 1; sep <= n; sep++) {
        for (int a : g[sep]) {
            for (int b : g[sep]) {
                if (a == b) continue;

                if (best[0][sep][a].val == -1) continue;
                if (best[1][sep][b].val == -1) continue;

                int keep = best[0][sep][a].val + best[1][sep][b].val;

                if (keep > bestKeep) {
                    bestKeep = keep;
                    bestKept = best[0][sep][a].mask | best[1][sep][b].mask;
                }
            }
        }
    }

    if (bestKeep == -1) {
        cout << -1 << '\n';
        return 0;
    }

    vector<int> destroyed;
    int cost = 0;

    for (int i = 1; i <= n; i++) {
        if (!bestKept[i]) {
            destroyed.push_back(i);
            cost += p[i];
        }
    }

    cout << cost << '\n';
    for (int v : destroyed) cout << v << ' ';
    cout << '\n';

    return 0;
}
