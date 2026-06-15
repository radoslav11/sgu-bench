#include <iostream>
#include <vector>

using namespace std;

/**
 * Problem: Solve A*X = B (mod P), where A is a sequence of N integers.
 * This is equivalent to finding X1, X2, ..., XN such that
 * A1*X1 + A2*X2 + ... + AN*XN ≡ B (mod P).
 * This can be written as A1*X1 + A2*X2 + ... + AN*XN + P*Y = B.
 * This is a linear Diophantine equation in N+1 variables.
 * A solution exists if and only if gcd(A1, A2, ..., AN, P) divides B.
 * 
 * The approach is to use the extended Euclidean algorithm iteratively.
 * Let g0 = P.
 * For i = 1 to N, let gi = gcd(Ai, gi-1).
 * Using the extended Euclidean algorithm, we can find xi and yi such that:
 * Ai * xi + gi-1 * yi = gi.
 * By substituting these equations iteratively, we can express gN as:
 * gN = AN*xN + AN-1*(xN-1 * yN) + AN-2*(xN-2 * yN-1 * yN) + ... + A1*(x1 * y2 * ... * yN) + P*(y1 * ... * yN).
 * Given B is a multiple of gN, let factor = B / gN.
 * Then the solution for each Xi is:
 * Xi = xi * (y_{i+1} * ... * yN) * factor (modulo P).
 */

// Extended Euclidean Algorithm to find x, y such that ax + by = gcd(a, b)
long long extgcd(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    long long x1, y1;
    long long d = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return d;
}

int main() {
    // Optimized I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    long long P, B;
    if (!(cin >> N >> P >> B)) return 0;

    vector<long long> A(N);
    for (int i = 0; i < N; ++i) {
        cin >> A[i];
        // Work with elements modulo P
        A[i] %= P;
    }

    // g[i] stores the gcd of (A[0], ..., A[i-1], P)
    vector<long long> g(N + 1);
    // x[i] and y[i] are the coefficients such that A[i-1]*x[i] + g[i-1]*y[i] = g[i]
    vector<long long> x(N + 1);
    vector<long long> y(N + 1);

    g[0] = P;
    for (int i = 1; i <= N; ++i) {
        g[i] = extgcd(A[i-1], g[i-1], x[i], y[i]);
    }

    // Check if B is divisible by the final GCD
    if (B % g[N] != 0) {
        cout << "NO" << endl;
    } else {
        cout << "YES" << endl;
        vector<long long> X(N);
        long long factor = B / g[N];
        long long current_y_prod = 1;

        // Calculate Xi from back to front using the derived formula
        for (int i = N; i >= 1; --i) {
            // We compute Xi = (xi * prod_{j=i+1}^N y_j * factor) mod P
            // To prevent overflow and handle negative values, perform modulo P at each step.
            long long term_x = (x[i] % P + P) % P;
            long long term_y = (current_y_prod % P + P) % P;
            long long term_f = (factor % P + P) % P;
            
            long long val = (term_x * term_y) % P;
            val = (val * term_f) % P;
            X[i-1] = val;

            // Update the product of y coefficients for the next iteration (i-1)
            long long term_yi = (y[i] % P + P) % P;
            current_y_prod = (current_y_prod % P + P) % P * term_yi % P;
        }

        for (int i = 0; i < N; ++i) {
            cout << X[i] << (i == N - 1 ? "" : " ");
        }
        cout << endl;
    }

    return 0;
}

