#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <algorithm>
using namespace std;

int main() {
    // Precompute binomial coefficients for n up to 50
    long long C[51][51] = {{0}};
    for (int i = 0; i <= 50; i++) {
        C[i][0] = 1;
        for (int j = 1; j <= i; j++) {
            C[i][j] = C[i-1][j-1] + C[i-1][j];
        }
    }

    int n, l1, l2, p_val;
    cin >> n >> l1 >> l2 >> p_val;

    double p = p_val / 100.0;

    // Compute unnormalized weights for k in [0, n]
    vector<long long> weights(n+1, 0);
    // k must be at least l1 and at most n - l2
    int low_k = l1;
    int high_k = n - l2;
    if (low_k <= high_k) {
        for (int k = low_k; k <= high_k; k++) {
            weights[k] = C[k][l1] * C[n - k][l2];
        }
    }

    double total_weight = 0.0;
    for (int k = 0; k <= n; k++) {
        total_weight += weights[k];
    }

    // Compute posterior probabilities
    vector<double> probs(n+1, 0.0);
    if (total_weight > 0) {
        for (int k = 0; k <= n; k++) {
            probs[k] = static_cast<double>(weights[k]) / total_weight;
        }
    }

    // Build prefix sum array
    vector<double> prefix(n+2, 0.0);
    for (int i = 0; i <= n; i++) {
        prefix[i+1] = prefix[i] + probs[i];
    }

    // Find the shortest interval [a, b] with probability >= p
    int best_a = 0;
    int best_b = n;
    int min_len = n;

    for (int a = 0; a <= n; a++) {
        for (int b = a; b <= n; b++) {
            double sum_prob = prefix[b+1] - prefix[a];
            // Check if sum_prob >= p, with tolerance for floating point errors
            if (sum_prob >= p - 1e-9) {
                int len = b - a;
                if (len < min_len || (len == min_len && a < best_a)) {
                    min_len = len;
                    best_a = a;
                    best_b = b;
                }
            }
        }
    }

    cout << best_a << " " << best_b << endl;

    return 0;
}
