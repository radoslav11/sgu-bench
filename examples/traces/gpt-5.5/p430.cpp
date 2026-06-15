#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<vector<int>> adj(n, vector<int>(n, 0));
    vector<pair<int,int>> edges;
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u][v] = adj[v][u] = 1;
        edges.push_back({u, v});
    }

    if (n == 1) {
        cout << "Yes\n0 0\n";
        return 0;
    }

    if (m == 0) {
        cout << "Yes\n";
        for (int i = 0; i < n; ++i) cout << 2.0 * i << " 0\n";
        return 0;
    }

    auto verify = [&](const vector<double>& x, const vector<double>& y) -> bool {
        for (int i = 0; i < n; ++i) {
            if (fabs(x[i]) > 100 || fabs(y[i]) > 100) return false;
        }

        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                double dx = x[i] - x[j];
                double dy = y[i] - y[j];
                double d = sqrt(dx * dx + dy * dy);

                if (d < 1e-2) return false;

                if (adj[i][j]) {
                    if (fabs(d - 1.0) > 1e-7) return false;
                } else {
                    if (fabs(d - 1.0) < 1e-2) return false;
                }
            }
        }
        return true;
    };

    mt19937 rng((unsigned)chrono::steady_clock::now().time_since_epoch().count());
    uniform_real_distribution<double> rnd(-2.0, 2.0);

    vector<double> bestX(n), bestY(n);

    auto startTime = chrono::steady_clock::now();

    const double TIME_LIMIT = 0.70;

    int attempts = 0;

    while (true) {
        double elapsed = chrono::duration<double>(chrono::steady_clock::now() - startTime).count();
        if (elapsed > TIME_LIMIT) break;

        attempts++;

        vector<double> x(n), y(n);
        for (int i = 0; i < n; ++i) {
            x[i] = rnd(rng);
            y[i] = rnd(rng);
        }

        vector<double> mx(n, 0), my(n, 0), vx(n, 0), vy(n, 0);

        const int ITER = 4500;
        const double beta1 = 0.9;
        const double beta2 = 0.999;
        const double eps = 1e-8;

        for (int it = 1; it <= ITER; ++it) {
            vector<double> gx(n, 0), gy(n, 0);

            for (int i = 0; i < n; ++i) {
                for (int j = i + 1; j < n; ++j) {
                    double dx = x[i] - x[j];
                    double dy = y[i] - y[j];
                    double r2 = dx * dx + dy * dy;
                    double r = sqrt(max(r2, 1e-20));

                    if (adj[i][j]) {
                        double diff = r - 1.0;
                        double coef = 24.0 * diff / r;
                        gx[i] += coef * dx;
                        gy[i] += coef * dy;
                        gx[j] -= coef * dx;
                        gy[j] -= coef * dy;
                    } else {
                        double margin = 0.075;

                        if (fabs(r - 1.0) < margin) {
                            double s = (r >= 1.0 ? 1.0 : -1.0);
                            double diff = margin - fabs(r - 1.0);
                            double coef = -2.0 * diff * s / r;
                            gx[i] += coef * dx;
                            gy[i] += coef * dy;
                            gx[j] -= coef * dx;
                            gy[j] -= coef * dy;
                        }

                        if (r < 0.08) {
                            double diff = 0.08 - r;
                            double coef = -4.0 * diff / r;
                            gx[i] += coef * dx;
                            gy[i] += coef * dy;
                            gx[j] -= coef * dx;
                            gy[j] -= coef * dy;
                        }
                    }
                }
            }

            double lr = 0.035;
            if (it > 1000) lr = 0.015;
            if (it > 2500) lr = 0.006;
            if (it > 3700) lr = 0.002;

            for (int i = 0; i < n; ++i) {
                mx[i] = beta1 * mx[i] + (1 - beta1) * gx[i];
                my[i] = beta1 * my[i] + (1 - beta1) * gy[i];
                vx[i] = beta2 * vx[i] + (1 - beta2) * gx[i] * gx[i];
                vy[i] = beta2 * vy[i] + (1 - beta2) * gy[i] * gy[i];

                double mhx = mx[i] / (1 - pow(beta1, it));
                double mhy = my[i] / (1 - pow(beta1, it));
                double vhx = vx[i] / (1 - pow(beta2, it));
                double vhy = vy[i] / (1 - pow(beta2, it));

                x[i] -= lr * mhx / (sqrt(vhx) + eps);
                y[i] -= lr * mhy / (sqrt(vhy) + eps);
            }

            if (it % 50 == 0) {
                double cx = 0, cy = 0;
                for (int i = 0; i < n; ++i) {
                    cx += x[i];
                    cy += y[i];
                }
                cx /= n;
                cy /= n;
                for (int i = 0; i < n; ++i) {
                    x[i] -= cx;
                    y[i] -= cy;
                }
            }

            if (it % 100 == 0 || it == ITER) {
                if (verify(x, y)) {
                    cout << "Yes\n";
                    cout << fixed << setprecision(15);
                    for (int i = 0; i < n; ++i) {
                        cout << x[i] << ' ' << y[i] << '\n';
                    }
                    return 0;
                }
            }
        }
    }

    cout << "No\n";
    return 0;
}
