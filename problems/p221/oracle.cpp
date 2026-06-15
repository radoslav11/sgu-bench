#include <bits/stdc++.h>
using namespace std;

// Non-negative bignum, base 1e9 limbs, little-endian.
struct Big {
    vector<int> d;
    Big(long long x = 0) {
        while(x > 0) {
            d.push_back((int)(x % 1000000000));
            x /= 1000000000;
        }
    }

    void trim() {
        while(!d.empty() && d.back() == 0) d.pop_back();
    }
};

Big add(const Big& a, const Big& b) {
    Big r;
    r.d.assign(max(a.d.size(), b.d.size()) + 1, 0);
    long long carry = 0;
    for(size_t i = 0; i < r.d.size(); i++) {
        long long cur = carry;
        if(i < a.d.size()) cur += a.d[i];
        if(i < b.d.size()) cur += b.d[i];
        r.d[i] = (int)(cur % 1000000000);
        carry = cur / 1000000000;
    }
    r.trim();
    return r;
}

Big mul(const Big& a, const Big& b) {
    if(a.d.empty() || b.d.empty()) return Big(0);
    vector<__int128> acc(a.d.size() + b.d.size(), 0);
    for(size_t i = 0; i < a.d.size(); i++) {
        for(size_t j = 0; j < b.d.size(); j++) {
            acc[i + j] += (long long)a.d[i] * b.d[j];
        }
    }
    Big r;
    r.d.assign(acc.size() + 1, 0);
    __int128 carry = 0;
    for(size_t i = 0; i < acc.size(); i++) {
        __int128 cur = acc[i] + carry;
        r.d[i] = (int)(cur % 1000000000);
        carry = cur / 1000000000;
    }
    size_t pos = acc.size();
    while(carry > 0) {
        r.d[pos++] = (int)(carry % 1000000000);
        carry /= 1000000000;
    }
    r.trim();
    return r;
}

string to_string(const Big& a) {
    if(a.d.empty()) return "0";
    string s = to_string(a.d.back());
    for(int i = (int)a.d.size() - 2; i >= 0; i--) {
        string part = to_string(a.d[i]);
        s += string(9 - part.size(), '0') + part;
    }
    return s;
}

// dp over diagonals of one color, sorted by length ascending:
// dp[j] after processing a diagonal of length v gains dp[j-1]*(v-j+1).
vector<Big> ways_one_color(int n, int k, int start) {
    vector<int> lens;
    for(int d = start; d < 2 * n - 1; d += 2) {
        lens.push_back(d < n ? d + 1 : 2 * n - 1 - d);
    }
    sort(lens.begin(), lens.end());

    vector<Big> dp(k + 1, Big(0));
    dp[0] = Big(1);
    for(int v: lens) {
        for(int j = min(k, v); j >= 1; j--) {
            dp[j] = add(dp[j], mul(dp[j - 1], Big(v - j + 1)));
        }
    }
    return dp;
}

int main() {
    int n;
    long long k;
    cin >> n >> k;
    if(k > 2LL * n - 1) {
        cout << 0 << "\n";
        return 0;
    }
    int kk = (int)k;
    vector<Big> even = ways_one_color(n, kk, 0);
    vector<Big> odd = ways_one_color(n, kk, 1);
    Big ans(0);
    for(int i = 0; i <= kk; i++) {
        ans = add(ans, mul(even[i], odd[kk - i]));
    }
    cout << to_string(ans) << "\n";
    return 0;
}
