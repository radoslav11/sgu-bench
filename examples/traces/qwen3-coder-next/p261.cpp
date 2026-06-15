#include <bits/stdc++.h>
using namespace std;

// Extended Euclidean Algorithm
long long extGcd(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    long long x1, y1;
    long long d = extGcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return d;
}

// Modular exponentiation
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

// Modular inverse
long long modInverse(long long a, long long mod) {
    long long x, y;
    long long g = extGcd(a, mod, x, y);
    if (g != 1) {
        return -1; // inverse doesn't exist
    }
    return (x % mod + mod) % mod;
}

// Find primitive root of prime p
long long findPrimitiveRoot(long long p) {
    if (p == 2) return 1;
    
    vector<long long> factors;
    long long phi = p - 1;
    long long temp = phi;
    
    // Factorize phi = p-1
    for (long long i = 2; i * i <= temp; ++i) {
        if (temp % i == 0) {
            factors.push_back(i);
            while (temp % i == 0) {
                temp /= i;
            }
        }
    }
    if (temp > 1) {
        factors.push_back(temp);
    }
    
    // Find smallest primitive root
    for (long long g = 2; g < p; ++g) {
        bool isPrimitive = true;
        for (long long factor : factors) {
            if (modPow(g, phi / factor, p) == 1) {
                isPrimitive = false;
                break;
            }
        }
        if (isPrimitive) {
            return g;
        }
    }
    return -1; // shouldn't happen for prime p
}

// Solve x^k = a (mod p), where p is prime
vector<long long> solveDiscreteRoots(long long p, long long k, long long a) {
    // Special case: a == 0
    if (a == 0) {
        return {0};
    }
    
    // Find primitive root g of p
    long long g = findPrimitiveRoot(p);
    
    // Solve g^(ind_g(x)*k) = g^(ind_g(a)) (mod p)
    // => ind_g(x)*k ≡ ind_g(a) (mod (p-1))
    
    // First, compute discrete log: find y such that g^y ≡ a (mod p)
    // Use baby-step giant-step algorithm
    long long m = (long long)ceil(sqrt(p - 1));
    
    // Baby steps: store g^j for j in [0, m)
    map<long long, long long> babySteps;
    long long current = 1;
    for (long long j = 0; j < m; ++j) {
        babySteps[current] = j;
        current = (current * g) % p;
    }
    
    // Compute g^(-m) mod p
    long long g_inv_m = modPow(modInverse(g, p), m, p);
    
    // Giant steps: find i such that a * (g^(-m))^i is in babySteps
    long long gamma = a;
    long long discreteLog = -1;
    for (long long i = 0; i < m; ++i) {
        if (babySteps.count(gamma)) {
            discreteLog = i * m + babySteps[gamma];
            break;
        }
        gamma = (gamma * g_inv_m) % p;
    }
    
    if (discreteLog == -1) {
        // No solution exists (except for a=0 which we handled already)
        return {};
    }
    
    // Now solve k * x ≡ discreteLog (mod (p-1))
    long long n = p - 1;
    long long d = __gcd(k, n);
    
    if (discreteLog % d != 0) {
        return {};
    }
    
    // Reduce equation: (k/d) * x ≡ (discreteLog/d) (mod (n/d))
    long long k_reduced = k / d;
    long long n_reduced = n / d;
    long long rhs_reduced = discreteLog / d;
    
    // Find inverse of k_reduced modulo n_reduced
    long long inv_k = modInverse(k_reduced, n_reduced);
    if (inv_k == -1) {
        return {};
    }
    
    // One solution
    long long x0 = (inv_k * rhs_reduced) % n_reduced;
    if (x0 < 0) x0 += n_reduced;
    
    // Generate all d solutions modulo n
    vector<long long> solutionsModP;
    for (long long i = 0; i < d; ++i) {
        long long x = (x0 + i * n_reduced) % n;
        // Compute g^x mod p
        long long root = modPow(g, x, p);
        solutionsModP.push_back(root);
    }
    
    // Sort and deduplicate (though there shouldn't be duplicates)
    sort(solutionsModP.begin(), solutionsModP.end());
    
    return solutionsModP;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long p, k, a;
    cin >> p >> k >> a;
    
    vector<long long> roots;
    
    // Handle special case: a == 0
    if (a == 0) {
        roots.push_back(0);
    } else {
        roots = solveDiscreteRoots(p, k, a);
    }
    
    cout << roots.size() << "\n";
    for (size_t i = 0; i < roots.size(); ++i) {
        if (i > 0) cout << " ";
        cout << roots[i];
    }
    if (!roots.empty()) cout << "\n";
    
    return 0;
}
