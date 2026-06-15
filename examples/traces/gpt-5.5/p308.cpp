#include <bits/stdc++.h>
using namespace std;

const double PI = acos(-1.0);

struct Point {
    double z, th;
};

static double normAng(double a) {
    while (a > PI) a -= 2 * PI;
    while (a < -PI) a += 2 * PI;
    return a;
}

static double edgeWeight(const Point& a, const Point& b) {
    double dz = b.z - a.z;
    double dt = normAng(b.th - a.th);
    double zm = (a.z + b.z) * 0.5;
    double E = (1.0 + 2.0 * zm * zm) / (1.0 + zm * zm);
    double G = 1.0 + zm * zm;
    return sqrt(E * dz * dz + G * dt * dt);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    double xA, yA, zA, xB, yB, zB;
    if (!(cin >> xA >> yA >> zA >> xB >> yB >> zB)) return 0;

    double thA = atan2(yA, xA);
    double thB = atan2(yB, xB);
    if (thA < 0) thA += 2 * PI;
    if (thB < 0) thB += 2 * PI;

    const int NZ = 201;
    const int NT = 720;
    const int L = 6;

    const double dz = 2.0 / (NZ - 1);
    const double dth = 2.0 * PI / NT;

    int gridN = NZ * NT;
    int start = gridN;
    int finish = gridN + 1;
    int totalN = gridN + 2;

    vector<double> dist(totalN, 1e100);
    vector<char> used(totalN, 0);

    auto id = [&](int i, int j) {
        j %= NT;
        if (j < 0) j += NT;
        return i * NT + j;
    };

    auto gridPoint = [&](int i, int j) -> Point {
        return {-1.0 + dz * i, dth * j};
    };

    Point A{zA, thA}, B{zB, thB};

    auto nodePoint = [&](int v) -> Point {
        if (v == start) return A;
        if (v == finish) return B;
        int i = v / NT;
        int j = v % NT;
        return gridPoint(i, j);
    };

    auto nearestI = [&](double z) {
        int i = int(round((z + 1.0) / dz));
        return max(0, min(NZ - 1, i));
    };

    auto nearestJ = [&](double th) {
        int j = int(round(th / dth)) % NT;
        if (j < 0) j += NT;
        return j;
    };

    int iA = nearestI(zA), jA = nearestJ(thA);
    int iB = nearestI(zB), jB = nearestJ(thB);

    auto periodicDiffIndex = [&](int a, int b) {
        int d = abs(a - b);
        return min(d, NT - d);
    };

    priority_queue<pair<double,int>, vector<pair<double,int>>, greater<pair<double,int>>> pq;
    dist[start] = 0.0;
    pq.push({0.0, start});

    vector<pair<int,int>> moves;
    for (int di = -L; di <= L; ++di) {
        for (int dj = -L; dj <= L; ++dj) {
            if (di == 0 && dj == 0) continue;
            if (std::gcd(abs(di), abs(dj)) == 1) {
                moves.push_back({di, dj});
            }
        }
    }

    auto relax = [&](int v, int to, double w) {
        if (dist[to] > dist[v] + w) {
            dist[to] = dist[v] + w;
            pq.push({dist[to], to});
        }
    };

    while (!pq.empty()) {
        auto [du, v] = pq.top();
        pq.pop();

        if (used[v]) continue;
        used[v] = 1;
        if (v == finish) break;

        Point pv = nodePoint(v);

        if (v < gridN) {
            int i = v / NT;
            int j = v % NT;

            for (auto [di, dj] : moves) {
                int ni = i + di;
                if (ni < 0 || ni >= NZ) continue;
                int nj = (j + dj) % NT;
                if (nj < 0) nj += NT;

                int to = id(ni, nj);
                Point q = gridPoint(ni, nj);
                relax(v, to, edgeWeight(pv, q));
            }

            if (abs(i - iB) <= L && periodicDiffIndex(j, jB) <= L) {
                relax(v, finish, edgeWeight(pv, B));
            }
            if (abs(i - iA) <= L && periodicDiffIndex(j, jA) <= L) {
                relax(v, start, edgeWeight(pv, A));
            }
        } else {
            int ci = (v == start ? iA : iB);
            int cj = (v == start ? jA : jB);

            for (int di = -L; di <= L; ++di) {
                int ni = ci + di;
                if (ni < 0 || ni >= NZ) continue;

                for (int dj = -L; dj <= L; ++dj) {
                    int nj = (cj + dj) % NT;
                    if (nj < 0) nj += NT;

                    int to = id(ni, nj);
                    Point q = gridPoint(ni, nj);
                    relax(v, to, edgeWeight(pv, q));
                }
            }

            int other = (v == start ? finish : start);
            relax(v, other, edgeWeight(pv, nodePoint(other)));
        }
    }

    cout.setf(ios::fixed);
    cout << setprecision(10) << dist[finish] << '\n';

    return 0;
}
