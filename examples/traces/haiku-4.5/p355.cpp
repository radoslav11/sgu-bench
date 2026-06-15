#include<bits/stdc++.h>
using namespace std;

int countPrimeFactors(int n) {
    int count = 0;
    
    // Count factor 2
    while (n % 2 == 0) {
        count++;
        n = n / 2;
    }
    
    // Count odd factors from 3 onwards
    for (int i = 3; i * i <= n; i += 2) {
        while (n % i == 0) {
            count++;
            n = n / i;
        }
    }
    
    // If n is still greater than 1, it's a prime factor
    if (n > 1) {
        count++;
    }
    
    return count;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    cin >> N;
    
    vector<int> colors(N + 1);
    int maxColor = 0;
    
    for (int i = 1; i <= N; i++) {
        int primeFactorCount = countPrimeFactors(i);
        colors[i] = primeFactorCount + 1;
        maxColor = max(maxColor, colors[i]);
    }
    
    cout << maxColor << "\n";
    for (int i = 1; i <= N; i++) {
        cout << colors[i];
        if (i < N) cout << " ";
    }
    cout << "\n";
    
    return 0;
}
