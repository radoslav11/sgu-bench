#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <unordered_set>
#include <array>
#include <string>
#include <cmath>

using namespace std;

struct BigInt {
    bool neg;
    vector<uint32_t> d;
    
    BigInt() : neg(false) { d.push_back(0); }
    BigInt(long long v) {
        if (v < 0) { neg = true; v = -v; }
        else neg = false;
        if (v == 0) d.push_back(0);
        while (v > 0) {
            d.push_back(v % 1000000000);
            v /= 1000000000;
        }
    }
    
    void mul(int v) {
        if (v < 0) { neg = !neg; v = -v; }
        if (v == 0) { d = {0}; neg = false; return; }
        uint64_t carry = 0;
        for (size_t i = 0; i < d.size(); ++i) {
            uint64_t cur = (uint64_t)d[i] * v + carry;
            d[i] = cur % 1000000000;
            carry = cur / 1000000000;
        }
        while (carry > 0) {
            d.push_back(carry % 1000000000);
            carry /= 1000000000;
        }
        trim();
    }
    
    void trim() {
        while (d.size() > 1 && d.back() == 0) d.pop_back();
        if (d.size() == 1 && d[0] == 0) neg = false;
    }
    
    bool operator<(const BigInt& o) const {
        if (neg != o.neg) return neg;
        if (d.size() != o.d.size()) {
            return neg ? (d.size() > o.d.size()) : (d.size() < o.d.size());
        }
        for (int i = (int)d.size() - 1; i >= 0; --i) {
            if (d[i] != o.d[i]) {
                return neg ? (d[i] > o.d[i]) : (d[i] < o.d[i]);
            }
        }
        return false;
    }
    
    bool operator==(const BigInt& o) const {
        return neg == o.neg && d == o.d;
    }
    
    string to_string() const {
        if (d.empty() || (d.size() == 1 && d[0] == 0)) return "0";
        string res = (neg ? "-" : "");
        res += std::to_string(d.back());
        for (int i = (int)d.size() - 2; i >= 0; --i) {
            string s = std::to_string(d[i]);
            res += string(9 - s.length(), '0') + s;
        }
        return res;
    }
};

struct State {
    BigInt val;
    array<int, 13> a_idx, b_idx;
    bool operator<(const State& o) const {
        return val < o.val;
    }
    bool operator==(const State& o) const {
        return a_idx == o.a_idx && b_idx == o.b_idx && val == o.val;
    }
};

struct StateHash {
    size_t operator()(const State& s) const {
        size_t h = 0;
        for (int i = 0; i < 13; ++i) {
            h ^= hash<int>()(s.a_idx[i]) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= hash<int>()(s.b_idx[i]) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }
};

BigInt compute_val(const array<int, 13>& a_idx, const array<int, 13>& b_idx, int p, int c, const vector<int>& A, const vector<int>& B) {
    BigInt res(1);
    for (int i = 0; i < p; ++i) res.mul(A[a_idx[i]]);
    for (int i = 0; i < c; ++i) res.mul(B[b_idx[i]]);
    return res;
}

void generate(const vector<int>& A, const vector<int>& B, int p, int c, vector<BigInt>& results, int k) {
    if (p > (int)A.size() || c > (int)B.size()) return;
    
    State start;
    for (int i = 0; i < 13; ++i) {
        start.a_idx[i] = i < p ? i : 0;
        start.b_idx[i] = i < c ? i : 0;
    }
    start.val = compute_val(start.a_idx, start.b_idx, p, c, A, B);
    
    priority_queue<State> pq;
    unordered_set<State, StateHash> visited;
    
    pq.push(start);
    visited.insert(start);
    
    while (!pq.empty() && results.size() < (size_t)k) {
        State curr = pq.top();
        pq.pop();
        results.push_back(curr.val);
        
        for (int j = 0; j < p; ++j) {
            int bound = (j == p - 1) ? (int)A.size() : curr.a_idx[j + 1];
            if (curr.a_idx[j] + 1 < bound) {
                State next_st = curr;
                next_st.a_idx[j]++;
                if (visited.find(next_st) == visited.end()) {
                    visited.insert(next_st);
                    next_st.val = compute_val(next_st.a_idx, next_st.b_idx, p, c, A, B);
                    pq.push(next_st);
                }
            }
        }
        for (int j = 0; j < c; ++j) {
            int bound = (j == c - 1) ? (int)B.size() : curr.b_idx[j + 1];
            if (curr.b_idx[j] + 1 < bound) {
                State next_st = curr;
                next_st.b_idx[j]++;
                if (visited.find(next_st) == visited.end()) {
                    visited.insert(next_st);
                    next_st.val = compute_val(next_st.a_idx, next_st.b_idx, p, c, A, B);
                    pq.push(next_st);
                }
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m, k;
    if (!(cin >> n >> m >> k)) return 0;
    
    vector<int> a(n);
    vector<int> pos, neg;
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
        if (a[i] >= 0) {
            pos.push_back(a[i]);
        } else {
            neg.push_back(a[i]);
        }
    }
    
    sort(pos.begin(), pos.end(), greater<int>());
    sort(neg.begin(), neg.end(), [](int x, int y) {
        return abs(x) > abs(y);
    });
    
    vector<BigInt> all_results;
    
    for (int c = 0; c <= m; c += 2) {
        int p = m - c;
        generate(pos, neg, p, c, all_results, k);
    }
    
    vector<int> pos_strict;
    for (int x : pos) {
        if (x > 0) pos_strict.push_back(x);
    }
    
    for (int c = 1; c <= m; c += 2) {
        int p = m - c;
        vector<int> A_odd = pos_strict;
        reverse(A_odd.begin(), A_odd.end());
        vector<int> B_odd = neg;
        reverse(B_odd.begin(), B_odd.end());
        generate(A_odd, B_odd, p, c, all_results, k);
    }
    
    sort(all_results.begin(), all_results.end(), [](const BigInt& x, const BigInt& y) {
        return y < x;
    });
    
    if (k <= (int)all_results.size()) {
        cout << all_results[k - 1].to_string() << "\n";
    } else if (!all_results.empty()) {
        cout << all_results.back().to_string() << "\n";
    }
    
    return 0;
}
