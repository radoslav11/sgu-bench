#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void solve() {
    int T, N;
    if (!(cin >> T >> N)) return;
    while (T--) {
        int total = 3 * N;
        vector<long long> X(total);
        for (int i = 0; i < total; ++i) {
            cin >> X[i];
        }
        
        sort(X.begin(), X.end());
        
        vector<long long> B(X.begin(), X.begin() + N);
        vector<long long> Y(X.begin() + N, X.end());
        
        long long max_sum = 0;
        for (int k = 1; k <= N; ++k) {
            long long u = Y[2 * k - 2];
            long long v = Y[2 * k - 1];
            long long b_val = B[N - k];
            
            long long opt1 = (v - b_val) * u;
            long long opt2 = (u - b_val) * v;
            
            max_sum += max(opt1, opt2);
        }
        
        cout << max_sum << "\n";
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}
