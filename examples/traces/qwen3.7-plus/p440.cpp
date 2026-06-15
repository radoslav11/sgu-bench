#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct Point {
    long long x, y;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    vector<Point> pts(N);
    for (int i = 0; i < N; ++i) {
        cin >> pts[i].x >> pts[i].y;
    }

    long long total_ways = 0;

    for (int i = 0; i < N; ++i) {
        long long px = pts[i].x, py = pts[i].y;
        for (int j = i + 1; j < N; ++j) {
            long long ux_ij = pts[j].x - px;
            long long uy_ij = pts[j].y - py;
            long long u2_ij = ux_ij * ux_ij + uy_ij * uy_ij;

            long long ux_ji = -ux_ij;
            long long uy_ji = -uy_ij;
            long long u2_ji = u2_ij;

            for (int k = j + 1; k < N; ++k) {
                long long vx_ik = pts[k].x - px;
                long long vy_ik = pts[k].y - py;
                long long v2_ik = vx_ik * vx_ik + vy_ik * vy_ik;

                long long vx_jk = vx_ik - ux_ij;
                long long vy_jk = vy_ik - uy_ij;
                long long v2_jk = vx_jk * vx_jk + vy_jk * vy_jk;

                auto check_valid = [&](long long X, long long Y2) -> bool {
                    if (X <= 0) return false;

                    double est = 1000.0 * (double)X / sqrt((double)Y2);
                    int start_V = max(0, (int)est - 2);
                    int best_V = -1;
                    
                    __int128 X2_128 = (__int128)X * X;
                    __int128 Y2_128 = (__int128)Y2;
                    __int128 target = 1000000LL * X2_128;

                    for (int V = start_V; V <= start_V + 5; ++V) {
                        if (V > 1000) break;
                        __int128 lhs = (__int128)V * V * Y2_128;
                        __int128 rhs = (__int128)(V + 1) * (V + 1) * Y2_128;
                        if (target >= lhs && target < rhs) {
                            best_V = V;
                            break;
                        }
                    }

                    if (best_V == -1) return false;
                    int d = best_V % 10;
                    if (d == 0) return false;

                    auto check_angle = [&](int deg) -> bool {
                        if (deg == 1) return true;
                        if (deg == 2) return 2 * X2_128 >= Y2_128;
                        if (deg == 3) return 4 * X2_128 >= 3 * Y2_128;
                        
                        __float128 z = (__float128)X2_128 / Y2_128;
                        if (deg == 4) return 8 * z * z - 8 * z + 1 >= 0;
                        if (deg == 5) return 16 * z * z - 20 * z + 5 >= 0;
                        if (deg == 6) return 16 * z * z - 16 * z + 1 >= 0;
                        if (deg == 7) return 64 * z * z * z - 112 * z * z + 56 * z - 7 >= 0;
                        if (deg == 8) return 16 * z * z * z * z - 32 * z * z * z + 24 * z * z - 8 * z + 1 >= 0;
                        if (deg == 9) return 64 * z * z * z - 96 * z * z + 36 * z - 3 >= 0;
                        if (deg == 10) return 256 * z * z * z * z - 512 * z * z * z + 304 * z * z - 48 * z + 1 >= 0;
                        return false;
                    };

                    return check_angle(d) && !check_angle(d + 1);
                };

                long long X_ik = ux_ij * vx_ik + uy_ij * vy_ik;
                if (X_ik > 0) {
                    long long Y2_ik = u2_ij * v2_ik;
                    if (check_valid(X_ik, Y2_ik)) total_ways++;
                }

                long long X_jk = ux_ji * vx_jk + uy_ji * vy_jk;
                if (X_jk > 0) {
                    long long Y2_jk = u2_ji * v2_jk;
                    if (check_valid(X_jk, Y2_jk)) total_ways++;
                }

                long long ux_ki = -vx_ik;
                long long uy_ki = -vy_ik;
                long long u2_ki = v2_ik;

                long long vx_kj = -vx_jk;
                long long vy_kj = -vy_jk;
                long long v2_kj = v2_jk;

                long long X_ki = ux_ki * vx_kj + uy_ki * vy_kj;
                if (X_ki > 0) {
                    long long Y2_ki = u2_ki * v2_kj;
                    if (check_valid(X_ki, Y2_ki)) total_ways++;
                }
            }
        }
    }

    cout << total_ways << "\n";
    return 0;
}
