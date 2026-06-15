#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

const int MOD = 1e9 + 7;

long long power(long long base, long long exp) {
    long long res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp /= 2;
    }
    return res;
}

long long modInverse(long long n) {
    return power(n, MOD - 2);
}

vector<long long> fact;
vector<long long> invFact;

void precompute(int n) {
    fact.resize(n + 1);
    invFact.resize(n + 1);
    fact[0] = 1;
    invFact[0] = 1;
    for (int i = 1; i <= n; i++) {
        fact[i] = (fact[i - 1] * i) % MOD;
    }
    invFact[n] = modInverse(fact[n]);
    for (int i = n - 1; i >= 1; i--) {
        invFact[i] = (invFact[i + 1] * (i + 1)) % MOD;
    }
}

long long nCr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fact[n] * invFact[r] % MOD * invFact[n - r] % MOD;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string s1, s2, s3;
    if (!(cin >> s1 >> s2 >> s3)) return 0;

    int n = s1.length();
    precompute(n);

    int n0 = 0, n1 = 0, n2 = 0, n3 = 0;
    for (int i = 0; i < n; i++) {
        int b1 = s1[i] - '0';
        int b2 = s2[i] - '0';
        int b3 = s3[i] - '0';
        if ((b1 == 0 && b2 == 0 && b3 == 0) || (b1 == 1 && b2 == 1 && b3 == 1)) {
            n0++;
        } else if ((b1 == 0 && b2 == 1 && b3 == 0) || (b1 == 1 && b2 == 0 && b3 == 1)) {
            n1++;
        } else if ((b1 == 0 && b2 == 0 && b3 == 1) || (b1 == 1 && b2 == 1 && b3 == 0)) {
            n2++;
        } else if ((b1 == 1 && b2 == 0 && b3 == 0) || (b1 == 0 && b2 == 1 && b3 == 1)) {
            n3++;
        }
    }

    if ((n1 % 2) != (n2 % 2) || (n1 % 2) != (n3 % 2)) {
        cout << 0 << "\n";
        return 0;
    }

    long long ans = 0;
    int x_min = max({0, (n1 - n2) / 2, (n1 - n3) / 2});
    int x_max = min({n1, (n1 + n2) / 2, (n1 + n3) / 2});

    for (int x = x_min; x <= x_max; x++) {
        long long ways = nCr(n1, x);
        ways = ways * nCr(n2, x + (n2 - n1) / 2) % MOD;
        ways = ways * nCr(n3, (n1 + n3) / 2 - x) % MOD;
        ans = (ans + ways) % MOD;
    }

    long long final_ans = (ans * power(2, n0)) % MOD;
    cout << final_ans << "\n";

    return 0;
}
