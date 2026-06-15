#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Problem Analysis:
 * A hill is a continuous subsequence a[i...k] such that there exists a j (i < j < k) 
 * where a[i] < a[i+1] < ... < a[j] and a[j] > a[j+1] > ... > a[k].
 * Based on the sample analysis, the height of a hill with peak j is defined as 
 * min(a[j] - a[i], a[j] - a[k]), where i and k are the boundaries of the maximal 
 * increasing/decreasing segments.
 * 
 * A dale is a continuous subsequence a[i...k] such that there exists a j (i < j < k) 
 * where a[i] > a[i+1] > ... > a[j] and a[j] < a[j+1] < ... < a[k].
 * The depth of a dale with valley j is defined as min(a[i] - a[j], a[k] - a[j]), 
 * where i and k are the boundaries of the maximal decreasing/increasing segments.
 * 
 * For each index j, we identify the maximal increasing and decreasing sequences 
 * that can serve as parts of a hill or a dale.
 * 
 * Constraints:
 * Sum of N <= 10^6.
 * Time limit: 0.5s (Requires fast I/O).
 * Memory limit: 256 MB.
 */

void solve() {
    int N;
    if (!(cin >> N)) return;
    vector<long long> a(N);
    for (int i = 0; i < N; ++i) {
        cin >> a[i];
    }

    if (N < 3) {
        cout << "0 0" << endl;
        return;
    }

    // Precompute the boundaries of strictly increasing/decreasing segments
    // i_inc[j] is the smallest index i such that a[i] < a[i+1] < ... < a[j]
    // k_dec[j] is the largest index k such that a[j] > a[j+1] > ... > a[k]
    // i_dec[j] is the smallest index i such that a[i] > a[i+1] > ... > a[j]
    // k_inc[j] is the largest index k such that a[j] < a[j+1] < ... < a[k]

    vector<int> i_inc(N), k_dec(N), i_dec(N), k_inc(N);

    i_inc[0] = 0;
    for (int j = 1; j < N; ++j) {
        if (a[j] > a[j-1]) {
            i_inc[j] = i_inc[j-1];
        } else {
            i_inc[j] = j;
        }
    }

    k_dec[N-1] = N-1;
    for (int j = N-2; j >= 0; --j) {
        if (a[j] > a[j+1]) {
            k_dec[j] = k_dec[j+1];
        } else {
            k_dec[j] = j;
        }
    }

    i_dec[0] = 0;
    for (int j = 1; j < N; ++j) {
        if (a[j] < a[j-1]) {
            i_dec[j] = i_dec[j-1];
        } else {
            i_dec[j] = j;
        }
    }

    k_inc[N-1] = N-1;
    for (int j = N-2; j >= 0; --j) {
        if (a[j] < a[j+1]) {
            k_inc[j] = k_inc[j+1];
        } else {
            k_inc[j] = j;
        }
    }

    long long max_h = 0;
    long long max_d = 0;

    for (int j = 0; j < N; ++j) {
        // Check for a hill with peak at index j
        if (i_inc[j] < j && k_dec[j] > j) {
            // The height is the minimum of the two sides of the peak
            long long h = min(a[j] - a[i_inc[j]], a[j] - a[k_dec[j]]);
            if (h > max_h) max_h = h;
        }
        // Check for a dale with valley at index j
        if (i_dec[j] < j && k_inc[j] > j) {
            // The depth is the minimum of the two sides of the valley
            long long d = min(a[i_dec[j]] - a[j], a[k_inc[j]] - a[j]);
            if (d > max_d) max_d = d;
        }
    }

    cout << max_h << " " << max_d << endl;
}

int main() {
    // Speed up input/output
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        solve();
    }
    return 0;
}

