#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

/**
 * Problem: Orthogonal Circles
 * Two circles are orthogonal if d^2 = r1^2 + r2^2, where d is the distance between centers.
 * For a target circle (X, Y, R) to be orthogonal to all given circles (xi, yi, ri):
 * (X - xi)^2 + (Y - yi)^2 = R^2 + ri^2
 * X^2 - 2X*xi + xi^2 + Y^2 - 2Y*yi + yi^2 = R^2 + ri^2
 * (X^2 + Y^2 - R^2) - 2X*xi - 2Y*yi + xi^2 + yi^2 - ri^2 = 0
 * Let K = X^2 + Y^2 - R^2.
 * Then 2*xi*X + 2*yi*Y - K = xi^2 + yi^2 - ri^2.
 * 
 * This is a system of linear equations: A_i*X + B_i*Y + C_i*K = D_i
 * where A_i = 2*xi, B_i = 2*yi, C_i = -1, and D_i = xi^2 + yi^2 - ri^2.
 * 
 * We solve this system for (X, Y, K) using Gaussian elimination on a 3x4 matrix.
 * After solving, we check R^2 = X^2 + Y^2 - K.
 * If R^2 > 0, we have a circle.
 */

typedef long double ld;

struct Row {
    ld v[4];
};

vector<ld> solve_system(int r_count, ld mat[3][4], int p[3], const vector<int>& free_vars, const vector<ld>& free_vals) {
    vector<ld> vars(3, 0.0L);
    for (int i = 0; i < (int)free_vars.size(); ++i) {
        vars[free_vars[i]] = free_vals[i];
    }
    for (int i = r_count - 1; i >= 0; --i) {
        ld sum = mat[i][3];
        for (int j = i + 1; j < r_count; ++j) {
            sum -= mat[i][p[j]] * vars[p[j]];
        }
        for (int f : free_vars) {
            sum -= mat[i][f] * vars[f];
        }
        vars[p[i]] = sum / mat[i][p[i]];
    }
    return vars;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    ld mat[3][4];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) mat[i][j] = 0.0L;
    }
    int r_count = 0;

    for (int i = 0; i < n; ++i) {
        ld xi, yi, ri;
        cin >> xi >> yi >> ri;
        ld cur[4] = {2.0L * xi, 2.0L * yi, -1.0L, xi * xi + yi * yi - ri * ri};

        for (int r = 0; r < r_count; ++r) {
            int pivot = -1;
            for (int c = 0; c < 3; ++c) {
                if (fabsl(mat[r][c]) > 1e-9L) { pivot = c; break; }
            }
            if (pivot != -1 && fabsl(cur[pivot]) > 1e-9L) {
                ld factor = cur[pivot] / mat[r][pivot];
                for (int c = 0; c < 4; ++c) cur[c] -= factor * mat[r][c];
            }
        }

        int pivot = -1;
        for (int c = 0; c < 3; ++c) {
            if (fabsl(cur[c]) > 1e-9L) { pivot = c; break; }
        }

        if (pivot != -1) {
            for (int c = 0; c < 4; ++c) mat[r_count][c] = cur[c];
            r_count++;
            for (int r1 = 0; r1 < r_count - 1; ++r1) {
                for (int r2 = r1 + 1; r2 < r_count; ++r2) {
                    int p1 = -1, p2 = -1;
                    for (int c = 0; c < 3; ++c) if (fabsl(mat[r1][c]) > 1e-9L) { p1 = c; break; }
                    for (int c = 0; c < 3; ++c) if (fabsl(mat[r2][c]) > 1e-9L) { p2 = c; break; }
                    if (p1 > p2) {
                        for (int c = 0; c < 4; ++c) swap(mat[r1][c], mat[r2][c]);
                    }
                }
            }
            for (int r1 = 0; r1 < r_count; ++r1) {
                for (int r2 = r1 + 1; r2 < r_count; ++r2) {
                    int p1 = -1, p2 = -1;
                    for (int c = 0; c < 3; ++c) if (fabsl(mat[r1][c]) > 1e-9L) { p1 = c; break; }
                    for (int c = 0; c < 3; ++c) if (fabsl(mat[r2][c]) > 1e-9L) { p2 = c; break; }
                    if (p1 != -1 && p2 != -1 && p1 == p2) {
                        ld factor = mat[r2][p1] / mat[r1][p1];
                        for (int c = 0; c < 4; ++c) mat[r2][c] -= factor * mat[r1][c];
                    }
                }
            }
        } else {
            if (fabsl(cur[3]) > 1e-9L) {
                cout << -1 << endl;
                return 0;
            }
        }
    }

    int p[3] = {-1, -1, -1};
    vector<int> pivot_cols;
    vector<int> free_cols;
    for (int c = 0; c < 3; ++c) {
        bool is_pivot = false;
        for (int r = 0; r < r_count; ++r) {
            int pc = -1;
            for (int col = 0; col < 3; ++col) {
                if (fabsl(mat[r][col]) > 1e-9L) { pc = col; break; }
            }
            if (pc == c) {
                is_pivot = true;
                break;
            }
        }
        if (is_pivot) pivot_cols.push_back(c);
        else free_cols.push_back(c);
    }

    for (int i = 0; i < r_count; ++i) {
        for (int c = 0; c < 3; ++c) {
            if (fabsl(mat[i][c]) > 1e-9L) {
                p[i] = c;
                break;
            }
        }
    }

    if (r_count == 3) {
        vector<ld> vars(3, 0.0L);
        for (int i = 2; i >= 0; --i) {
            ld sum = mat[i][3];
            for (int j = i + 1; j < 3; ++j) sum -= mat[i][p[j]] * vars[p[j]];
            vars[p[i]] = sum / mat[i][p[i]];
        }
        ld X = vars[0], Y = vars[1], K = vars[2];
        ld R2 = X * X + Y * Y - K;
        if (R2 > 1e-9L) cout << fixed << setprecision(10) << (double)X << " " << (double)Y << " " << (double)sqrtl(R2) << endl;
        else cout << -1 << endl;
    } else if (r_count == 2) {
        vector<ld> v0 = solve_system(2, mat, p, free_cols, {0.0L});
        vector<ld> v1 = solve_system(2, mat, p, free_cols, {1.0L});
        ld vx = v1[0] - v0[0], vy = v1[1] - v0[1], vz = v1[2] - v0[2];
        ld X0 = v0[0], Y0 = v0[1], K0 = v0[2];
        ld a = vx * vx + vy * vy, b = 2.0L * X0 * vx + 2.0L * Y0 * vy - vz, c = X0 * X0 + Y0 * Y0 - K0;
        if (a > 1e-9L) cout << -2 << endl;
        else if (a < -1e-9L) {
            ld disc = b * b - 4.0L * a * c;
            if (disc > 1e-9L) cout << -2 << endl;
            else cout << -1 << endl;
        } else {
            if (fabsl(b) > 1e-9L || c > 1e-9L) cout << -2 << endl;
            else cout << -1 << endl;
        }
    } else {
        // r_count < 2 or r_count = 0
        cout << -2 << endl;
    }

    return 0;
}

