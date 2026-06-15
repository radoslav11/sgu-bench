#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

long long dist(long long r1, long long c1, long long r2, long long c2) {
    return max(abs(r1 - r2), abs(c1 - c2));
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long N;
    if (!(cin >> N)) return 0;

    long long P1, Q1, P2, Q2, P3, Q3;
    cin >> P1 >> Q1 >> P2 >> Q2 >> P3 >> Q3;

    long long D = dist(P1, Q1, P2, Q2);

    if (D <= 3) {
        cout << "NO\n" << max(0LL, D - 1) << "\n";
        return 0;
    }

    long long max_m = (D - 2) / 2;
    for (long long m = 1; m <= max_m; ++m) {
        // Check if Black-White king can intercept White king
        long long r_min_W = max({P2 - m, P3 - m, 1LL});
        long long r_max_W = min({P2 + m, P3 + m, N});
        long long c_min_W = max({Q2 - m, Q3 - m, 1LL});
        long long c_max_W = min({Q2 + m, Q3 + m, N});

        bool valid_W = false;
        if (r_min_W <= r_max_W && c_min_W <= c_max_W) {
            long long r_proj = max(r_min_W, min(P1, r_max_W));
            long long c_proj = max(c_min_W, min(Q1, c_max_W));
            long long d_min = dist(r_proj, c_proj, P1, Q1);

            long long d_max = 0;
            d_max = max(d_max, dist(r_min_W, c_min_W, P1, Q1));
            d_max = max(d_max, dist(r_min_W, c_max_W, P1, Q1));
            d_max = max(d_max, dist(r_max_W, c_min_W, P1, Q1));
            d_max = max(d_max, dist(r_max_W, c_max_W, P1, Q1));

            if (d_min <= D - m && d_max >= D - 2 * m) {
                valid_W = true;
            }
        }

        // Check if Black-White king can intercept Black king
        long long r_min_B = max({P1 - m, P3 - m, 1LL});
        long long r_max_B = min({P1 + m, P3 + m, N});
        long long c_min_B = max({Q1 - m, Q3 - m, 1LL});
        long long c_max_B = min({Q1 + m, Q3 + m, N});

        bool valid_B = false;
        if (r_min_B <= r_max_B && c_min_B <= c_max_B) {
            long long r_proj = max(r_min_B, min(P2, r_max_B));
            long long c_proj = max(c_min_B, min(Q2, c_max_B));
            long long d_min = dist(r_proj, c_proj, P2, Q2);

            long long d_max = 0;
            d_max = max(d_max, dist(r_min_B, c_min_B, P2, Q2));
            d_max = max(d_max, dist(r_min_B, c_max_B, P2, Q2));
            d_max = max(d_max, dist(r_max_B, c_min_B, P2, Q2));
            d_max = max(d_max, dist(r_max_B, c_max_B, P2, Q2));

            if (d_min <= D - m && d_max >= D - 2 * m) {
                valid_B = true;
            }
        }

        if (valid_W || valid_B) {
            cout << "YES\n" << m << "\n";
            return 0;
        }
    }

    cout << "NO\n" << D - 1 << "\n";
    return 0;
}
