#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;

    const double PI = acos(-1.0);

    double A[3][3] = {
        {1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}
    };
    double b[3] = {0.0, 0.0, 0.0};

    auto apply = [&](double L[3][3], double c[3]) {
        double nA[3][3]{};
        double nb[3]{};

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                for (int k = 0; k < 3; ++k) {
                    nA[i][j] += L[i][k] * A[k][j];
                }
            }
        }

        for (int i = 0; i < 3; ++i) {
            nb[i] = c[i];
            for (int k = 0; k < 3; ++k) {
                nb[i] += L[i][k] * b[k];
            }
        }

        memcpy(A, nA, sizeof(A));
        memcpy(b, nb, sizeof(b));
    };

    for (int spell = 0; spell < N; ++spell) {
        char type;
        cin >> type;

        double L[3][3] = {};
        double c[3] = {0.0, 0.0, 0.0};

        if (type == 'T') {
            cin >> c[0] >> c[1] >> c[2];

            for (int i = 0; i < 3; ++i) {
                L[i][i] = 1.0;
            }
        } else if (type == 'S') {
            double sx, sy, sz;
            cin >> sx >> sy >> sz;

            L[0][0] = sx;
            L[1][1] = sy;
            L[2][2] = sz;
        } else if (type == 'R') {
            double x, y, z, angle;
            cin >> x >> y >> z >> angle;

            double len = sqrt(x * x + y * y + z * z);
            x /= len;
            y /= len;
            z /= len;

            double rad = angle * PI / 180.0;
            double cs = cos(rad);
            double sn = sin(rad);
            double one = 1.0 - cs;

            L[0][0] = cs + x * x * one;
            L[0][1] = x * y * one - z * sn;
            L[0][2] = x * z * one + y * sn;

            L[1][0] = y * x * one + z * sn;
            L[1][1] = cs + y * y * one;
            L[1][2] = y * z * one - x * sn;

            L[2][0] = z * x * one - y * sn;
            L[2][1] = z * y * one + x * sn;
            L[2][2] = cs + z * z * one;
        }

        apply(L, c);
    }

    int M;
    cin >> M;

    cout.setf(ios::fixed);
    cout << setprecision(10);

    for (int i = 0; i < M; ++i) {
        double v[3];
        cin >> v[0] >> v[1] >> v[2];

        double res[3];
        for (int r = 0; r < 3; ++r) {
            res[r] = b[r];
            for (int k = 0; k < 3; ++k) {
                res[r] += A[r][k] * v[k];
            }
            if (fabs(res[r]) < 5e-12) res[r] = 0.0;
        }

        cout << res[0] << ' ' << res[1] << ' ' << res[2] << '\n';
    }

    return 0;
}
