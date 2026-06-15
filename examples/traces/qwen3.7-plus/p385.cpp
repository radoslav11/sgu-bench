#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    // W[i][j] stores the number of cycles of length j containing a specific element 
    // in a set of i elements. This is equal to (i-1)! / (i-j)!
    vector<vector<long double>> W(n + 1, vector<long double>(n + 1, 0.0L));
    for (int i = 1; i <= n; ++i) {
        W[i][1] = 1.0L;
        for (int j = 2; j <= i; ++j) {
            W[i][j] = W[i][j - 1] * (long double)(i - j + 1);
        }
    }

    // D[i] stores the number of derangements of size i
    vector<long double> D(n + 1, 0.0L);
    D[0] = 1.0L;
    D[1] = 0.0L;
    for (int i = 2; i <= n; ++i) {
        for (int j = 2; j <= i; ++j) {
            D[i] += W[i][j] * D[i - j];
        }
    }

    // total_V will store the sum over all derangements of the number of elements 
    // in cycles of maximum length.
    long double total_V = 0.0L;
    
    for (int k = 2; k <= n; ++k) {
        // N_k[i] is the number of permutations of i elements with cycle lengths in {2, ..., k}
        vector<long double> N_k(n + 1, 0.0L);
        // V_k[i] is the sum of (number of elements in cycles of length k) over all 
        // permutations of i elements with cycle lengths in {2, ..., k}
        vector<long double> V_k(n + 1, 0.0L);
        
        N_k[0] = 1.0L;
        N_k[1] = 0.0L;
        V_k[0] = 0.0L;
        V_k[1] = 0.0L;

        for (int i = 2; i <= n; ++i) {
            for (int j = 2; j <= min(i, k); ++j) {
                N_k[i] += W[i][j] * N_k[i - j];
            }
            
            if (i >= k) {
                V_k[i] += (long double)k * W[i][k] * N_k[i - k];
            }
            for (int j = 2; j <= min(i, k); ++j) {
                V_k[i] += W[i][j] * V_k[i - j];
            }
        }
        total_V += V_k[n];
    }

    long double ans = total_V / D[n];
    cout << fixed << setprecision(10) << ans << "\n";

    return 0;
}
