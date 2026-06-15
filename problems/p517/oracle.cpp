#include <bits/stdc++.h>
using namespace std;

// Minimal base-1e9 non-negative big integer supporting addition and decimal
// printing, used to count cornerless domino tilings exactly.
struct Big {
    vector<uint32_t> d;  // little-endian base 1e9 limbs
    Big() {}
    Big(uint64_t v) {
        while(v) {
            d.push_back(v % 1000000000ULL);
            v /= 1000000000ULL;
        }
    }

    void trim() {
        while(!d.empty() && d.back() == 0) {
            d.pop_back();
        }
    }
};

Big add(const Big& a, const Big& b) {
    Big r;
    uint64_t carry = 0;
    size_t n = max(a.d.size(), b.d.size());
    for(size_t i = 0; i < n || carry; i++) {
        uint64_t cur = carry;
        if(i < a.d.size()) {
            cur += a.d[i];
        }
        if(i < b.d.size()) {
            cur += b.d[i];
        }
        r.d.push_back(cur % 1000000000ULL);
        carry = cur / 1000000000ULL;
    }
    r.trim();
    return r;
}

string to_string_big(const Big& a) {
    if(a.d.empty()) {
        return "0";
    }
    string s = to_string(a.d.back());
    for(int i = (int)a.d.size() - 2; i >= 0; i--) {
        string part = to_string(a.d[i]);
        s += string(9 - part.size(), '0') + part;
    }
    return s;
}

int main() {
    int m, n;
    cin >> m >> n;

    if((long long)m * n % 2 == 1) {
        cout << 0 << "\n";
        return 0;
    }

    if(n > m) {
        swap(n, m);
    }

    if(n == 1) {
        cout << 1 << "\n";
        return 0;
    }

    if(n == 2) {
        vector<Big> dp(m + 1);
        dp[0] = Big(1);
        dp[1] = Big(1);
        dp[2] = Big(2);
        for(int i = 3; i <= m; i++) {
            dp[i] = add(dp[i - 1], dp[i - 3]);
        }
        cout << to_string_big(dp[m]) << "\n";
        return 0;
    }

    if(n % 2 == 1) {
        vector<Big> dp(m + 1);
        dp[0] = Big(1);
        for(int i = 1; i <= m; i++) {
            dp[i] = Big(0);
            for(int delta: {n - 1, n + 1}) {
                if(i - delta >= 0) {
                    dp[i] = add(dp[i], dp[i - delta]);
                }
            }
        }
        cout << to_string_big(add(dp[m], dp[m])) << "\n";
        return 0;
    }

    vector<array<Big, 2>> dp(m + 1);
    dp[0][0] = Big(1);
    dp[0][1] = Big(1);
    for(int i = 1; i <= m; i++) {
        dp[i][0] = dp[i - 1][1];
        dp[i][1] = Big(0);
        for(int delta: {n - 2, n}) {
            if(i - delta >= 0) {
                dp[i][1] = add(dp[i][1], dp[i - delta][0]);
            }
        }
    }
    cout << to_string_big(add(dp[m][0], dp[m][1])) << "\n";
    return 0;
}
