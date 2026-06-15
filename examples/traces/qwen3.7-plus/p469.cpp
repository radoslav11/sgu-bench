#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <tuple>

using namespace std;

double W, H, D;
int N;
vector<double> X, Y, Z, R;

inline double f(double x, double y, double z) {
    double res = x;
    if (W - x < res) res = W - x;
    if (y < res) res = y;
    if (H - y < res) res = H - y;
    if (z < res) res = z;
    if (D - z < res) res = D - z;
    for (int i = 0; i < N; ++i) {
        double dx = x - X[i];
        double dy = y - Y[i];
        double dz = z - Z[i];
        double d = sqrt(dx*dx + dy*dy + dz*dz) - R[i];
        if (d < res) res = d;
    }
    return res;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> W >> H >> D)) return 0;
    cin >> N;
    X.resize(N); Y.resize(N); Z.resize(N); R.resize(N);
    for (int i = 0; i < N; ++i) {
        cin >> X[i] >> Y[i] >> Z[i] >> R[i];
    }

    mt19937 rng(1337);
    uniform_real_distribution<double> dist(0.0, 1.0);

    double best_x = W / 2.0, best_y = H / 2.0, best_z = D / 2.0;
    double best_f = f(best_x, best_y, best_z);

    vector<tuple<double, double, double>> starts;
    starts.push_back({W / 2.0, H / 2.0, D / 2.0});
    
    for (int i = 0; i < N; ++i) {
        starts.push_back({max(0.0, min(W, X[i])), max(0.0, min(H, Y[i])), max(0.0, min(D, Z[i]))});
    }
    
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            starts.push_back({
                max(0.0, min(W, (X[i] + X[j]) / 2.0)),
                max(0.0, min(H, (Y[i] + Y[j]) / 2.0)),
                max(0.0, min(D, (Z[i] + Z[j]) / 2.0))
            });
        }
    }

    for (int i = 0; i < 2000; ++i) {
        starts.push_back({dist(rng) * W, dist(rng) * H, dist(rng) * D});
    }

    double max_dim = max({W, H, D});
    double final_temp = 1e-5;
    double cooling_rate = pow(final_temp / max(1.0, max_dim), 1.0 / 2000.0);

    for (auto& start : starts) {
        double x = get<0>(start);
        double y = get<1>(start);
        double z = get<2>(start);
        double cur_f = f(x, y, z);
        double temp = max_dim;

        for (int iter = 0; iter < 2000; ++iter) {
            double nx = x + temp * (dist(rng) * 2.0 - 1.0);
            double ny = y + temp * (dist(rng) * 2.0 - 1.0);
            double nz = z + temp * (dist(rng) * 2.0 - 1.0);

            nx = max(0.0, min(W, nx));
            ny = max(0.0, min(H, ny));
            nz = max(0.0, min(D, nz));

            double nf = f(nx, ny, nz);
            if (nf > cur_f || exp((nf - cur_f) / max(1e-9, temp)) > dist(rng)) {
                x = nx; y = ny; z = nz;
                cur_f = nf;
                if (cur_f > best_f) {
                    best_f = cur_f;
                    best_x = x; best_y = y; best_z = z;
                }
            }
            temp *= cooling_rate;
        }
    }

    double x = best_x, y = best_y, z = best_z;
    double cur_f = f(x, y, z);
    double step = max_dim / 10.0;

    while (step > 1e-6) {
        bool improved = false;
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dz = -1; dz <= 1; ++dz) {
                    if (dx == 0 && dy == 0 && dz == 0) continue;
                    double nx = x + dx * step;
                    double ny = y + dy * step;
                    double nz = z + dz * step;
                    if (nx >= 0 && nx <= W && ny >= 0 && ny <= H && nz >= 0 && nz <= D) {
                        double nf = f(nx, ny, nz);
                        if (nf > cur_f + step * 1e-4) {
                            x = nx; y = ny; z = nz;
                            cur_f = nf;
                            improved = true;
                        }
                    }
                }
            }
        }
        if (!improved) {
            step /= 2.0;
        }
    }

    cout.precision(10);
    cout << fixed << x << " " << y << " " << z << "\n";

    return 0;
}
