#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

const double PI = acos(-1.0);

struct Point {
    double x, y;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    vector<Point> V(N);
    for (int i = 0; i < N; ++i) {
        cin >> V[i].x >> V[i].y;
    }
    V.push_back(V[0]);

    vector<double> phi(N + 1);
    phi[0] = atan2(V[0].y, V[0].x);
    for (int i = 1; i <= N; ++i) {
        double theta = atan2(V[i].y, V[i].x);
        phi[i] = theta;
        while (phi[i] - phi[i - 1] > PI) phi[i] -= 2.0 * PI;
        while (phi[i] - phi[i - 1] < -PI) phi[i] += 2.0 * PI;
    }

    vector<pair<double, double>> intervals;
    for (int i = 0; i < M; ++i) {
        Point A, B;
        cin >> A.x >> A.y >> B.x >> B.y;
        double L = atan2(A.y, A.x);
        double R = atan2(B.y, B.x);
        
        if (R < L - PI) R += 2.0 * PI;
        if (L < R - PI) L += 2.0 * PI;
        if (L > R) swap(L, R);

        double L_norm = fmod(L + 2.0 * PI, 2.0 * PI);
        if (L_norm < 0) L_norm += 2.0 * PI;
        double R_norm = L_norm + (R - L);

        if (R_norm < 2.0 * PI) {
            intervals.push_back({L_norm, R_norm});
        } else {
            intervals.push_back({L_norm, 2.0 * PI});
            intervals.push_back({0.0, R_norm - 2.0 * PI});
        }
    }

    sort(intervals.begin(), intervals.end());
    vector<pair<double, double>> merged;
    for (auto& iv : intervals) {
        if (merged.empty()) {
            merged.push_back(iv);
        } else {
            if (iv.first <= merged.back().second + 1e-9) {
                merged.back().second = max(merged.back().second, iv.second);
            } else {
                merged.push_back(iv);
            }
        }
    }

    double phi_0 = phi[0];
    vector<pair<double, double>> U;
    for (auto& iv : merged) {
        double S_j = iv.first;
        double E_j = iv.second;
        double S_prime = S_j;
        long long k = round((phi_0 - S_prime) / (2.0 * PI));
        S_prime += k * 2.0 * PI;
        
        while (S_prime < phi_0 - 1e-9) S_prime += 2.0 * PI;
        while (S_prime >= phi_0 + 2.0 * PI - 1e-9) S_prime -= 2.0 * PI;
        
        double E_prime = E_j + (S_prime - S_j);
        if (E_prime > phi_0 + 2.0 * PI) {
            U.push_back({S_prime, phi_0 + 2.0 * PI});
            U.push_back({phi_0, E_prime - 2.0 * PI});
        } else {
            U.push_back({S_prime, E_prime});
        }
    }

    sort(U.begin(), U.end());

    double total_shadow_length = 0.0;
    int p = 0;
    int U_size = U.size();
    
    for (int i = 0; i < N; ++i) {
        double A_i = phi[i];
        double B_i = phi[i + 1];

        while (p < U_size && U[p].second < A_i - 1e-9) {
            p++;
        }

        for (int q = p; q < U_size; ++q) {
            if (U[q].first > B_i + 1e-9) break;

            double alpha = max(A_i, U[q].first);
            double beta = min(B_i, U[q].second);

            if (beta > alpha + 1e-9) {
                Point A = V[i];
                Point B = V[i + 1];

                double a = B.y - A.y;
                double b = A.x - B.x;
                double c = B.x * A.y - B.y * A.x;

                double r_alpha = -c / (a * cos(alpha) + b * sin(alpha));
                double px_alpha = r_alpha * cos(alpha);
                double py_alpha = r_alpha * sin(alpha);

                double r_beta = -c / (a * cos(beta) + b * sin(beta));
                double px_beta = r_beta * cos(beta);
                double py_beta = r_beta * sin(beta);

                double dx = px_alpha - px_beta;
                double dy = py_alpha - py_beta;
                total_shadow_length += sqrt(dx * dx + dy * dy);
            }
        }
    }

    cout << fixed << setprecision(9) << total_shadow_length << "\n";

    return 0;
}
