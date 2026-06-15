#include <bits/stdc++.h>
using namespace std;

struct Point {
    long double x, y;
};

struct Key {
    long long x, y;
    bool operator==(const Key& other) const {
        return x == other.x && y == other.y;
    }
};

struct KeyHash {
    size_t operator()(const Key& k) const {
        uint64_t a = (uint64_t)(k.x + 4000000000000000000LL);
        uint64_t b = (uint64_t)(k.y + 4000000000000000000LL);
        a ^= a >> 30; a *= 0xbf58476d1ce4e5b9ULL;
        a ^= a >> 27; a *= 0x94d049bb133111ebULL;
        a ^= a >> 31;
        b ^= b >> 30; b *= 0xbf58476d1ce4e5b9ULL;
        b ^= b >> 27; b *= 0x94d049bb133111ebULL;
        b ^= b >> 31;
        return (size_t)(a ^ (b + 0x9e3779b97f4a7c15ULL + (a << 6) + (a >> 2)));
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Point> A(n), B(n);
    long double cx1 = 0, cy1 = 0, cx2 = 0, cy2 = 0;

    for (int i = 0; i < n; i++) {
        cin >> A[i].x >> A[i].y;
        cx1 += A[i].x;
        cy1 += A[i].y;
    }

    for (int i = 0; i < n; i++) {
        cin >> B[i].x >> B[i].y;
        cx2 += B[i].x;
        cy2 += B[i].y;
    }

    if (n == 1) {
        cout << fixed << setprecision(12) << 0.0 << '\n';
        return 0;
    }

    cx1 /= n;
    cy1 /= n;
    cx2 /= n;
    cy2 /= n;

    for (int i = 0; i < n; i++) {
        A[i].x -= cx1;
        A[i].y -= cy1;
        B[i].x -= cx2;
        B[i].y -= cy2;
    }

    int pivot = 0;
    long double maxr = -1;
    for (int i = 0; i < n; i++) {
        long double r = A[i].x * A[i].x + A[i].y * A[i].y;
        if (r > maxr) {
            maxr = r;
            pivot = i;
        }
    }

    const long double PI = acosl(-1.0L);
    const long double TWO_PI = 2.0L * PI;
    const long double RAD_EPS = 1e-6L;
    const long double TOL = 2e-6L;
    const long double TOL2 = TOL * TOL;
    const long double CELL = TOL;

    unordered_map<Key, vector<int>, KeyHash> grid;
    grid.reserve(n * 4);

    auto cellKey = [&](long double x, long double y) -> Key {
        return Key{(long long)floor(x / CELL), (long long)floor(y / CELL)};
    };

    for (int i = 0; i < n; i++) {
        grid[cellKey(B[i].x, B[i].y)].push_back(i);
    }

    vector<int> used(n, 0);
    int stamp = 0;

    auto verify = [&](long double theta) -> bool {
        stamp++;
        long double c = cosl(theta);
        long double s = sinl(theta);

        for (int i = 0; i < n; i++) {
            long double x = A[i].x * c - A[i].y * s;
            long double y = A[i].x * s + A[i].y * c;

            Key base = cellKey(x, y);
            int found = -1;

            for (long long dx = -1; dx <= 1 && found == -1; dx++) {
                for (long long dy = -1; dy <= 1 && found == -1; dy++) {
                    Key k{base.x + dx, base.y + dy};
                    auto it = grid.find(k);
                    if (it == grid.end()) continue;

                    for (int idx : it->second) {
                        if (used[idx] == stamp) continue;
                        long double ex = x - B[idx].x;
                        long double ey = y - B[idx].y;
                        if (ex * ex + ey * ey <= TOL2) {
                            found = idx;
                            break;
                        }
                    }
                }
            }

            if (found == -1) return false;
            used[found] = stamp;
        }

        return true;
    };

    long double ans = PI;

    Point a = A[pivot];
    long double ar = a.x * a.x + a.y * a.y;

    for (int i = 0; i < n; i++) {
        Point b = B[i];
        long double br = b.x * b.x + b.y * b.y;

        if (fabsl(ar - br) > RAD_EPS) continue;

        long double cross = a.x * b.y - a.y * b.x;
        long double dot = a.x * b.x + a.y * b.y;
        long double theta = atan2l(cross, dot);

        if (theta < 0) theta += TWO_PI;

        if (verify(theta)) {
            long double diff = min(theta, TWO_PI - theta);
            if (diff < ans) ans = diff;
        }
    }

    cout << fixed << setprecision(12) << (double)ans << '\n';

    return 0;
}
