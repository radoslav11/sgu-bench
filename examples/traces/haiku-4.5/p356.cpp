#include<bits/stdc++.h>
using namespace std;

long long gcd(long long a, long long b) {
    while(b) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

long long derangement(int n) {
    if(n == 0) return 1;
    if(n == 1) return 0;
    
    vector<long long> D(n + 1);
    D[0] = 1;
    D[1] = 0;
    
    for(int i = 2; i <= n; i++) {
        D[i] = (long long)(i - 1) * (D[i - 1] + D[i - 2]);
    }
    
    return D[n];
}

long long factorial(int n) {
    long long result = 1;
    for(int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

long long binomial(int n, int k) {
    if(k > n) return 0;
    if(k == 0 || k == n) return 1;
    
    long long numerator = 1;
    long long denominator = 1;
    
    for(int i = 0; i < k; i++) {
        numerator *= (n - i);
        denominator *= (i + 1);
    }
    
    return numerator / denominator;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int K, N;
    cin >> K >> N;
    
    long long numerator = binomial(N, K) * derangement(N - K);
    long long denominator = factorial(N);
    
    if(numerator == 0) {
        cout << "0\n";
    } else {
        long long g = gcd(numerator, denominator);
        numerator /= g;
        denominator /= g;
        cout << numerator << "/" << denominator << "\n";
    }
    
    return 0;
}
