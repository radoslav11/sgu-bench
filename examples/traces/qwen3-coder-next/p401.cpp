#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdio>

int main() {
    int N;
    double u, t_val;
    std::cin >> N >> u >> t_val;

    std::vector<int> w(N), v(N);
    for (int i = 0; i < N; i++) {
        std::cin >> w[i] >> v[i];
    }

    double total_min = 0.0;
    for (int i = 0; i < N; i++) {
        total_min += static_cast<double>(w[i]) / u;
    }

    if (t_val < total_min - 1e-12) {
        std::cout << -1 << std::endl;
        return 0;
    }

    int X = 0;
    for (int i = 0; i < N; i++) {
        X += w[i];
    }

    int max_v = *std::max_element(v.begin(), v.end());
    int min_v = *std::min_element(v.begin(), v.end());

    double low1 = static_cast<double>(max_v) + u + 1e-12;
    double high1 = 1e15;
    double lambda = 0.0;
    for (int iter = 0; iter < 200; iter++) {
        lambda = (low1 + high1) * 0.5;
        double total_time = 0.0;
        for (int i = 0; i < N; i++) {
            double diff = lambda - v[i];
            double term = std::sqrt(diff * diff - u * u);
            total_time += static_cast<double>(w[i]) * diff / (u * term);
        }
        if (total_time > t_val) {
            low1 = lambda;
        } else {
            high1 = lambda;
        }
    }
    lambda = (low1 + high1) * 0.5;

    std::vector<double> t1(N);
    double R = 0.0;
    for (int i = 0; i < N; i++) {
        double diff = lambda - v[i];
        double term = std::sqrt(diff * diff - u * u);
        t1[i] = static_cast<double>(w[i]) * diff / (u * term);
        double y_i = v[i] * t1[i] + std::sqrt(u * u * t1[i] * t1[i] - static_cast<double>(w[i]) * w[i]);
        R += y_i;
    }

    double low2 = -1e15;
    double high2 = static_cast<double>(min_v) - u - 1e-12;
    double mu = 0.0;
    for (int iter = 0; iter < 200; iter++) {
        mu = (low2 + high2) * 0.5;
        double total_time = 0.0;
        for (int i = 0; i < N; i++) {
            double diff = v[i] - mu;
            double term = std::sqrt(diff * diff - u * u);
            total_time += static_cast<double>(w[i]) * diff / (u * term);
        }
        if (total_time > t_val) {
            high2 = mu;
        } else {
            low2 = mu;
        }
    }
    mu = (low2 + high2) * 0.5;

    std::vector<double> t2(N);
    double L = 0.0;
    for (int i = 0; i < N; i++) {
        double diff = v[i] - mu;
        double term = std::sqrt(diff * diff - u * u);
        t2[i] = static_cast<double>(w[i]) * diff / (u * term);
        double y_i = v[i] * t2[i] - std::sqrt(u * u * t2[i] * t2[i] - static_cast<double>(w[i]) * w[i]);
        L += y_i;
    }

    double d1 = std::sqrt(static_cast<double>(X) * X + R * R);
    double d2 = std::sqrt(static_cast<double>(X) * X + L * L);

    if (d1 >= d2) {
        std::printf("%.10f\n", d1);
        for (int i = 0; i < N; i++) {
            if (i > 0) std::printf(" ");
            std::printf("%.10f", t1[i]);
        }
        std::printf("\n");
    } else {
        std::printf("%.10f\n", d2);
        for (int i = 0; i < N; i++) {
            if (i > 0) std::printf(" ");
            std::printf("%.10f", t2[i]);
        }
        std::printf("\n");
    }

    return 0;
}
