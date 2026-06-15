#include <bits/stdc++.h>
using namespace std;

vector<int> sieve(int n) {
    vector<bool> is_prime(n + 1, true);
    vector<int> primes;
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i <= n; i++) {
        if (is_prime[i]) {
            primes.push_back(i);
            for (long long j = (long long)i * i; j <= n; j += i) {
                is_prime[j] = false;
            }
        }
    }
    return primes;
}

vector<int> getParity(long long num, vector<int>& primes) {
    vector<int> parity(primes.size(), 0);
    for (int i = 0; i < primes.size(); i++) {
        int cnt = 0;
        while (num % primes[i] == 0) {
            num /= primes[i];
            cnt++;
        }
        parity[i] = cnt % 2;
    }
    return parity;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t, m;
    cin >> t >> m;
    
    // Get first t primes
    vector<int> primes = sieve(300); // 300 is enough to get 62 primes, more than 100
    primes.resize(t);
    
    vector<vector<int>> parities;
    for (int i = 0; i < m; i++) {
        long long b;
        cin >> b;
        parities.push_back(getParity(b, primes));
    }
    
    // Gaussian elimination over GF(2)
    vector<vector<int>> basis = parities;
    int rank = 0;
    
    for (int col = 0; col < t && rank < m; col++) {
        // Find pivot
        int pivot = -1;
        for (int row = rank; row < m; row++) {
            if (basis[row][col] == 1) {
                pivot = row;
                break;
            }
        }
        
        if (pivot == -1) continue;
        
        // Swap rows
        swap(basis[rank], basis[pivot]);
        
        // Eliminate
        for (int row = 0; row < m; row++) {
            if (row != rank && basis[row][col] == 1) {
                for (int c = 0; c < t; c++) {
                    basis[row][c] ^= basis[rank][c];
                }
            }
        }
        
        rank++;
    }
    
    // Number of solutions = 2^(m - rank) - 1
    long long ans = (1LL << (m - rank)) - 1;
    cout << ans << endl;
    
    return 0;
}
