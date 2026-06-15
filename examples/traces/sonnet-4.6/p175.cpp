#include <bits/stdc++.h>
using namespace std;

long long solve(long long N, long long q) {
    if (N == 1) return 1;
    long long K = N / 2;
    long long second_len = N - K; // length of second half
    if (q > K) {
        // in second half, position in reversed second half
        long long pos = N - q + 1;
        return solve(second_len, pos);
    } else {
        // in first half, position in reversed first half
        long long pos = K - q + 1;
        return second_len + solve(K, pos);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    long long N, q;
    cin >> N >> q;
    cout << solve(N, q) << endl;
    return 0;
}
