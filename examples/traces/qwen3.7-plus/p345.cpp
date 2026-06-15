#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Point {
    double x, y;
};

double cross(const Point& a, const Point& b) {
    return a.x * b.y - a.y * b.x;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    vector<Point> V(N);
    for (int i = 0; i < N; ++i) {
        cin >> V[i].x >> V[i].y;
    }

    double total_area = 0;
    for (int i = 0; i < N; ++i) {
        total_area += cross(V[i], V[(i + 1) % N]);
    }
    total_area /= 2.0;

    bool is_ccw = (total_area > 0);
    if (!is_ccw) {
        reverse(V.begin(), V.end());
        total_area = abs(total_area);
    }

    vector<Point> V_dup(4 * N + 1);
    for (int i = 0; i <= 4 * N; ++i) {
        V_dup[i] = V[i % N];
    }

    vector<double> S(4 * N + 1, 0.0);
    for (int i = 0; i < 4 * N; ++i) {
        S[i + 1] = S[i] + cross(V_dup[i], V_dup[i + 1]);
    }

    int P;
    if (!(cin >> P)) return 0;

    const double EPS = 1e-7;

    for (int q = 0; q < P; ++q) {
        Point A, B;
        cin >> A.x >> A.y >> B.x >> B.y;

        auto get_D = [&](int i) {
            int idx = ((i % N) + N) % N;
            return cross({V_dup[idx].x - A.x, V_dup[idx].y - A.y}, {B.x - A.x, B.y - A.y});
        };

        int k = -1, m = -1;
        int check_pts[] = {0, N / 4, N / 2, 3 * N / 4, N - 1};
        for (int idx : check_pts) {
            double d = get_D(idx);
            if (d > EPS && k == -1) k = idx;
            if (d < -EPS && m == -1) m = idx;
        }

        if (k == -1 && m == -1) {
            for (int i = 0; i < N; ++i) {
                double d = get_D(i);
                if (d > EPS && k == -1) k = i;
                if (d < -EPS && m == -1) m = i;
                if (k != -1 && m != -1) break;
            }
        }

        if (k == -1 || m == -1) {
            cout << fixed << setprecision(6) << 0.000000 << "\n";
            continue;
        }

        int L = k, R = k + N - 1;
        int ans_E_plus = k + N - 1;
        while (L <= R) {
            int mid = L + (R - L) / 2;
            if (get_D(mid) <= EPS) {
                ans_E_plus = mid;
                R = mid - 1;
            } else {
                L = mid + 1;
            }
        }
        int E_plus = ans_E_plus - 1;

        if (m <= E_plus) m += N;

        L = m; R = m + N - 1;
        int ans_E_minus = m + N - 1;
        while (L <= R) {
            int mid = L + (R - L) / 2;
            if (get_D(mid) >= -EPS) {
                ans_E_minus = mid;
                R = mid - 1;
            } else {
                L = mid + 1;
            }
        }
        int E_minus = ans_E_minus - 1;

        double d1 = get_D(E_plus);
        double d2 = get_D(E_plus + 1);
        double den1 = d1 - d2;
        double t1 = (abs(den1) < EPS) ? 0.0 : (d1 / den1);
        Point P1;
        P1.x = V_dup[E_plus].x + t1 * (V_dup[E_plus + 1].x - V_dup[E_plus].x);
        P1.y = V_dup[E_plus].y + t1 * (V_dup[E_plus + 1].y - V_dup[E_plus].y);

        double d3 = get_D(E_minus);
        double d4 = get_D(E_minus + 1);
        double den2 = d3 - d4;
        double t2 = (abs(den2) < EPS) ? 0.0 : (d3 / den2);
        Point P2;
        P2.x = V_dup[E_minus].x + t2 * (V_dup[E_minus + 1].x - V_dup[E_minus].x);
        P2.y = V_dup[E_minus].y + t2 * (V_dup[E_minus + 1].y - V_dup[E_minus].y);

        double area_minus = 0.5 * (
            cross(P1, V_dup[E_plus + 1]) +
            (S[E_minus] - S[E_plus + 1]) +
            cross(V_dup[E_minus], P2) +
            cross(P2, P1)
        );

        area_minus = abs(area_minus);
        double ans = min(area_minus, total_area - area_minus);
        if (ans < 0) ans = 0;
        
        cout << fixed << setprecision(6) << ans << "\n";
    }

    return 0;
}
