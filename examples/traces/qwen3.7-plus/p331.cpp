#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

double a_arr[10], b_arr[10], delta_arr[10];

double v(int lane, double t) {
    return b_arr[lane] + a_arr[lane] * sin(t + delta_arr[lane]);
}

double dv(int lane, double t) {
    return a_arr[lane] * cos(t + delta_arr[lane]);
}

double V(int lane, double t) {
    return b_arr[lane] * t - a_arr[lane] * cos(t + delta_arr[lane]);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    double d, c;
    if (!(cin >> N >> d >> c)) return 0;

    for (int i = 1; i <= N; ++i) {
        cin >> a_arr[i] >> b_arr[i] >> delta_arr[i];
    }

    double dt = 1e-4;
    double T_max = 80.0;
    int K = round(T_max / dt);

    vector<double> dp((N + 1) * (K + 1), 0.0);
    vector<short> action((N + 1) * (K + 1), 0);

    auto get_dp = [&](int i, int k) { return dp[i * (K + 1) + k]; };
    auto set_dp = [&](int i, int k, double val) { dp[i * (K + 1) + k] = val; };
    auto get_act = [&](int i, int k) { return action[i * (K + 1) + k]; };
    auto set_act = [&](int i, int k, short val) { action[i * (K + 1) + k] = val; };

    int sw_steps[10][10];
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N; ++j) {
            sw_steps[i][j] = round(c * abs(i - j) / dt);
        }
    }

    for (int k = K - 1; k >= 0; --k) {
        double t = k * dt;
        for (int i = 1; i <= N; ++i) {
            double val = get_dp(i, k + 1) + v(i, t) * dt;
            short best_act = i;
            for (int j = 1; j <= N; ++j) {
                if (i == j) continue;
                int idx = k + sw_steps[i][j];
                if (idx <= K && get_dp(j, idx) > val) {
                    val = get_dp(j, idx);
                    best_act = j;
                }
            }
            set_dp(i, k, val);
            set_act(i, k, best_act);
        }
    }

    struct Segment {
        int lane;
        double start_t;
        double end_t;
    };
    vector<Segment> path;
    int curr_i = 1;
    int curr_k = 0;

    while (curr_k < K) {
        int next_k = curr_k;
        while (next_k < K && get_act(curr_i, next_k) == curr_i) {
            next_k++;
        }

        if (next_k > curr_k) {
            if (!path.empty() && path.back().lane == curr_i) {
                path.back().end_t = next_k * dt;
            } else {
                path.push_back({curr_i, curr_k * dt, next_k * dt});
            }
        }

        if (next_k == K) break;

        int next_lane = get_act(curr_i, next_k);
        int from_lane = curr_i;
        curr_i = next_lane;
        curr_k = next_k + sw_steps[from_lane][next_lane];
    }

    if (!path.empty() && path.back().end_t == 0.0) {
        path.back().end_t = K * dt;
    }

    vector<int> f_S;
    vector<double> f_x;

    for (size_t i = 0; i < path.size(); ++i) {
        f_S.push_back(path[i].lane);
        if (i < path.size() - 1) {
            f_x.push_back(path[i].end_t);
        } else {
            f_x.push_back(path[i].end_t);
        }
    }

    double current_dist = 0;
    for (size_t i = 0; i < f_S.size(); ++i) {
        double start_t = (i == 0) ? 0.0 : f_x[i - 1] + c * abs(f_S[i - 1] - f_S[i]);
        current_dist += V(f_S[i], f_x[i]) - V(f_S[i], start_t);
    }

    if (current_dist < d) {
        int P = 1;
        int n = f_S.size();
        if (n >= 2) {
            while (P <= n / 2) {
                bool ok = true;
                for (int i = 0; i < P; ++i) {
                    if (f_S[n - 1 - i] != f_S[n - 1 - i - P]) {
                        ok = false;
                        break;
                    }
                }
                if (ok) break;
                P++;
            }
        }

        double dt_cycle = f_x.back() - f_x[n - 1 - P];
        double cycle_dist = 0;
        int start_idx = n - P;
        for (int i = start_idx; i < n; ++i) {
            double st = (i == 0) ? 0.0 : f_x[i - 1] + c * abs(f_S[i - 1] - f_S[i]);
            cycle_dist += V(f_S[i], f_x[i]) - V(f_S[i], st);
        }

        if (cycle_dist <= 1e-9) cycle_dist = 1e-9;
        int add_cycles = (d - current_dist) / cycle_dist + 3;
        for (int c_idx = 0; c_idx < add_cycles; ++c_idx) {
            for (int j = 0; j < P; ++j) {
                f_S.push_back(f_S[n - P + j]);
                f_x.push_back(f_x.back() + dt_cycle);
            }
        }
    }

    int m = f_S.size() - 1;
    vector<double> x = f_x;

    for (int iter = 0; iter < 50; ++iter) {
        vector<double> F(m + 1, 0.0);
        vector<double> J(m + 1, 0.0);

        for (int k = 0; k < m; ++k) {
            double end_k = x[k];
            double start_k_plus_1 = end_k + c * abs(f_S[k] - f_S[k + 1]);

            double v_curr = v(f_S[k], end_k);
            double v_next = v(f_S[k + 1], start_k_plus_1);

            F[k] = v_curr - v_next;

            double dv_curr = dv(f_S[k], end_k);
            double dv_next = dv(f_S[k + 1], start_k_plus_1);

            J[k] = dv_curr - dv_next;
            if (J[k] > -1e-6) {
                J[k] = -1e-6;
            }
        }

        double total_D = 0.0;
        for (int k = 0; k <= m; ++k) {
            double start_t = (k == 0) ? 0.0 : x[k - 1] + c * abs(f_S[k - 1] - f_S[k]);
            total_D += V(f_S[k], x[k]) - V(f_S[k], start_t);
        }
        F[m] = total_D - d;

        vector<double> dx(m + 1, 0.0);
        for (int k = 0; k < m; ++k) {
            dx[k] = -F[k] / J[k];
        }

        double sum_F_dx = 0.0;
        for (int k = 0; k < m; ++k) {
            sum_F_dx += F[k] * dx[k];
        }

        double v_last = v(f_S[m], x[m]);
        if (abs(v_last) < 1e-9) v_last = 1e-9;

        dx[m] = (-F[m] - sum_F_dx) / v_last;

        double alpha = 1.0;
        for (int damp = 0; damp < 20; ++damp) {
            bool ok = true;
            for (int k = 0; k <= m; ++k) {
                double next_x = x[k] + alpha * dx[k];
                if (k > 0) {
                    double min_x = x[k - 1] + c * abs(f_S[k - 1] - f_S[k]);
                    if (next_x <= min_x + 1e-11) {
                        ok = false;
                        break;
                    }
                }
                if (next_x < 0) {
                    ok = false;
                    break;
                }
            }
            if (ok) break;
            alpha *= 0.5;
        }

        double max_dx = 0.0;
        for (int k = 0; k <= m; ++k) {
            x[k] += alpha * dx[k];
            max_dx = max(max_dx, abs(alpha * dx[k]));
        }

        if (max_dx < 1e-12) break;
    }

    cout << fixed << setprecision(15);
    cout << x[m] << "\n";
    cout << m << "\n";
    for (int k = 0; k < m; ++k) {
        cout << f_S[k + 1] << " " << x[k] << "\n";
    }

    return 0;
}
