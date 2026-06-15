#include <bits/stdc++.h>
using namespace std;

using ull = unsigned long long;

const ull LIMIT = 9223372036854775807ULL; // signed 64-bit maximum

int K;
vector<ull> primes;
ull bestAns = 0;

void genPrimes(int cnt) {
    for (int x = 2; (int)primes.size() < cnt; ++x) {
        bool ok = true;
        for (int d = 2; d * d <= x; ++d) {
            if (x % d == 0) {
                ok = false;
                break;
            }
        }
        if (ok) primes.push_back((ull)x);
    }
}

void dfs(int idx, int rem, int maxExp, ull cur) {
    if (rem == 1) {
        if (bestAns == 0 || cur < bestAns) bestAns = cur;
        return;
    }

    if (idx >= (int)primes.size()) return;

    ull p = primes[idx];
    ull pw = 1;

    for (int exp = 1; exp <= maxExp; ++exp) {
        if (pw > LIMIT / p) break;
        pw *= p;

        ull bound = (bestAns == 0 ? LIMIT : bestAns - 1);
        if (cur > bound / pw) break;

        if (rem % (exp + 1) == 0) {
            dfs(idx + 1, rem / (exp + 1), exp, cur * pw);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> K;

    if (K == 1) {
        cout << 1 << '\n';
        return 0;
    }

    genPrimes(32);

    dfs(0, K, K - 1, 1);

    cout << bestAns << '\n';
    return 0;
}
