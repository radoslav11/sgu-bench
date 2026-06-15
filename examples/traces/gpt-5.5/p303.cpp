#include <bits/stdc++.h>
using namespace std;

struct Point {
    long long x, y;
};

long long crossp(const Point& a, const Point& b) {
    return a.x * b.y - a.y * b.x;
}

struct Segment {
    int u, v, w, idx;
};

struct Half {
    int from, to, eid;
};

struct Dinic {
    struct Edge {
        int to, rev, cap;
    };

    int n;
    vector<vector<Edge>> g;
    vector<int> level, it;

    Dinic(int n = 0) : n(n), g(n), level(n), it(n) {}

    void addEdge(int v, int to, int cap) {
        Edge a{to, (int)g[to].size(), cap};
        Edge b{v, (int)g[v].size(), 0};
        g[v].push_back(a);
        g[to].push_back(b);
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (auto &e : g[v]) {
                if (e.cap > 0 && level[e.to] == -1) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
        }

        return level[t] != -1;
    }

    int dfs(int v, int t, int pushed) {
        if (v == t || pushed == 0) return pushed;

        for (int &i = it[v]; i < (int)g[v].size(); i++) {
            Edge &e = g[v][i];

            if (e.cap > 0 && level[e.to] == level[v] + 1) {
                int tr = dfs(e.to, t, min(pushed, e.cap));
                if (tr) {
                    e.cap -= tr;
                    g[e.to][e.rev].cap += tr;
                    return tr;
                }
            }
        }

        return 0;
    }

    int maxflow(int s, int t) {
        int flow = 0;

        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);

            while (true) {
                int pushed = dfs(s, t, INT_MAX);
                if (!pushed) break;
                flow += pushed;
            }
        }

        return flow;
    }

    vector<int> reachable(int s) {
        vector<int> used(n, 0);
        queue<int> q;
        used[s] = 1;
        q.push(s);

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (auto &e : g[v]) {
                if (e.cap > 0 && !used[e.to]) {
                    used[e.to] = 1;
                    q.push(e.to);
                }
            }
        }

        return used;
    }
};

bool angleCmp(const int a, const int b, const vector<Half>& half, const vector<Point>& pts) {
    Point pa{
        pts[half[a].to].x - pts[half[a].from].x,
        pts[half[a].to].y - pts[half[a].from].y
    };
    Point pb{
        pts[half[b].to].x - pts[half[b].from].x,
        pts[half[b].to].y - pts[half[b].from].y
    };

    auto upper = [](const Point& p) {
        return p.y > 0 || (p.y == 0 && p.x > 0);
    };

    bool ua = upper(pa), ub = upper(pb);
    if (ua != ub) return ua > ub;

    long long cr = crossp(pa, pb);
    return cr > 0;
}

bool pointInsidePolygon(const vector<int>& poly, const vector<Point>& pts, const Point& p) {
    bool inside = false;
    int n = poly.size();

    for (int i = 0; i < n; i++) {
        Point a = pts[poly[i]];
        Point b = pts[poly[(i + 1) % n]];

        if ((a.y > p.y) != (b.y > p.y)) {
            long double xint = (long double)(b.x - a.x) * (p.y - a.y) / (long double)(b.y - a.y) + a.x;
            if ((long double)p.x < xint) inside = !inside;
        }
    }

    return inside;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    map<pair<long long, long long>, int> id;
    vector<Point> pts;
    vector<Segment> segs(N);

    auto getId = [&](long long x, long long y) -> int {
        pair<long long, long long> key = {x, y};
        auto it = id.find(key);
        if (it != id.end()) return it->second;

        int nid = pts.size();
        id[key] = nid;
        pts.push_back({x, y});
        return nid;
    };

    for (int i = 0; i < N; i++) {
        long long x1, y1, x2, y2;
        int v;
        cin >> x1 >> y1 >> x2 >> y2 >> v;

        int a = getId(x1, y1);
        int b = getId(x2, y2);

        segs[i] = {a, b, v, i + 1};
    }

    Point hq1, hq2;
    cin >> hq1.x >> hq1.y >> hq2.x >> hq2.y;

    int V = pts.size();

    vector<Half> half;
    vector<vector<int>> out(V);

    for (int i = 0; i < N; i++) {
        int h1 = half.size();
        half.push_back({segs[i].u, segs[i].v, i});
        int h2 = half.size();
        half.push_back({segs[i].v, segs[i].u, i});

        out[segs[i].u].push_back(h1);
        out[segs[i].v].push_back(h2);
    }

    for (int v = 0; v < V; v++) {
        sort(out[v].begin(), out[v].end(), [&](int a, int b) {
            return angleCmp(a, b, half, pts);
        });
    }

    vector<int> pos(2 * N);
    for (int v = 0; v < V; v++) {
        for (int i = 0; i < (int)out[v].size(); i++) {
            pos[out[v][i]] = i;
        }
    }

    vector<int> faceOfHalf(2 * N, -1);
    vector<vector<int>> faces;
    vector<long long> area2;

    for (int start = 0; start < 2 * N; start++) {
        if (faceOfHalf[start] != -1) continue;

        int fid = faces.size();
        vector<int> poly;
        long long ar = 0;

        int h = start;
        while (faceOfHalf[h] == -1) {
            faceOfHalf[h] = fid;
            poly.push_back(half[h].from);
            ar += crossp(pts[half[h].from], pts[half[h].to]);

            int v = half[h].to;
            int rev = h ^ 1;
            int p = pos[rev];
            int deg = out[v].size();

            h = out[v][(p - 1 + deg) % deg];
        }

        faces.push_back(poly);
        area2.push_back(ar);
    }

    int F = faces.size();
    int source = -1, sink = -1;

    for (int i = 0; i < F; i++) {
        if (area2[i] <= 0) continue;

        if (source == -1 && pointInsidePolygon(faces[i], pts, hq1)) source = i;
        if (sink == -1 && pointInsidePolygon(faces[i], pts, hq2)) sink = i;
    }

    Dinic dinic(F);

    vector<pair<int, int>> dualEnds(N);

    for (int i = 0; i < N; i++) {
        int f1 = faceOfHalf[2 * i];
        int f2 = faceOfHalf[2 * i + 1];

        dualEnds[i] = {f1, f2};

        dinic.addEdge(f1, f2, segs[i].w);
        dinic.addEdge(f2, f1, segs[i].w);
    }

    int answer = dinic.maxflow(source, sink);
    vector<int> reach = dinic.reachable(source);

    vector<int> result;
    for (int i = 0; i < N; i++) {
        int a = dualEnds[i].first;
        int b = dualEnds[i].second;

        if (reach[a] != reach[b]) {
            result.push_back(segs[i].idx);
        }
    }

    cout << answer << '\n';
    cout << result.size() << '\n';

    for (int i = 0; i < (int)result.size(); i++) {
        if (i) cout << ' ';
        cout << result[i];
    }
    cout << '\n';

    return 0;
}
