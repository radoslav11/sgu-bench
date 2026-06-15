#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

/**
 * Problem: Find the number of peaceful rook dispositions of k rooks on a given board.
 * A board is represented by row lengths b_1, b_2, ..., b_n.
 * A disposition is peaceful if no two rooks are in the same row or column.
 * 
 * DP approach:
 * Let the sorted row lengths be b_1 <= b_2 <= ... <= b_n.
 * Let dp[j] be the number of ways to place j rooks in the rows processed so far.
 * When considering the next row with length b_i, the new dp values are:
 * new_dp[j] = old_dp[j] + old_dp[j-1] * (b_i - (j - 1))
 * This formula ensures each rook in the current row occupies a column not used by the previous j-1 rooks.
 * Since b_i is sorted, any column used by previous rooks is also available in the current row.
 * 
 * BigInt is necessary as the result can be up to 250!, which is ~500 digits.
 */

const long long BASE = 1000000000;

struct BigInt {
    vector<long long> digits;

    BigInt(long long v = 0) {
        if (v == 0) {
            digits.push_back(0);
        } else {
            while (v > 0) {
                digits.push_back(v % BASE);
                v /= BASE;
            }
        }
    }

    void add(const BigInt& other) {
        if (other.digits.empty()) return;
        size_t n = max(digits.size(), other.digits.size());
        long long carry = 0;
        for (size_t i = 0; i < n || carry; ++i) {
            if (i == digits.size()) digits.push_back(0);
            long long other_digit = (i < other.digits.size()) ? other.digits[i] : 0;
            digits[i] += other_digit + carry;
            if (digits[i] >= BASE) {
                carry = digits[i] / BASE;
                digits[i] %= BASE;
            } else {
                carry = 0;
            }
        }
        while (digits.size() > 1 && digits.back() == 0) digits.pop_back();
    }

    void print() const {
        if (digits.empty()) {
            cout << 0 << endl;
            return;
        }
        cout << digits.back();
        for (int i = (int)digits.size() - 2; i >= 0; --i) {
            cout << setfill('0') << setw(9) << digits[i];
        }
        cout << endl;
    }
};

// Function to multiply a BigInt by a small integer v.
BigInt multiply(const BigInt& a, int v) {
    if (v == 0 || (a.digits.size() == 1 && a.digits[0] == 0)) return BigInt(0);
    BigInt res;
    res.digits.clear();
    long long carry = 0;
    for (size_t i = 0; i < a.digits.size() || carry; ++i) {
        long long cur = (i < a.digits.size() ? a.digits[i] : 0) * (long long)v + carry;
        res.digits.push_back(cur % BASE);
        carry = cur / BASE;
    }
    while (res.digits.size() > 1 && res.digits.back() == 0) res.digits.pop_back();
    return res;
}

int main() {
    // Faster I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    if (!(cin >> n >> k)) return 0;
    vector<int> b(n);
    for (int i = 0; i < n; ++i) cin >> b[i];
    
    // Sorting the row lengths helps to simplify the DP state transition.
    sort(b.begin(), b.end());

    // dp[j] stores the number of ways to place j rooks in the rows considered so far.
    vector<BigInt> dp(k + 1, BigInt(0));
    dp[0] = BigInt(1);

    for (int i = 0; i < n; ++i) {
        // Iterate j backwards to reuse the same dp array (standard 1D DP space optimization).
        for (int j = k; j >= 1; --j) {
            int multiplier = b[i] - (j - 1);
            if (multiplier > 0) {
                // dp[j] (new) = dp[j] (old) + dp[j-1] (old) * multiplier
                dp[j].add(multiply(dp[j-1], multiplier));
            }
        }
    }

    dp[k].print();

    return 0;
}

