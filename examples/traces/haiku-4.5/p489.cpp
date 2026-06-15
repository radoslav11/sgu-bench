#include <bits/stdc++.h>
using namespace std;

long long power(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long n, m;
    cin >> n >> m;
    
    // The answer is 2^(n-1) mod m
    long long answer = power(2, n - 1, m);
    
    cout << answer << endl;
    
    return 0;
}
