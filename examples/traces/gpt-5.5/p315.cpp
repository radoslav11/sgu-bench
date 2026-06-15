#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;
const double PI = acos(-1.0);
const double TWO_PI = 2.0 * PI;
const double NEG = -1e100;

struct Point {
    double x, y;
};

Point operator + (Point a, Point b) { return {a.x + b.x, a.y + b.y}; }
Point operator - (Point a, Point b) { return {a.x - b.x, a.y - b.y}; }
Point operator * (Point a, double k) { return {a.x * k, a.y * k}; }

double cross(Point a, Point b) {
    return a.x * b.y - a.y * b.x;
}

double dot(Point a, Point b) {
    return a.x * b.x + a.y * b.y;
}

double norm(Point a) {
    return sqrt(dot(a, a));
}

bool samePoint(Point a, Point b) {
    return norm(a - b) < 1e-8;
}

bool onSegment(Point p, Point a, Point b) {
    return fabs(cross(p - a, b - a)) < 1e-8 &&
           dot(p - a, p - b) <= 1e-8;
}

double angleOf(Point p) {
    double a = atan2(p.y, p.x);
    if (a < 0) a += TWO_PI;
    return a;
}

double ccwDelta(double a, double b) {
    double d = b - a;
    while (d < 0) d += TWO_PI;
    while (d >= TWO_PI) d -= TWO_PI;
    return d;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;

    vector<Point> A(N), B(N);
    vector<vector<Point>> pts(N);

    for (int i = 0; i < N; i++) {
        cin >> A[i].x >> A[i].y >> B[i].x >> B[i].y;
        pts[i].push_back(A[i]);
        pts[i].push_back(B[i]);
    }

    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            Point r = B[i] - A[i];
            Point s = B[j] - A[j];
            double den = cross(r, s);

            if (fabs(den) > EPS) {
                double t = cross(A[j] - A[i], s) / den;
                Point p = A[i] + r * t;
                if (onSegment(p, A[i], B[i]) && onSegment(p, A[j], B[j])) {
                    pts[i].push_back(p);
                    pts[j].push_back(p);
                }
            } else {
                if (fabs(cross(A[j] - A[i], r)) < EPS) {
                    if (onSegment(A[i], A[j], B[j])) pts[j].push_back(A[i]);
                    if (onSegment(B[i], A[j], B[j])) pts[j].push_back(B[i]);
                    if (onSegment(A[j], A[i], B[i])) pts[i].push_back(A[j]);
                    if (onSegment(B[j], A[i], B[i])) pts[i].push_back(B[j]);
                }
            }
        }
    }

    vector<Point> vertices;

    auto getId = [&](Point p) -> int {
        for (int i = 0; i < (int)vertices.size(); i++) {
            if (samePoint(vertices[i], p)) return i;
        }
        vertices.push_back(p);
        return (int)vertices.size() - 1;
    };

    vector<pair<int,int>> undirectedEdges;
    set<pair<int,int>> usedEdges;

    for (int i = 0; i < N; i++) {
        vector<pair<double,int>> v;

        Point dir = B[i] - A[i];
        double len2 = dot(dir, dir);

        for (Point p : pts[i]) {
            if (!onSegment(p, A[i], B[i])) continue;

            double t = 0;
            if (len2 > EPS) t = dot(p - A[i], dir) / len2;

            int id = getId(p);
            v.push_back({t, id});
        }

        sort(v.begin(), v.end(), [](auto &l, auto &r) {
            if (fabs(l.first - r.first) > 1e-10) return l.first < r.first;
            return l.second < r.second;
        });

        vector<int> ids;
        for (auto &p : v) {
            if (ids.empty() || ids.back() != p.second)
                ids.push_back(p.second);
        }

        for (int k = 0; k + 1 < (int)ids.size(); k++) {
            int u = ids[k], vtx = ids[k + 1];
            if (u == vtx) continue;

            Point p = vertices[u], q = vertices[vtx];
            if (norm(p - q) < EPS) continue;

            if (norm(p) < EPS || norm(q) < EPS) continue;
            if (fabs(cross(p, q)) < EPS) continue; // radial piece, unusable

            int a = min(u, vtx), b = max(u, vtx);
            if (!usedEdges.count({a, b})) {
                usedEdges.insert({a, b});
                undirectedEdges.push_back({u, vtx});
            }
        }
    }

    int V = vertices.size();

    vector<double> ang(V);
    for (int i = 0; i < V; i++) ang[i] = angleOf(vertices[i]);

    struct Arc {
        int to;
        double delta;
        double w;
    };

    vector<vector<Arc>> g(V);

    for (auto [u, v] : undirectedEdges) {
        double d = ccwDelta(ang[u], ang[v]);
        double w = norm(vertices[u] - vertices[v]);

        if (d > EPS && d < PI - EPS) {
            g[u].push_back({v, d, w});
        } else {
            double rd = TWO_PI - d;
            if (rd > EPS && rd < PI - EPS) {
                g[v].push_back({u, rd, w});
            }
        }
    }

    double answer = 0.0;

    for (int start = 0; start < V; start++) {
        if (norm(vertices[start]) < EPS) continue;

        double startAng = ang[start];
        double finishAng = startAng + TWO_PI;

        vector<vector<double>> memo(V, vector<double>(3, NAN));

        function<double(int,int)> dfs = [&](int u, int layer) -> double {
            double curAng = ang[u] + layer * TWO_PI;

            if (curAng > finishAng + 1e-8) return NEG;
            if (u == start && layer == 1) return 0.0;

            double &res = memo[u][layer];
            if (res == res) return res;

            res = NEG;

            for (auto e : g[u]) {
                double nextAng = curAng + e.delta;
                if (nextAng > finishAng + 1e-8) continue;

                int nl = (int)floor((nextAng - ang[e.to]) / TWO_PI + 0.5);
                if (nl < 0 || nl > 2) continue;

                double realNext = ang[e.to] + nl * TWO_PI;
                if (fabs(realNext - nextAng) > 1e-7) continue;
                if (realNext <= curAng + EPS) continue;

                double sub = dfs(e.to, nl);
                if (sub > NEG / 2) {
                    res = max(res, e.w + sub);
                }
            }

            return res;
        };

        double cur = dfs(start, 0);
        if (cur > answer) answer = cur;
    }

    if (answer < 1e-7) answer = 0.0;

    cout.setf(ios::fixed);
    cout << setprecision(5) << answer << "\n";

    return 0;
}
