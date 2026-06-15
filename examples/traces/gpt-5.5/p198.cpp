#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;
const double PI = acos(-1.0);

struct Circle {
    double x, y, r;
};

struct Arc {
    int s, t;
    int c;
    double a, b;
    bool used = false;
};

double normAng(double a) {
    while (a < 0) a += 2 * PI;
    while (a >= 2 * PI) a -= 2 * PI;
    return a;
}

double principal(double a) {
    while (a <= -PI) a += 2 * PI;
    while (a > PI) a -= 2 * PI;
    return a;
}

double dist2(double x1, double y1, double x2, double y2) {
    double dx = x1 - x2, dy = y1 - y2;
    return dx * dx + dy * dy;
}

struct DSU {
    vector<int> p;
    DSU(int n = 0) : p(n) { iota(p.begin(), p.end(), 0); }
    int add() {
        p.push_back((int)p.size());
        return (int)p.size() - 1;
    }
    int find(int x) {
        return p[x] == x ? x : p[x] = find(p[x]);
    }
    void unite(int a, int b) {
        a = find(a), b = find(b);
        if (a != b) p[b] = a;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;

    vector<Circle> c(N);
    for (int i = 0; i < N; i++) cin >> c[i].x >> c[i].y >> c[i].r;

    double sx, sy, sr;
    cin >> sx >> sy >> sr;

    for (auto &v : c) v.r += sr;

    vector<Arc> arcs;
    vector<pair<double, double>> verts;
    DSU dsu;

    auto addVertex = [&](double x, double y) {
        int id = dsu.add();
        verts.push_back({x, y});
        return id;
    };

    for (int i = 0; i < N; i++) {
        vector<double> cuts = {0, 2 * PI};
        vector<pair<double, double>> cover;
        bool full = false;

        for (int j = 0; j < N; j++) if (i != j) {
            double dx = c[j].x - c[i].x;
            double dy = c[j].y - c[i].y;
            double d = hypot(dx, dy);

            if (d < EPS) {
                if (c[i].r < c[j].r - EPS) full = true;
                continue;
            }

            double C = (c[j].r * c[j].r - d * d - c[i].r * c[i].r) / (2 * d * c[i].r);
            double base = atan2(dy, dx);

            if (C >= 1 - EPS) {
                full = true;
            } else if (C > -1 + EPS) {
                double q = acos(C);
                double l = normAng(base + q);
                double r = normAng(base + 2 * PI - q);

                cuts.push_back(l);
                cuts.push_back(r);

                if (l <= r) {
                    cover.push_back({l, r});
                } else {
                    cover.push_back({l, 2 * PI});
                    cover.push_back({0, r});
                }
            }
        }

        if (full) continue;

        sort(cuts.begin(), cuts.end());
        vector<double> u;
        for (double x : cuts) {
            if (u.empty() || fabs(x - u.back()) > 1e-8) u.push_back(x);
        }

        for (int k = 0; k + 1 < (int)u.size(); k++) {
            double a = u[k], b = u[k + 1];
            if (b - a < 1e-8) continue;

            double m = (a + b) / 2;
            bool inside = false;

            for (auto [l, r] : cover) {
                if (m > l + EPS && m < r - EPS) {
                    inside = true;
                    break;
                }
            }

            if (!inside) {
                double x1 = c[i].x + c[i].r * cos(a);
                double y1 = c[i].y + c[i].r * sin(a);
                double x2 = c[i].x + c[i].r * cos(b);
                double y2 = c[i].y + c[i].r * sin(b);

                int v1 = addVertex(x1, y1);
                int v2 = addVertex(x2, y2);

                arcs.push_back({v1, v2, i, a, b, false});
            }
        }
    }

    int V = verts.size();

    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            if (dist2(verts[i].first, verts[i].second, verts[j].first, verts[j].second) < 1e-12) {
                dsu.unite(i, j);
            }
        }
    }

    unordered_map<int, vector<int>> out;
    for (int i = 0; i < (int)arcs.size(); i++) {
        arcs[i].s = dsu.find(arcs[i].s);
        arcs[i].t = dsu.find(arcs[i].t);
        out[arcs[i].s].push_back(i);
    }

    auto pointAngle = [&](double x, double y) {
        return atan2(y - sy, x - sx);
    };

    auto arcWinding = [&](const Arc &e) {
        const Circle &cc = c[e.c];
        double res = 0;
        int parts = max(1, (int)ceil((e.b - e.a) / (PI / 12)));
        double prev = pointAngle(cc.x + cc.r * cos(e.a), cc.y + cc.r * sin(e.a));
        for (int k = 1; k <= parts; k++) {
            double ang = e.a + (e.b - e.a) * k / parts;
            double cur = pointAngle(cc.x + cc.r * cos(ang), cc.y + cc.r * sin(ang));
            res += principal(cur - prev);
            prev = cur;
        }
        return res;
    };

    for (int i = 0; i < (int)arcs.size(); i++) {
        if (arcs[i].used) continue;

        double wind = 0;
        int cur = i;

        while (!arcs[cur].used) {
            arcs[cur].used = true;
            wind += arcWinding(arcs[cur]);

            int v = arcs[cur].t;
            int nxt = -1;
            for (int id : out[v]) {
                if (!arcs[id].used) {
                    nxt = id;
                    break;
                }
            }
            if (nxt == -1) break;
            cur = nxt;
        }

        int w = (int)round(wind / (2 * PI));
        if (w < 0) {
            cout << "NO\n";
            return 0;
        }
    }

    cout << "YES\n";
    return 0;
}
