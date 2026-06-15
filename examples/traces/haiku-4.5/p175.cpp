#include<bits/stdc++.h>
using namespace std;

long long solve(long long N, long long q) {
    if (N == 1) return 1;
    
    long long K = N / 2;
    
    if (q <= K) {
        // In first half - goes to second half of result
        return K + (N - K) + 1 - solve(K, K + 1 - q);
    } else {
        // In second half - goes to first half of result
        return solve(N - K, N + 1 - q);
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long N, q;
    cin >> N >> q;
    
    cout << solve(N, q) << endl;
    
    return 0;
}
