#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <algorithm>
#include <iomanip>

using namespace std;

struct BigInt {
    vector<uint32_t> digits;
    
    BigInt(uint64_t v = 0) {
        if (v == 0) digits.push_back(0);
        while (v > 0) {
            digits.push_back(v % 1000000000);
            v /= 1000000000;
        }
    }
    
    BigInt operator*(uint32_t v) const {
        if (v == 0) return BigInt(0);
        if (v == 1) return *this;
        BigInt res;
        res.digits.clear();
        res.digits.reserve(digits.size() + 2);
        uint64_t carry = 0;
        for (uint32_t d : digits) {
            uint64_t cur = (uint64_t)d * v + carry;
            res.digits.push_back(cur % 1000000000);
            carry = cur / 1000000000;
        }
        while (carry > 0) {
            res.digits.push_back(carry % 1000000000);
            carry /= 1000000000;
        }
        return res;
    }
    
    BigInt operator*(const BigInt& other) const {
        if (digits.empty() || other.digits.empty() || 
            (digits.size() == 1 && digits[0] == 0) || 
            (other.digits.size() == 1 && other.digits[0] == 0)) {
            return BigInt(0);
        }
        BigInt res;
        res.digits.assign(digits.size() + other.digits.size(), 0);
        for (size_t i = 0; i < digits.size(); ++i) {
            uint64_t carry = 0;
            uint64_t vi = digits[i];
            for (size_t j = 0; j < other.digits.size() || carry > 0; ++j) {
                uint64_t cur = res.digits[i + j] + vi * (j < other.digits.size() ? other.digits[j] : 0) + carry;
                res.digits[i + j] = cur % 1000000000;
                carry = cur / 1000000000;
            }
        }
        while (res.digits.size() > 1 && res.digits.back() == 0) {
            res.digits.pop_back();
        }
        return res;
    }
    
    bool operator<(const BigInt& other) const {
        if (digits.size() != other.digits.size())
            return digits.size() < other.digits.size();
        for (int i = (int)digits.size() - 1; i >= 0; --i) {
            if (digits[i] != other.digits[i])
                return digits[i] < other.digits[i];
        }
        return false;
    }
    
    void print() const {
        if (digits.empty()) {
            cout << 0;
            return;
        }
        cout << digits.back();
        for (int i = (int)digits.size() - 2; i >= 0; --i) {
            cout << setfill('0') << setw(9) << digits[i];
        }
    }
};

const int PRIMES[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};

BigInt power_bigint(uint32_t p, int exp) {
    if (exp == 0) return BigInt(1);
    BigInt res(1);
    BigInt base(p);
    while (exp > 0) {
        if (exp % 2 == 1) res = res * base;
        base = base * base;
        exp /= 2;
    }
    return res;
}

map<tuple<int, int, int>, pair<bool, BigInt>> memo;

pair<bool, BigInt> solve(int rem_K, int max_a, int p_idx) {
    if (rem_K == 1) {
        return {true, BigInt(1)};
    }
    if (p_idx >= 25) {
        return {false, BigInt(0)};
    }
    
    auto key = make_tuple(rem_K, max_a, p_idx);
    if (memo.count(key)) {
        return memo[key];
    }
    
    pair<bool, BigInt> best = {false, BigInt(0)};
    
    for (int d = 2; d <= max_a + 1; ++d) {
        if (rem_K % d == 0) {
            auto sub = solve(rem_K / d, d - 1, p_idx + 1);
            if (sub.first) {
                BigInt current = sub.second * power_bigint(PRIMES[p_idx], d - 1);
                if (!best.first || current < best.second) {
                    best = {true, current};
                }
            }
        }
    }
    
    return memo[key] = best;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int K;
    if (cin >> K) {
        auto res = solve(K, 100000, 0);
        if (res.first) {
            res.second.print();
            cout << "\n";
        } else {
            cout << 0 << "\n";
        }
    }
    
    return 0;
}
