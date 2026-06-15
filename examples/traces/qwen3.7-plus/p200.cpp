#include <iostream>
#include <vector>
#include <bitset>
#include <algorithm>

using namespace std;

vector<int> get_primes(int count) {
    vector<int> primes;
    for (int i = 2; primes.size() < count; i++) {
        bool is_p = true;
        for (int p : primes) {
            if (p * p > i) break;
            if (i % p == 0) {
                is_p = false;
                break;
            }
        }
        if (is_p) {
            primes.push_back(i);
        }
    }
    return primes;
}

struct BigInt {
    vector<int> digits;
    BigInt(long long v = 0) {
        if (v == 0) digits.push_back(0);
        while (v > 0) {
            digits.push_back(v % 10);
            v /= 10;
        }
    }
    BigInt multiply2() {
        BigInt res;
        res.digits.clear();
        int carry = 0;
        for (int d : digits) {
            int val = d * 2 + carry;
            res.digits.push_back(val % 10);
            carry = val / 10;
        }
        if (carry > 0) res.digits.push_back(carry);
        return res;
    }
    void subtract1() {
        int i = 0;
        while (i < digits.size() && digits[i] == 0) {
            digits[i] = 9;
            i++;
        }
        if (i < digits.size()) {
            digits[i]--;
        }
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
    }
    void print() {
        for (int i = (int)digits.size() - 1; i >= 0; i--) {
            cout << digits[i];
        }
        cout << "\n";
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t, m;
    if (!(cin >> t >> m)) return 0;

    vector<int> primes = get_primes(100);

    vector<bitset<128>> A(m);
    for (int i = 0; i < m; i++) {
        long long b;
        cin >> b;
        for (int j = 0; j < t; j++) {
            int p = primes[j];
            int count = 0;
            while (b % p == 0) {
                count++;
                b /= p;
            }
            if (count % 2 == 1) {
                A[i].set(j);
            }
        }
        if (b > 1) {
            cerr << "Warning: b_i has prime factors outside the first t primes.\n";
        }
    }

    int rank = 0;
    for (int col = 0; col < t; col++) {
        int pivot = -1;
        for (int row = rank; row < m; row++) {
            if (A[row].test(col)) {
                pivot = row;
                break;
            }
        }
        if (pivot != -1) {
            swap(A[rank], A[pivot]);
            for (int row = rank + 1; row < m; row++) {
                if (A[row].test(col)) {
                    A[row] ^= A[rank];
                }
            }
            rank++;
        }
    }

    int free_vars = m - rank;
    BigInt ans(1);
    for (int i = 0; i < free_vars; i++) {
        ans = ans.multiply2();
    }
    ans.subtract1();
    ans.print();

    return 0;
}
