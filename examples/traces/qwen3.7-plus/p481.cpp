#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cstdint>

using namespace std;

struct BigInt {
    vector<uint32_t> digits;
    
    BigInt() {}
    BigInt(uint64_t v) {
        if (v == 0) digits.push_back(0);
        while (v > 0) {
            digits.push_back(v % 1000000000);
            v /= 1000000000;
        }
    }
    
    void multiply(uint32_t v) {
        uint64_t carry = 0;
        for (size_t i = 0; i < digits.size(); ++i) {
            uint64_t cur = (uint64_t)digits[i] * v + carry;
            digits[i] = cur % 1000000000;
            carry = cur / 1000000000;
        }
        while (carry > 0) {
            digits.push_back(carry % 1000000000);
            carry /= 1000000000;
        }
    }
    
    void divide(uint32_t v) {
        uint64_t rem = 0;
        for (int i = (int)digits.size() - 1; i >= 0; --i) {
            uint64_t cur = rem * 1000000000 + digits[i];
            digits[i] = cur / v;
            rem = cur % v;
        }
        remove_leading_zeros();
    }
    
    void add(const BigInt& other) {
        uint64_t carry = 0;
        size_t n = std::max(digits.size(), other.digits.size());
        digits.resize(n, 0);
        for (size_t i = 0; i < n; ++i) {
            uint64_t cur = digits[i] + (i < other.digits.size() ? other.digits[i] : 0) + carry;
            digits[i] = cur % 1000000000;
            carry = cur / 1000000000;
        }
        if (carry > 0) {
            digits.push_back(carry);
        }
    }
    
    void remove_leading_zeros() {
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
        if (digits.empty()) {
            digits.push_back(0);
        }
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
        cout << "\n";
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    uint32_t n;
    if (!(cin >> n)) return 0;
    
    if (n < 3) {
        cout << 0 << "\n";
        return 0;
    }
    
    BigInt C0(1);
    C0.digits.reserve(3000);
    for (uint32_t i = 2; i <= n - 1; ++i) {
        C0.multiply(i);
    }
    C0.divide(2);
    
    BigInt Ans = C0;
    Ans.digits.reserve(3000);
    
    for (uint32_t j = 1; j <= n - 3; ++j) {
        C0.multiply(n);
        C0.divide(j);
        Ans.add(C0);
    }
    
    Ans.print();
    
    return 0;
}
