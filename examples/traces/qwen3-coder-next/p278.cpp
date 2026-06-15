#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
using namespace std;

double eval(double p, int N, int A, int B, const vector<int>& a, const vector<int>& b, const vector<int>& c) {
    // Check all fuels with b[i] == 0 and a[i] > 0: require p >= c[i]/a[i]
    for (int i = 0; i < N; i++) {
        if (b[i] == 0 && a[i] > 0) {
            double required = static_cast<double>(c[i]) / a[i];
            if (p < required - 1e-8) {
                return 1e18;
            }
        }
    }

    double q_needed = 0.0;
    for (int i = 0; i < N; i++) {
        if (b[i] == 0) {
            // Skip: either a[i]==0 (so no constraint) or already handled above for a[i]>0
            continue;
        }
        // For fuel i: a[i] * p + b[i] * q >= c[i]
        double q_req = (static_cast<double>(c[i]) - static_cast<double>(a[i]) * p) / static_cast<double>(b[i]);
        if (q_req > q_needed) {
            q_needed = q_req;
        }
    }
    if (q_needed < 0) {
        q_needed = 0;
    }
    return static_cast<double>(A) * p + static_cast<double>(B) * q_needed;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int N, A, B;
    cin >> N >> A >> B;
    vector<int> a(N), b(N), c(N);
    for (int i = 0; i < N; i++) {
        cin >> a[i] >> b[i] >> c[i];
    }

    double lo = 0.0, hi = 300.0;
    double best_val = 1e18;

    // 100 iterations of ternary search for convex function minimization
    for (int iter = 0; iter < 100; iter++) {
        double m1 = lo + (hi - lo) / 3.0;
        double m2 = hi - (hi - lo) / 3.0;

        double f1 = eval(m1, N, A, B, a, b, c);
        double f2 = eval(m2, N, A, B, a, b, c);

        if (f1 < f2) {
            best_val = min(best_val, f1);
            hi = m2;
        } else {
            best_val = min(best_val, f2);
            lo = m1;
        }
    }

    cout << fixed << setprecision(6) << best_val << endl;
    return 0;
}
