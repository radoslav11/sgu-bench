#pragma GCC optimize("O3,unroll-loops")
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

double evaluate(const vector<double>& Z, double th_A, double th_B, double z_A, double z_B, int N) {
    double total_dist = 0;
    double dth = (th_B - th_A) / N;
    for (int i = 0; i < N; ++i) {
        double th1 = th_A + dth * i;
        double th2 = th_A + dth * (i + 1);
        double z1 = (i == 0) ? z_A : Z[i - 1];
        double z2 = (i == N - 1) ? z_B : Z[i];
        
        double r1 = sqrt(1.0 + z1 * z1);
        double x1 = r1 * cos(th1);
        double y1 = r1 * sin(th1);
        
        double r2 = sqrt(1.0 + z2 * z2);
        double x2 = r2 * cos(th2);
        double y2 = r2 * sin(th2);
        
        double dx = x1 - x2;
        double dy = y1 - y2;
        double dz = z1 - z2;
        total_dist += sqrt(dx*dx + dy*dy + dz*dz);
    }
    return total_dist;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    double xA, yA, zA, xB, yB, zB;
    if (!(cin >> xA >> yA >> zA >> xB >> yB >> zB)) return 0;

    double rA = sqrt(xA * xA + yA * yA);
    double rB = sqrt(xB * xB + yB * yB);
    
    double cos_th = (xA * xB + yA * yB) / (rA * rB);
    if (cos_th > 1.0) cos_th = 1.0;
    if (cos_th < -1.0) cos_th = -1.0;
    double delta_th = acos(cos_th);

    int N = 100;
    vector<double> Z(N - 1);
    for (int i = 0; i < N - 1; ++i) {
        Z[i] = zA + (zB - zA) * (i + 1.0) / N;
    }

    double best_f = evaluate(Z, 0, delta_th, zA, zB, N);
    double lr = 0.1;

    for (int iter = 0; iter < 1000; ++iter) {
        vector<double> grad(N - 1, 0.0);
        double scale = max({1e-5, delta_th / 20.0, std::abs(zA - zB) / 20.0});
        double eps = scale * 1e-3;
        if (eps < 1e-7) eps = 1e-7;
        if (eps > 1e-3) eps = 1e-3;
        
        double max_grad = 0;
        for (int i = 0; i < N - 1; ++i) {
            double orig = Z[i];
            Z[i] = orig + eps;
            double f_plus = evaluate(Z, 0, delta_th, zA, zB, N);
            Z[i] = orig - eps;
            double f_minus = evaluate(Z, 0, delta_th, zA, zB, N);
            Z[i] = orig;
            grad[i] = (f_plus - f_minus) / (2 * eps);
            max_grad = max(max_grad, std::abs(grad[i]));
        }
        
        if (max_grad < 1e-7) break;
        
        for (int i = 0; i < N - 1; ++i) {
            grad[i] /= max_grad;
        }
        
        vector<double> new_Z = Z;
        for (int i = 0; i < N - 1; ++i) {
            new_Z[i] = Z[i] - lr * grad[i];
            new_Z[i] = max(-2.0, min(2.0, new_Z[i]));
        }
        
        double new_f = evaluate(new_Z, 0, delta_th, zA, zB, N);
        if (new_f < best_f) {
            best_f = new_f;
            Z = new_Z;
            lr *= 1.2;
            if (lr > 1.0) lr = 1.0;
        } else {
            lr *= 0.5;
            if (lr < 1e-7) break;
        }
    }

    cout << fixed << setprecision(10) << best_f << "\n";

    return 0;
}
