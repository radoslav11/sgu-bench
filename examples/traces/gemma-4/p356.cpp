#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// A BigInt class to handle large numbers for N up to 100.
struct BigInt {
    vector<int> digits;
    static const int BASE = 10;

    BigInt(long long v = 0) {
        if (v == 0) digits.push_back(0);
        while (v > 0) {
            digits.push_back(v % BASE);
            v /= BASE;
        }
    }

    void add(const BigInt& other) {
        int carry = 0;
        for (size_t i = 0; i < max(digits.size(), other.digits.size()) || carry; ++i) {
            if (i == digits.size()) digits.push_back(0);
            int other_digit = (i < other.digits.size()) ? other.digits[i] : 0;
            int sum = digits[i] + other_digit + carry;
            digits[i] = sum % BASE;
            carry = sum / BASE;
        }
    }

    void sub(const BigInt& other) {
        int borrow = 0;
        for (size_t i = 0; i < other.digits.size() || borrow; ++i) {
            int other_digit = (i < other.digits.size()) ? other.digits[i] : 0;
            int sub_val = digits[i] - other_digit - borrow;
            if (sub_val < 0) {
                sub_val += BASE;
                borrow = 1;
            } else {
                borrow = 0;
            }
            digits[i] = sub_val;
        }
        while (digits.size() > 1 && digits.back() == 0) digits.pop_back();
    }

    void mul(int v) {
        if (v == 0) {
            digits = {0};
            return;
        }
        int carry = 0;
        for (size_t i = 0; i < digits.size() || carry; ++i) {
            if (i == digits.size()) digits.push_back(0);
            long long cur = (long long)digits[i] * v + carry;
            digits[i] = cur % BASE;
            carry = cur / BASE;
        }
        while (digits.size() > 1 && digits.back() == 0) digits.pop_back();
    }

    void mul(const BigInt& other) {
        if ((digits.size() == 1 && digits[0] == 0) || (other.digits.size() == 1 && other.digits[0] == 0)) {
            digits = {0};
            return;
        }
        BigInt res;
        res.digits.assign(digits.size() + other.digits.size(), 0);
        for (size_t i = 0; i < digits.size(); ++i) {
            int carry = 0;
            for (size_t j = 0; j < other.digits.size() || carry; ++j) {
                int other_digit = (j < other.digits.size()) ? other.digits[j] : 0;
                long long cur = (long long)res.digits[i + j] + (long long)digits[i] * other_digit + carry;
                res.digits[i + j] = cur % BASE;
                carry = cur / BASE;
            }
        }
        while (res.digits.size() > 1 && res.digits.back() == 0) res.digits.pop_back();
        *this = res;
    }

    bool operator<(const BigInt& other) const {
        if (digits.size() != other.digits.size()) return digits.size() < other.digits.size();
        for (int i = (int)digits.size() - 1; i >= 0; --i) {
            if (digits[i] != other.digits[i]) return digits[i] < other.digits[i];
        }
        return false;
    }

    bool operator>(const BigInt& other) const { return other < *this; }
    bool operator==(const BigInt& other) const { return digits == other.digits; }
    bool operator!=(const BigInt& other) const { return !(*this == other); }

    void mulBase() {
        if (digits.size() == 1 && digits[0] == 0) return;
        digits.insert(digits.begin(), 0);
    }

    void addInt(int v) {
        int carry = v;
        for (size_t i = 0; i < digits.size() && carry; ++i) {
            int sum = digits[i] + carry;
            digits[i] = sum % BASE;
            carry = sum / BASE;
        }
        while (carry) {
            digits.push_back(carry % BASE);
            carry /= BASE;
        }
    }

    static pair<BigInt, BigInt> divmod(BigInt a, BigInt b) {
        BigInt q(0), r(0);
        q.digits.assign(a.digits.size(), 0);
        for (int i = (int)a.digits.size() - 1; i >= 0; --i) {
            r.mulBase();
            r.addInt(a.digits[i]);
            int d = 0;
            int low = 0, high = BASE - 1;
            while (low <= high) {
                int mid = (low + high) / 2;
                BigInt temp = b;
                temp.mul(mid);
                if (!(r < temp)) {
                    d = mid;
                    low = mid + 1;
                } else {
                    high = mid - 1;
                }
            }
            q.digits[i] = d;
            BigInt sub_val = b;
            sub_val.mul(d);
            r.sub(sub_val);
        }
        while (q.digits.size() > 1 && q.digits.back() == 0) q.digits.pop_back();
        while (r.digits.size() > 1 && r.digits.back() == 0) r.digits.pop_back();
        return {q, r};
    }

    void print() const {
        for (int i = (int)digits.size() - 1; i >= 0; --i) {
            cout << digits[i];
        }
    }
};

BigInt gcd(BigInt a, BigInt b) {
    while (b != BigInt(0)) {
        a = BigInt::divmod(a, b).second;
        swap(a, b);
    }
    return a;
}

int main() {
    int K, N;
    if (!(cin >> K >> N)) return 0;

    if (K > N || (K == N - 1 && N > 0)) {
        cout << 0 << endl;
        return 0;
    }

    // Precompute derangements D(m)
    vector<BigInt> D(N + 1);
    D[0] = BigInt(1);
    if (N >= 1) D[1] = BigInt(0);
    for (int i = 2; i <= N; ++i) {
        D[i] = D[i - 1];
        D[i].mul(i - 1);
        BigInt sum = D[i - 2];
        sum.mul(i - 1);
        D[i].add(sum);
    }

    // Precompute combinations C(n, k) using Pascal's triangle
    vector<vector<BigInt>> C(N + 1, vector<BigInt>(N + 1, BigInt(0)));
    for (int i = 0; i <= N; ++i) {
        C[i][0] = BigInt(1);
        for (int j = 1; j <= i; ++j) {
            C[i][j] = C[i - 1][j - 1];
            C[i][j].add(C[i - 1][j]);
        }
    }

    // Precompute factorials N!
    BigInt factN(1);
    for (int i = 1; i <= N; ++i) {
        factN.mul(i);
    }

    // Number of favorable outcomes: A = C(N, K) * D(N-K)
    BigInt A = C[N][K];
    A.mul(D[N - K]);

    // Total number of outcomes: B = N!
    BigInt B = factN;

    if (A == BigInt(0)) {
        cout << 0 << endl;
    } else {
        BigInt common = gcd(A, B);
        BigInt finalA = BigInt::divmod(A, common).first;
        BigInt finalB = BigInt::divmod(B, common).first;
        finalA.print();
        cout << "/";
        finalB.print();
        cout << endl;
    }

    return 0;
}

