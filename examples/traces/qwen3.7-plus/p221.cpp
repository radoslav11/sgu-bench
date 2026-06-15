#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct BigInt {
    vector<uint32_t> digits;
    BigInt() {
        digits.push_back(0);
    }
    BigInt(uint64_t v) {
        if (v == 0) {
            digits.push_back(0);
        } else {
            while (v > 0) {
                digits.push_back(v % 1000000000);
                v /= 1000000000;
            }
        }
    }
    BigInt operator+(const BigInt& o) const {
        BigInt res;
        res.digits.clear();
        uint64_t carry = 0;
        size_t n = max(digits.size(), o.digits.size());
        for (size_t i = 0; i < n || carry; ++i) {
            uint64_t sum = carry;
            if (i < digits.size()) sum += digits[i];
            if (i < o.digits.size()) sum += o.digits[i];
            res.digits.push_back(sum % 1000000000);
            carry = sum / 1000000000;
        }
        if (res.digits.empty()) res.digits.push_back(0);
        return res;
    }
    BigInt operator*(uint32_t v) const {
        if (v == 0) return BigInt(0);
        BigInt res;
        res.digits.clear();
        uint64_t carry = 0;
        for (size_t i = 0; i < digits.size() || carry; ++i) {
            uint64_t prod = carry;
            if (i < digits.size()) prod += (uint64_t)digits[i] * v;
            res.digits.push_back(prod % 1000000000);
            carry = prod / 1000000000;
        }
        if (res.digits.empty()) res.digits.push_back(0);
        while (res.digits.size() > 1 && res.digits.back() == 0) {
            res.digits.pop_back();
        }
        return res;
    }
    BigInt operator*(const BigInt& o) const {
        if (digits.size() == 1 && digits[0] == 0) return BigInt(0);
        if (o.digits.size() == 1 && o.digits[0] == 0) return BigInt(0);
        BigInt res;
        res.digits.assign(digits.size() + o.digits.size(), 0);
        for (size_t i = 0; i < digits.size(); ++i) {
            uint64_t carry = 0;
            for (size_t j = 0; j < o.digits.size() || carry; ++j) {
                uint64_t cur = res.digits[i + j] + carry;
                if (j < o.digits.size()) {
                    cur += (uint64_t)digits[i] * o.digits[j];
                }
                res.digits[i + j] = cur % 1000000000;
                carry = cur / 1000000000;
            }
        }
        while (res.digits.size() > 1 && res.digits.back() == 0) {
            res.digits.pop_back();
        }
        return res;
    }
    void print() const {
        if (digits.empty()) {
            cout << 0 << "\n";
            return;
        }
        cout << digits.back();
        for (int i = (int)digits.size() - 2; i >= 0; --i) {
            cout << setfill('0') << setw(9) << digits[i];
        }
        cout << "\n";
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, k;
    if (!(cin >> n >> k)) return 0;

    vector<int> even_lengths, odd_lengths;
    for (int c = 0; c <= 2 * n - 2; ++c) {
        int len;
        if (c < n) len = c + 1;
        else len = 2 * n - 1 - c;
        
        if (c % 2 == 0) even_lengths.push_back(len);
        else odd_lengths.push_back(len);
    }
    
    sort(even_lengths.begin(), even_lengths.end());
    sort(odd_lengths.begin(), odd_lengths.end());

    auto solve = [&](const vector<int>& lengths) {
        int max_bishops = min(k, (int)lengths.size());
        vector<BigInt> dp(max_bishops + 1, BigInt(0));
        dp[0] = BigInt(1);
        for (int len : lengths) {
            vector<BigInt> new_dp = dp;
            for (int j = 1; j <= max_bishops; ++j) {
                if (len >= j) {
                    BigInt ways = dp[j - 1] * (len - j + 1);
                    new_dp[j] = new_dp[j] + ways;
                }
            }
            dp = new_dp;
        }
        return dp;
    };

    vector<BigInt> dp_even = solve(even_lengths);
    vector<BigInt> dp_odd = solve(odd_lengths);

    BigInt ans(0);
    for (int i = 0; i <= k; ++i) {
        int j = k - i;
        if (i < (int)dp_even.size() && j < (int)dp_odd.size()) {
            BigInt prod = dp_even[i] * dp_odd[j];
            ans = ans + prod;
        }
    }
    
    ans.print();
    return 0;
}
