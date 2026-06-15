#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

const int BASE = 1e9;

struct BigInt {
    vector<int> d;
    
    BigInt() {}
    
    BigInt(long long v) {
        if (v == 0) d.push_back(0);
        while (v > 0) {
            d.push_back(v % BASE);
            v /= BASE;
        }
    }
    
    BigInt(string s) {
        for (int i = (int)s.length(); i > 0; i -= 9) {
            if (i < 9) {
                d.push_back(stoi(s.substr(0, i)));
            } else {
                d.push_back(stoi(s.substr(i - 9, 9)));
            }
        }
        trim();
    }
    
    void trim() {
        while (d.size() > 1 && d.back() == 0) {
            d.pop_back();
        }
        if (d.empty()) {
            d.push_back(0);
        }
    }
    
    bool operator<(const BigInt& o) const {
        if (d.size() != o.d.size()) return d.size() < o.d.size();
        for (int i = (int)d.size() - 1; i >= 0; i--) {
            if (d[i] != o.d[i]) return d[i] < o.d[i];
        }
        return false;
    }
    
    bool operator==(const BigInt& o) const {
        if (d.size() != o.d.size()) return false;
        for (size_t i = 0; i < d.size(); i++) {
            if (d[i] != o.d[i]) return false;
        }
        return true;
    }
    
    bool operator<=(const BigInt& o) const {
        return *this < o || *this == o;
    }
    
    BigInt operator*(const BigInt& o) const {
        BigInt res;
        res.d.assign(d.size() + o.d.size(), 0);
        for (size_t i = 0; i < d.size(); i++) {
            long long carry = 0;
            for (size_t j = 0; j < o.d.size() || carry; j++) {
                long long cur = res.d[i + j] + d[i] * 1LL * (j < o.d.size() ? o.d[j] : 0) + carry;
                res.d[i + j] = cur % BASE;
                carry = cur / BASE;
            }
        }
        res.trim();
        return res;
    }
    
    BigInt operator-(const BigInt& o) const {
        BigInt res = *this;
        int borrow = 0;
        for (size_t i = 0; i < o.d.size() || borrow; i++) {
            long long sub = borrow + (i < o.d.size() ? o.d[i] : 0);
            if (res.d[i] < sub)
