#include <bits/stdc++.h>
using namespace std;

const double LOW = 0.01;
const double EPS = 1e-10;

struct Node {
    double val[2];
    double win[2];
};

double bribeCost(double p, double lambda) {
    return -log((1.0 - p) / 0.99) / lambda;
}

double br(double D, double lambda) {
    double T = 1.0 / (lambda * 0.99);
    if (D <= T) return LOW;
    return 1.0 - 1.0 / (lambda * D);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, V;
    double F1, F2;
    if (!(cin >> N >> F1 >> F2 >> V)) return 0;

    double L1 = -log(1.0 - F1);
    double L2 = -log(1.0 - F2);

    vector<vector<Node>> dp(N + 1, vector<Node>(N + 1));

    for (int a = N; a >= 0; --a) {
        for (int b = N; b >= 0; --b) {
            if (a == N && b == N) {
                dp[a][b].val[0] = dp[a][b].val[1] = V / 2.0;
                dp[a][b].win[0] = dp[a][b].win[1] = 0.5;
            } else if (a == N) {
                dp[a][b].val[0] = V;
                dp[a][b].val[1] = 0;
                dp[a][b].win[0] = 1;
                dp[a][b].win[1] = 0;
            } else if (b == N) {
                dp[a][b].val[0] = 0;
                dp[a][b].val[1] = V;
                dp[a][b].win[0] = 0;
                dp[a][b].win[1] = 1;
            }
        }
    }

    for (int sum = 2 * N - 2; sum >= 0; --sum) {
        for (int a = min(N - 1, sum); a >= 0; --a) {
            int b = sum - a;
            if (b < 0 || b >= N) continue;

            double A00 = dp[a][b].val[0];
            double A10 = dp[a + 1][b].val[0];
            double A01 = dp[a][b + 1].val[0];
            double A11 = dp[a + 1][b + 1].val[0];

            double B00 = dp[a][b].val[1];
            double B10 = dp[a + 1][b].val[1];
            double B01 = dp[a][b + 1].val[1];
            double B11 = dp[a + 1][b + 1].val[1];

            double c10 = A10 - A00;
            double c11 = A11 - A01;
            double d10 = B01 - B00;
            double d11 = B11 - B10;

            auto D1 = [&](double q) {
                return c10 * (1.0 - q) + c11 * q;
            };
            auto D2 = [&](double p) {
                return d10 * (1.0 - p) + d11 * p;
            };

            double p = LOW, q = LOW;
            bool found = false;

            auto check = [&](double pp, double qq) {
                double bp = br(D1(qq), L1);
                double bq = br(D2(pp), L2);
                return fabs(pp - bp) < 1e-8 && fabs(qq - bq) < 1e-8;
            };

            vector<pair<double,double>> cand;

            // both at lower bound
            cand.push_back({LOW, LOW});

            // p interior, q lower
            {
                double pp = br(D1(LOW), L1);
                cand.push_back({pp, LOW});
            }

            // p lower, q interior
            {
                double qq = br(D2(LOW), L2);
                cand.push_back({LOW, qq});
            }

            // both interior: Newton from several starting points
            vector<double> starts = {0.02, 0.1, 0.3, 0.5, 0.7, 0.9, 0.98};
            double m1 = c11 - c10;
            double m2 = d11 - d10;

            for (double sp : starts) {
                for (double sq : starts) {
                    double pp = sp, qq = sq;
                    bool ok = true;

                    for (int it = 0; it < 100; ++it) {
                        double x = D1(qq);
                        double y = D2(pp);

                        if (x <= 0 || y <= 0) {
                            ok = false;
                            break;
                        }

                        double f1 = pp - 1.0 + 1.0 / (L1 * x);
                        double f2 = qq - 1.0 + 1.0 / (L2 * y);

                        if (fabs(f1) + fabs(f2) < 1e-12) break;

                        double j12 = -m1 / (L1 * x * x);
                        double j21 = -m2 / (L2 * y * y);
                        double det = 1.0 - j12 * j21;

                        if (fabs(det) < 1e-14) {
                            ok = false;
                            break;
                        }

                        double dpv = (-f1 + j12 * f2) / det;
                        double dqv = (j21 * f1 - f2) / det;

                        double step = 1.0;
                        bool moved = false;
                        for (int ls = 0; ls < 30; ++ls) {
                            double np = pp + step * dpv;
                            double nq = qq + step * dqv;
                            if (np > LOW && np < 1.0 && nq > LOW && nq < 1.0) {
                                pp = np;
                                qq = nq;
                                moved = true;
                                break;
                            }
                            step *= 0.5;
                        }
                        if (!moved) {
                            ok = false;
                            break;
                        }
                    }

                    if (ok) cand.push_back({pp, qq});
                }
            }

            for (auto [pp, qq] : cand) {
                pp = min(max(pp, LOW), 1.0 - 1e-12);
                qq = min(max(qq, LOW), 1.0 - 1e-12);
                if (check(pp, qq)) {
                    p = pp;
                    q = qq;
                    found = true;
                    break;
                }
            }

            // Safe fallback: damped best-response iteration.
            if (!found) {
                p = q = 0.5;
                for (int it = 0; it < 100000; ++it) {
                    double np = br(D1(q), L1);
                    double nq = br(D2(p), L2);
                    p = 0.5 * p + 0.5 * np;
                    q = 0.5 * q + 0.5 * nq;
                }
                p = br(D1(q), L1);
                q = br(D2(p), L2);
            }

            double prob00 = (1.0 - p) * (1.0 - q);
            double prob10 = p * (1.0 - q);
            double prob01 = (1.0 - p) * q;
            double prob11 = p * q;

            dp[a][b].val[0] =
                -bribeCost(p, L1)
                + prob00 * dp[a][b].val[0]
                + prob10 * dp[a + 1][b].val[0]
                + prob01 * dp[a][b + 1].val[0]
                + prob11 * dp[a + 1][b + 1].val[0];

            dp[a][b].val[1] =
                -bribeCost(q, L2)
                + prob00 * dp[a][b].val[1]
                + prob10 * dp[a + 1][b].val[1]
                + prob01 * dp[a][b + 1].val[1]
                + prob11 * dp[a + 1][b + 1].val[1];

            dp[a][b].win[0] =
                prob00 * dp[a][b].win[0]
                + prob10 * dp[a + 1][b].win[0]
                + prob01 * dp[a][b + 1].win[0]
                + prob11 * dp[a + 1][b + 1].win[0];

            dp[a][b].win[1] =
                prob00 * dp[a][b].win[1]
                + prob10 * dp[a + 1][b].win[1]
                + prob01 * dp[a][b + 1].win[1]
                + prob11 * dp[a + 1][b + 1].win[1];
        }
    }

    cout.setf(ios::fixed);
    cout << setprecision(9) << dp[0][0].win[0] << " " << dp[0][0].win[1] << "\n";

    return 0;
}
