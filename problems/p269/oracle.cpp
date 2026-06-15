// Editorial C++ solution for p269 (Rooks). The original used
// boost::multiprecision::cpp_int, which is not available here, so BigInt
// is replaced by a minimal base-1e9 unsigned big integer supporting the
// two operations the DP needs: multiply by a small int and add.
#include <bits/stdc++.h>
using namespace std;

struct BigInt {
    // little-endian limbs, base 1e9
    vector<uint32_t> d;

    BigInt(uint32_t v = 0) {
        if(v) d.push_back(v);
    }

    bool zero() const { return d.empty(); }

    BigInt mul_small(uint64_t m) const {
        BigInt r;
        if(zero() || m == 0) return r;
        r.d.assign(d.size(), 0);
        uint64_t carry = 0;
        for(size_t i = 0; i < d.size(); i++) {
            uint64_t cur = (uint64_t)d[i] * m + carry;
            r.d[i] = (uint32_t)(cur % 1000000000ULL);
            carry = cur / 1000000000ULL;
        }
        while(carry) {
            r.d.push_back((uint32_t)(carry % 1000000000ULL));
            carry /= 1000000000ULL;
        }
        return r;
    }

    void add(const BigInt& o) {
        if(o.zero()) return;
        d.resize(max(d.size(), o.d.size()) + 1, 0);
        uint64_t carry = 0;
        for(size_t i = 0; i < d.size(); i++) {
            uint64_t cur = carry + d[i] + (i < o.d.size() ? o.d[i] : 0);
            d[i] = (uint32_t)(cur % 1000000000ULL);
            carry = cur / 1000000000ULL;
        }
        while(!d.empty() && d.back() == 0) d.pop_back();
    }

    string str() const {
        if(zero()) return "0";
        string s = to_string(d.back());
        for(int i = (int)d.size() - 2; i >= 0; i--) {
            string part = to_string(d[i]);
            s += string(9 - part.size(), '0') + part;
        }
        return s;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> b(n);
    for(int i = 0; i < n; i++) {
        cin >> b[i];
    }

    sort(b.begin(), b.end());

    // dp[j] = number of ways to place exactly j rooks so far
    vector<BigInt> dp(k + 1);
    dp[0] = BigInt(1);

    for(int idx = 0; idx < n; idx++) {
        int v = b[idx];
        for(int j = k; j >= 1; j--) {
            long long ways = (long long)v - (j - 1);
            if(ways > 0 && !dp[j - 1].zero()) {
                dp[j].add(dp[j - 1].mul_small((uint64_t)ways));
            }
        }
    }

    cout << dp[k].str() << "\n";
    return 0;
}
