#include <bits/stdc++.h>
using namespace std;

struct Fraction {
    long long num, den;

    Fraction(long long n = 0, long long d = 1) {
        if (d < 0) n = -n, d = -d;
        long long g = std::gcd(llabs(n), llabs(d));
        num = n / g;
        den = d / g;
    }

    bool operator<(const Fraction& other) const {
        if (num != other.num) return num < other.num;
        return den < other.den;
    }
};

struct PointKey {
    Fraction x, y;

    bool operator<(const PointKey& other) const {
        if (x < other.x) return true;
        if (other.x < x) return false;
        return y < other.y;
    }
};

struct Point {
    long double x, y;
};

struct Line {
    long long A, B, C;
    long double dx, dy;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<Line> lines(N);

    for (int i = 0; i < N; i++) {
        long long x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;

        long long A = y1 - y2;
        long long B = x2 - x1;
        long long C = -(A * x1 + B * y1);

        lines[i] = {A, B, C, (long double)(x2 - x1), (long double)(y2 - y1)};
    }

    map<PointKey, int> id;
    vector<Point> pts;
    vector<vector<int>> onLine(N);

    auto getId = [&](const PointKey& key) -> int {
        auto it = id.find(key);
        if (it != id.end()) return it->second;

        int newId = (int)pts.size();
        id[key] = newId;

        long double x = (long double)key.x.num / (long double)key.x.den;
        long double y = (long double)key.y.num / (long double)key.y.den;
        pts.push_back({x, y});

        return newId;
    };

    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            long long D = lines[i].A * lines[j].B - lines[j].A * lines[i].B;
            if (D == 0) continue;

            long long xNum = lines[i].B * lines[j].C - lines[j].B * lines[i].C;
            long long yNum = lines[i].C * lines[j].A - lines[j].C * lines[i].A;

            PointKey key{Fraction(xNum, D), Fraction(yNum, D)};
            int p = getId(key);

            onLine[i].push_back(p);
            onLine[j].push_back(p);
        }
    }

    int V = (int)pts.size();
    vector<vector<int>> adj(V);
    set<pair<int, int>> edges;

    for (int i = 0; i < N; i++) {
        auto& v = onLine[i];

        sort(v.begin(), v.end(), [&](int a, int b) {
            long double ta = pts[a].x * lines[i].dx + pts[a].y * lines[i].dy;
            long double tb = pts[b].x * lines[i].dx + pts[b].y * lines[i].dy;
            return ta < tb;
        });

        v.erase(unique(v.begin(), v.end()), v.end());

        for (int k = 0; k + 1 < (int)v.size(); k++) {
            int a = v[k], b = v[k + 1];
            if (a == b) continue;
            if (a > b) swap(a, b);
            edges.insert({a, b});
        }
    }

    for (auto [a, b] : edges) {
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    vector<unordered_map<int, int>> pos(V);

    for (int i = 0; i < V; i++) {
        sort(adj[i].begin(), adj[i].end(), [&](int a, int b) {
            long double aa = atan2(pts[a].y - pts[i].y, pts[a].x - pts[i].x);
            long double bb = atan2(pts[b].y - pts[i].y, pts[b].x - pts[i].x);
            return aa < bb;
        });

        for (int j = 0; j < (int)adj[i].size(); j++) {
            pos[i][adj[i][j]] = j;
        }
    }

    set<pair<int, int>> used;
    vector<long double> areas;

    for (int u = 0; u < V; u++) {
        for (int v : adj[u]) {
            if (used.count({u, v})) continue;

            int su = u, sv = v;
            long double twiceArea = 0;

            while (!used.count({u, v})) {
                used.insert({u, v});

                twiceArea += pts[u].x * pts[v].y - pts[u].y * pts[v].x;

                int idx = pos[v][u];
                int deg = (int)adj[v].size();

                int nextIdx = (idx - 1 + deg) % deg;
                int w = adj[v][nextIdx];

                u = v;
                v = w;
            }

            if (u == su && v == sv) {
                long double area = twiceArea / 2.0L;
                if (area > 1e-8L) {
                    areas.push_back(area);
                }
            }
        }
    }

    sort(areas.begin(), areas.end());

    cout << areas.size() << '\n';
    cout << fixed << setprecision(4);
    for (long double a : areas) {
        cout << (double)a << '\n';
    }

    return 0;
}
