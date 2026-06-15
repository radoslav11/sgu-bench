#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

using int128 = __int128_t;

int128 gcd(int128 a, int128 b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        a %= b;
        int128 temp = a;
        a = b;
        b = temp;
    }
    return a;
}

int128 det3(int128 m[3][3]) {
    return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
         - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])
         + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
}

struct ExactRow {
    int128 v[4];
    int pivot;
    int orig_idx;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<long long> x(n), y(n), r(n), w(n);
    for (int i = 0; i < n; ++i) {
        cin >> x[i] >> y[i] >> r[i];
        w[i] = r[i] * r[i] - x[i] * x[i] - y[i] * y[i];
    }

    vector<ExactRow> basis;

    for (int i = 0; i < n; ++i) {
        int128 row[4] = {x[i], y[i], 1, w[i]};
        
        for (auto& b : basis) {
            if (row[b.pivot] != 0) {
                int128 p = b.v[b.pivot];
                int128 v = row[b.pivot];
                int128 g = gcd(p, v);
                p /= g;
                v /= g;
                
                for (int j = b.pivot; j < 4; ++j) {
                    row[j] = row[j] * p - b.v[j] * v;
                }
                
                int128 row_gcd = 0;
                for (int j = b.pivot; j < 4; ++j) {
                    row_gcd = gcd(row_gcd, row[j]);
                }
                if (row_gcd > 1) {
                    for (int j = b.pivot; j < 4; ++j) {
                        row[j] /= row_gcd;
                    }
                }
            }
        }
        
        int p = -1;
        for (int j = 0; j < 4; ++j) {
            if (row[j] != 0) {
                p = j;
                break;
            }
        }
        
        if (p != -1) {
            if (row[p] < 0) {
                for (int j = p; j < 4; ++j) {
                    row[j] = -row[j];
                }
            }
            basis.push_back({{row[0], row[1], row[2], row[3]}, p, i});
        }
    }

    int rank_A = 0;
    int rank_AW = basis.size();
    
    for (const auto& b : basis) {
        if (b.pivot < 3) {
            rank_A++;
        }
    }

    if (rank_A < rank_AW) {
        cout << -1 << "\n";
    } else if (rank_A < 3) {
        cout << -2 << "\n";
    } else {
        int idx0 = basis[0].orig_idx;
        int idx1 = basis[1].orig_idx;
        int idx2 = basis[2].orig_idx;

        int128 A_mat[3][3];
        A_mat[0][0] = x[idx0]; A_mat[0][1] = y[idx0]; A_mat[0][2] = 1;
        A_mat[1][0] = x[idx1]; A_mat[1][1] = y[idx1]; A_mat[1][2] = 1;
        A_mat[2][0] = x[idx2]; A_mat[2][1] = y[idx2]; A_mat[2][2] = 1;

        int128 D = det3(A_mat);

        int128 Ax[3][3], Ay[3][3], Az[3][3];
        Ax[0][0] = w[idx0]; Ax[0][1] = y[idx0]; Ax[0][2] = 1;
        Ax[1][0] = w[idx1]; Ax[1][1] = y[idx1]; Ax[1][2] = 1;
        Ax[2][0] = w[idx2]; Ax[2][1] = y[idx2]; Ax[2][2] = 1;

        Ay[0][0] = x[idx0]; Ay[0][1] = w[idx0]; Ay[0][2] = 1;
        Ay[1][0] = x[idx1]; Ay[1][1] = w[idx1]; Ay[1][2] = 1;
        Ay[2][0] = x[idx2]; Ay[2][1] = w[idx2]; Ay[2][2] = 1;

        Az[0][0] = x[idx0]; Az[0][1] = y[idx0]; Az[0][2] = w[idx0];
        Az[1][0] = x[idx1]; Az[1][1] = y[idx1]; Az[1][2] = w[idx1];
        Az[2][0] = x[idx2]; Az[2][1] = y[idx2]; Az[2][2] = w[idx2];

        int128 DX = det3(Ax);
        int128 DY = det3(Ay);
        int128 DZ = det3(Az);

        long double X = (long double)DX / (long double)D;
        long double Y = (long double)DY / (long double)D;
        long double Z = (long double)DZ / (long double)D;

        long double r2 = (X * X + Y * Y) / 4.0 - Z;

        if (r2 <= 1e-11) {
            cout << -1 << "\n";
        } else {
            long double r_val = sqrt(r2);
            long double xc = -X / 2.0;
            long double yc = -Y / 2.0;

            if (abs(xc) < 1e-9) xc = 0.0;
            if (abs(yc) < 1e-9) yc = 0.0;
            if (abs(r_val) < 1e-9) r_val = 0.0;

            cout << fixed << setprecision(9) << xc << " " << yc << " " << r_val << "\n";
        }
    }

    return 0;
}
