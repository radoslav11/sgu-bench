#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <cstdlib>

using namespace std;

struct Fraction {
    long long x, y;
    Fraction(long long _x = 0, long long _y = 1) {
        if (_y < 0) { _x = -_x; _y = -_y; }
        long long g = std::gcd(std::abs(_x), _y);
        x = _x / g;
        y = _y / g;
    }
    bool operator<(const Fraction& o) const {
        return x * o.y < o.x * y;
    }
    bool operator==(const Fraction& o) const {
        return x == o.x && y == o.y;
    }
    bool operator<=(const Fraction& o) const {
        return x * o.y <= o.x * y;
    }
    bool operator>=(const Fraction& o) const {
        return x * o.y >= o.x * y;
    }
    bool operator>(const Fraction& o) const {
        return x * o.y > o.x * y;
    }
};

struct Interval {
    Fraction L, R;
    bool operator<(const Interval& o) const {
        if (L == o.L) return R < o.R;
        return L < o.L;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    vector<Interval> wolves(N);
    for (int i = 0; i < N; ++i) {
        long long x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        Fraction s1(x1, y1), s2(x2, y2);
        if (s1 < s2) {
            wolves[i].L = s1;
            wolves[i].R = s2;
        } else {
            wolves[i].L = s2;
            wolves[i].R = s1;
        }
    }

    vector<Interval> sheep(M);
    for (int i = 0; i < M; ++i) {
        long long x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        Fraction s1(x1, y1), s2(x2, y2);
        if (s1 < s2) {
            sheep[i].L = s1;
            sheep[i].R = s2;
        } else {
            sheep[i].L = s2;
            sheep[i].R = s1;
        }
    }

    sort(sheep.begin(), sheep.end());
    vector<Interval> F;
    for (const auto& s : sheep) {
        if (F.empty()) {
            F.push_back(s);
        } else {
            if (s.L <= F.back().R) {
                if (s.R > F.back().R) {
                    F.back().R = s.R;
                }
            } else {
                F.push_back(s);
            }
        }
    }

    vector<Fraction> L_arr, R_arr;
    L_arr.reserve(F.size());
    R_arr.reserve(F.size());
    for (const auto& f : F) {
        L_arr.push_back(f.L);
        R_arr.push_back(f.R);
    }

    sort(wolves.begin(), wolves.end(), [](const Interval& a, const Interval& b) {
        if (a.R == b.R) return a.L < b.L;
        return a.R < b.R;
    });

    int shots = 0;
    Fraction last_S(0, 1);
    bool last_is_open = false;
    bool first = true;

    for (const auto& w : wolves) {
        bool covered = false;
        if (!first) {
            if (last_S > w.L || (last_S == w.L && !last_is_open)) {
                covered = true;
            }
        }

        if (covered) continue;

        auto it = upper_bound(L_arr.begin(), L_arr.end(), w.R);
        int idx = distance(L_arr.begin(), it);

        bool wR_forbidden = false;
        if (idx > 0) {
            if (R_arr[idx - 1] >= w.R) {
                wR_forbidden = true;
            }
        }

        if (!wR_forbidden) {
            last_S = w.R;
            last_is_open = false;
        } else {
            int k = idx - 1;
            if (k >= 0 && w.L < L_arr[k]) {
                last_S = L_arr[k];
                last_is_open = true;
            } else {
                cout << "No solution\n";
                return 0;
            }
        }
        shots++;
        first = false;
    }

    cout << shots << "\n";
    return 0;
}
