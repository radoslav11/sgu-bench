#include <iostream>
#include <vector>
#include <map>
#include <numeric>
#include <algorithm>
#include <iomanip>
#include <random>

using namespace std;

uint64_t mult_mod(uint64_t a, uint64_t b, uint64_t m) {
    return (uint64_t)((unsigned __int128)a * b % m);
}

uint64_t power(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = mult_mod(res, base, mod);
        base = mult_mod(base, base, mod);
        exp /= 2;
    }
    return res;
}

bool miller_rabin(uint64_t n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;
    uint64_t d = n - 1;
    int s = 0;
    while (d % 2 == 0) { d /= 2; s++; }
    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (uint64_t a : bases) {
        if (n <= a) break;
        uint64_t x = power(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; r++) {
            x = mult_mod(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

uint64_t pollard_rho(uint64_t n) {
    if (n % 2 == 0) return 2;
    if (miller_rabin(n)) return n;
    uint64_t x = 2, y = 2, d = 1, c = 1;
    auto f = [&](uint64_t val, uint64_t mod, uint64_t cst) {
        return (mult_mod(val, val, mod) + cst) % mod;
    };
    while (d == 1) {
        x = f(x, n, c);
        y = f(f(y, n, c), n, c);
        uint64_t diff = x > y ? x - y : y - x;
        d = std::gcd(diff, n);
        if (d == n) {
            x = rand() % (n - 2) + 2;
            y = x;
            c = rand() % (n - 1) + 1;
            d = 1;
        }
    }
    return d;
}

void factorize(uint64_t n, map<uint64_t, int>& factors) {
    if (n == 1) return;
    if (miller_rabin(n)) {
        factors[n]++;
        return;
    }
    uint64_t d = pollard_rho(n);
    factorize(d, factors);
    factorize(n / d, factors);
}

struct BigInt {
    vector<uint32_t> d;
    BigInt(uint64_t v = 0) {
        if (v == 0) d = {0};
        else {
            while (v > 0) {
                d.push_back(v % 1000000000);
                v /= 1000000000;
            }
        }
    }
    void trim() {
        while (d.size() > 1 && d.back() == 0) d.pop_back();
    }
    
    BigInt operator+(const BigInt& o) const {
        BigInt res; res.d.clear();
        uint64_t carry = 0;
        size_t n = max(d.size(), o.d.size());
        for (size_t i = 0; i < n; ++i) {
            uint64_t sum = carry;
            if (i < d.size()) sum += d[i];
            if (i < o.d.size()) sum += o.d[i];
            res.d.push_back(sum % 1000000000);
            carry = sum / 1000000000;
        }
        if (carry) res.d.push_back(carry);
        res.trim();
        return res;
    }
    
    BigInt operator*(uint64_t v) const {
        if (v == 0) return BigInt(0);
        BigInt res; res.d.clear();
        uint64_t carry = 0;
        for (size_t i = 0; i < d.size(); ++i) {
            uint64_t cur = (uint64_t)d[i] * v + carry;
            res.d.push_back(cur % 1000000000);
            carry = cur / 1000000000;
        }
        while (carry) {
            res.d.push_back(carry % 1000000000);
            carry /= 1000000000;
        }
        res.trim();
        return res;
    }
    
    uint64_t mod(uint64_t v) const {
        uint64_t rem = 0;
        for (int i = (int)d.size() - 1; i >= 0; --i) {
            rem = (rem * 1000000000ULL + d[i]) % v;
        }
        return rem;
    }
    
    BigInt div_uint(uint64_t v) const {
        BigInt res; res.d.assign(d.size(), 0);
        uint64_t rem = 0;
        for (int i = (int)d.size() - 1; i >= 0; --i) {
            uint64_t cur = rem * 1000000000ULL + d[i];
            res.d[i] = cur / v;
            rem = cur % v;
        }
        res.trim();
        return res;
    }
    
    void print() const {
        if (d.empty()) { cout << 0; return; }
        cout << d.back();
        for (int i = (int)d.size() - 2; i >= 0; --i) {
            cout << setfill('0') << setw(9) << d[i];
        }
    }
};

uint64_t C[55][55];

void precompute() {
    for (int i = 0; i <= 50; ++i) {
        C[i][0] = 1;
        for (int j = 1; j <= i; ++j) {
            C[i][j] = C[i-1][j-1] + C[i-1][j];
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    precompute();
    
    int n;
    if (!(cin >> n)) return 0;
    
    BigInt D = 1;
    vector<BigInt> N;
    N.push_back(1);
    
    map<uint64_t, int> D_factors_map;
    
    for (int k = 1; k <= n; ++k) {
        uint64_t M = 3ULL * (1ULL << k) - 2;
        factorize(M, D_factors_map);
        
        BigInt sum = D;
        for (int j = 0; j < k; ++j) {
            BigInt term = N[j] * C[k][j];
            sum = sum + term;
        }
        
        D = D * M;
        for (int j = 0; j < k; ++j) {
            N[j] = N[j] * M;
        }
        
        N.push_back(sum);
    }
    
    BigInt p_num = N[n];
    BigInt p_den = D;
    
    for (auto& kv : D_factors_map) {
        uint64_t p = kv.first;
        int count = kv.second;
        while (count > 0 && p_num.mod(p) == 0) {
            p_num = p_num.div_uint(p);
            p_den = p_den.div_uint(p);
            count--;
        }
    }
    
    p_num.print();
    cout << "/";
    p_den.print();
    cout << "\n";
    
    return 0;
}
