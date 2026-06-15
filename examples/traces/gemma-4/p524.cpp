#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>

using namespace std;

/**
 * Problem Analysis:
 * Let the new coordinates be y_i = y_1 + (i-1)*d, where d > 0 and y_1 is the starting coordinate.
 * We want to minimize S = sum_{i=1}^{n} |y_i - x_i|.
 * Substituting y_i: S = sum_{i=1}^{n} |y_1 + (i-1)*d - x_i|.
 * Let x'_i = x_i - (i-1)*d.
 * Then S = sum_{i=1}^{n} |y_1 - x'_i|.
 * For a fixed d, the value of y_1 that minimizes S is the median of {x'_1, x'_2, ..., x'_n}.
 * 
 * The function f(d) = min_{y_1} sum_{i=1}^{n} |y_1 - x'_i| is the partial minimization 
 * of a convex function g(y_1, d) = sum_{i=1}^{n} |y_1 + (i-1)d - x_i|.
 * Since g(y_1, d) is convex in (y_1, d), f(d) is also a convex function of d.
 * We can find the optimal d using ternary search.
 * 
 * Constraints:
 * n up to 400.
 * x_i up to 10000.
 * Time limit is 0.5s, which is tight for 100 iterations of ternary search with sorting.
 * We'll use std::nth_element to find the median in O(n) time.
 */

int N;
vector<double> X;

double f(double d, double &median_out) {
    vector<double> x_prime(N);
    for (int i = 0; i < N; ++i) {
        x_prime[i] = X[i] - (double)i * d;
    }
    
    // Finding the median using nth_element to optimize time complexity to O(N)
    int mid = N / 2;
    nth_element(x_prime.begin(), x_prime.begin() + mid, x_prime.end());
    double median = x_prime[mid];
    
    // If N is even, any value in the range [x_prime_sorted[N/2-1], x_prime_sorted[N/2]] works.
    // Using x_prime[mid] is a valid choice.
    
    double sum = 0;
    for (int i = 0; i < N; ++i) {
        sum += abs(median - x_prime[i]);
    }
    median_out = median;
    return sum;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N)) return 0;
    X.resize(N);
    for (int i = 0; i < N; ++i) {
        cin >> X[i];
    }

    // Ternary search for the optimal d in the range [0, 20000]
    // The range 20000 is enough since x_i range is [-10000, 10000].
    double low = 0, high = 20000;
    for (int iter = 0; iter < 100; ++iter) {
        double m1 = low + (high - low) / 3.0;
        double m2 = high - (high - low) / 3.0;
        double dummy;
        if (f(m1, dummy) < f(m2, dummy)) {
            high = m2;
        } else {
            low = m1;
        }
    }

    // After finding the optimal d, recalculate the minimal sum and corresponding y_1
    double optimal_d = low;
    double optimal_y1;
    double min_shift = f(optimal_d, optimal_y1);

    // Output the result
    cout << fixed << setprecision(10) << min_shift << "\n";
    for (int i = 0; i < N; ++i) {
        double y_i = optimal_y1 + (double)i * optimal_d;
        cout << fixed << setprecision(10) << y_i << (i == N - 1 ? "" : " ");
    }
    cout << endl;

    return 0;
}

