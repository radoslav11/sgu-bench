#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
using namespace std;

long long modPow(long long base, long long exp, long long mod) {
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

long long modInverse(long long a, long long mod) {
    return modPow(a, mod - 2, mod);
}

int gcd(int a, int b) {
    while (b) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int main() {
    long long N, M, P;
    cin >> N >> M >> P;
    
    M %= P;
    
    // dp[used] = list of (denominator product, exponent) for partitions of 'used'
    // We'll iterate by cycle length
    vector<vector<pair<long long, long long>>> dp(N + 1);
    
    // dp[i] contains pairs (den, exp) where den = denominator product for current partial partition
    // and exp = edge orbit count for current partial partition
    dp[0].push_back({1, 0});
    
    for (int len = 1; len <= N; len++) {
        // For each cycle length from N down to 1 (to avoid using same cycle multiple times)
        // Actually, iterate len from 1 to N
        vector<vector<pair<long long, long long>>> new_dp = dp;
        
        for (int count = 1; count * len <= N; count++) {
            long long denom_factor = 1;
            for (int i = 1; i <= count; i++) {
                denom_factor = (denom_factor * i) % P;
            }
            denom_factor = (denom_factor * modPow(len, count, P)) % P;
            
            long long edge_orbits_added = 0;
            // Within each cycle of length len: floor(len/2) orbits per cycle
            edge_orbits_added += count * (len / 2);
            // Between cycles of same length: C(count, 2) * len orbits
            edge_orbits_added += (count * (count - 1) / 2) * len;
            // With previous cycles: for each previous cycle length k with a_k cycles, add a_k * count * gcd(k, len)
            // But we don't have that info in dp...
            
            // Actually, we need to store the full partition info or compute differently
            // Let's use a different approach: iterate over all partitions using recursion
        }
    }
    
    // Better approach: generate all partitions using recursion
    vector<int> parts;
    long long result = 0;
    
    function<void(int, int, long long, long long)> dfs = [&](int remaining, int min_part, long long denom, long long exp) {
        if (remaining == 0) {
            // Compute M^exp / denom mod P
            long long term = modPow(M, exp, P);
            term = (term * modInverse(denom % P, P)) % P;
            result = (result + term) % P;
            return;
        }
        
        for (int len = min_part; len <= remaining; len++) {
            // How many cycles of this length? try count from 1 to remaining/len
            int max_count = remaining / len;
            
            long long denom_factor = modPow(len, 1, P);
            denom_factor = (denom_factor * 1) % P; // first cycle
            
            for (int count = 1; count <= max_count; count++) {
                long long new_denom = (denom * modPow(len, 1, P) % P * count) % P;
                
                long long new_exp = exp;
                // Add contribution of this cycle
                // Within cycle: floor(len/2) orbits
                new_exp += len / 2;
                // Between this cycle and previous cycles of same length: (count-1) * len orbits
                if (count > 1) {
                    new_exp += (count - 1) * len;
                }
                // Between this cycle and previous cycles of different lengths
                // We need to know the previous cycle lengths...
                
                // This approach is getting messy. Let's use the standard method.
            }
        }
    };
    
    // Let's implement the standard Burnside's lemma approach with partitions
    
    // We'll generate all integer partitions of N
    vector<vector<int>> partitions;
    vector<int> current;
    
    function<void(int, int)> generate_partitions = [&](int n, int max_val) {
        if (n == 0) {
            partitions.push_back(current);
            return;
        }
        for (int i = min(n, max_val); i >= 1; i--) {
            current.push_back(i);
            generate_partitions(n - i, i);
            current.pop_back();
        }
    };
    
    generate_partitions(N, N);
    
    // For each partition, convert to cycle type (a_1, a_2, ..., a_N)
    for (const auto& part : partitions) {
        vector<int> count(N + 1, 0); // count[i] = number of cycles of length i
        for (int len : part) {
            count[len]++;
        }
        
        // Compute denominator = ∏_{i=1}^N (i^{count[i]} * count[i]!)
        long long denom = 1;
        for (int i = 1; i <= N; i++) {
            if (count[i] > 0) {
                // i^{count[i]}
                denom = (denom * modPow(i, count[i], P)) % P;
                // count[i]!
                for (int j = 1; j <= count[i]; j++) {
                    denom = (denom * j) % P;
                }
            }
        }
        
        // Compute edge orbit count E
        long long E = 0;
        // Within cycles
        for (int i = 1; i <= N; i++) {
            if (count[i] > 0) {
                E += (long long)count[i] * (i / 2);
            }
        }
        // Between cycles of same length i: C(count[i], 2) * i
        for (int i = 1; i <= N; i++) {
            if (count[i] > 1) {
                E += (long long)count[i] * (count[i] - 1) / 2 * i;
            }
        }
        // Between cycles of different lengths i and j: count[i] * count[j] * gcd(i,j)
        for (int i = 1; i <= N; i++) {
            for (int j = i + 1; j <= N; j++) {
                if (count[i] > 0 && count[j] > 0) {
                    E += (long long)count[i] * count[j] * gcd(i, j);
                }
            }
        }
        
        // Add term M^E / denom
        long long term = modPow(M, E, P);
        term = (term * modInverse(denom % P, P)) % P;
        result = (result + term) % P;
    }
    
    cout << result << endl;
    
    return 0;
}
