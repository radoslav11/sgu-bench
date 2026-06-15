#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

typedef long long ll;
const ll BASE = 1e9;

struct BigInt {
    vector<ll> digits;
    bool negative = false;

    BigInt(ll v = 0) {
        if (v < 0) {
            negative = true;
            v = -v;
        }
        if (v == 0) digits.push_back(0);
        while (v > 0) {
            digits.push_back(v % BASE);
            v /= BASE;
        }
    }

    void trim() {
        while (digits.size() > 1 && digits.back() == 0) digits.pop_back();
        if (digits.empty()) {
            digits.push_back(0);
            negative = false;
        }
        if (digits.size() == 1 && digits[0] == 0) negative = false;
    }

    bool operator<(const BigInt& other) const {
        if (negative != other.negative) return negative;
        if (digits.size() != other.digits.size())
            return negative ? digits.size() > other.digits.size() : digits.size() < other.digits.size();
        for (int i = (int)digits.size() - 1; i >= 0; --i) {
            if (digits[i] != other.digits[i])
                return negative ? digits[i] > other.digits[i] : digits[i] < other.digits[i];
        }
        return false;
    }

    bool operator==(const BigInt& other) const {
        return negative == other.negative && digits == other.digits;
    }

    bool operator!=(const BigInt& other) const {
        return !(*this == other);
    }

    bool abs_less(const BigInt& other) const {
        if (digits.size() != other.digits.size())
            return digits.size() < other.digits.size();
        for (int i = (int)digits.size() - 1; i >= 0; --i) {
            if (digits[i] != other.digits[i])
                return digits[i] < other.digits[i];
        }
        return false;
    }

    BigInt operator-() const {
        if (*this == 0) return *this;
        BigInt res = *this;
        res.negative = !negative;
        return res;
    }

    BigInt operator+(const BigInt& other) const {
        if (negative == other.negative) {
            BigInt res;
            res.negative = negative;
            res.digits.clear();
            ll carry = 0;
            for (size_t i = 0; i < max(digits.size(), other.digits.size()) || carry; ++i) {
                ll sum = carry + (i < digits.size() ? digits[i] : 0) + (i < other.digits.size() ? other.digits[i] : 0);
                res.digits.push_back(sum % BASE);
                carry = sum / BASE;
            }
            res.trim();
            return res;
        } else {
            return *this - (-other);
        }
    }

    BigInt operator-(const BigInt& other) const {
        if (negative != other.negative) return *this + (-other);
        if (abs_less(other)) {
            BigInt res = other - *this;
            res.negative = !negative;
            res.trim();
            return res;
        }
        BigInt res;
        res.negative = negative;
        res.digits.clear();
        ll borrow = 0;
        for (size_t i = 0; i < digits.size(); ++i) {
            ll sub = digits[i] - borrow - (i < other.digits.size() ? other.digits[i] : 0);
            if (sub < 0) {
                sub += BASE;
                borrow = 1;
            } else {
                borrow = 0;
            }
            res.digits.push_back(sub);
        }
        res.trim();
        return res;
    }

    BigInt operator*(const BigInt& other) const {
        BigInt res;
        res.negative = negative != other.negative;
        res.digits.assign(digits.size() + other.digits.size(), 0);
        for (size_t i = 0; i < digits.size(); ++i) {
            ll carry = 0;
            for (size_t j = 0; j < other.digits.size() || carry; ++j) {
                ll cur = res.digits[i + j] + digits[i] * (j < other.digits.size() ? other.digits[j] : 0) + carry;
                res.digits[i + j] = cur % BASE;
                carry = cur / BASE;
            }
        }
        res.trim();
        return res;
    }

    void multiplyBASEAndAdd(ll val) {
        if (digits.size() == 1 && digits[0] == 0) {
            digits[0] = val;
        } else {
            digits.insert(digits.begin(), val);
        }
        trim();
    }

    pair<BigInt, BigInt> divmod(const BigInt& other) const {
        BigInt b = other;
        b.negative = false;
        BigInt a = *this;
        a.negative = false;
        if (a < b) return {0, *this};

        BigInt q, r;
        q.digits.assign(a.digits.size(), 0);
        for (int i = (int)a.digits.size() - 1; i >= 0; --i) {
            r.multiplyBASEAndAdd(a.digits[i]);
            ll low = 0, high = BASE - 1, best_q = 0;
            while (low <= high) {
                ll mid = low + (high - low) / 2;
                if (!(r < b * mid)) {
                    best_q = mid;
                    low = mid + 1;
                } else {
                    high = mid - 1;
                }
            }
            q.digits[i] = best_q;
            r = r - b * best_q;
        }
        q.negative = negative != other.negative;
        r.negative = negative;
        q.trim();
        r.trim();
        return {q, r};
    }

    BigInt operator/(const BigInt& other) const { return divmod(other).first; }
    BigInt operator%(const BigInt& other) const {
        BigInt res = divmod(other).second;
        if (res.negative) res = res + (other.negative ? -other : other);
        return res;
    }

    bool operator>(const BigInt& other) const { return other < *this; }
    bool operator>=(const BigInt& other) const { return !(*this < other); }
};

BigInt extendedGCD(BigInt a, BigInt b, BigInt &x, BigInt &y) {
    if (a == 0) {
        x = 0; y = 1;
        return b;
    }
    BigInt x1, y1;
    BigInt gcd = extendedGCD(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return gcd;
}

BigInt modInverse(BigInt a, BigInt m) {
    BigInt x, y;
    extendedGCD(a, m, x, y);
    return (x % m + m) % m;
}

BigInt power(BigInt base, int exp) {
    BigInt res(1);
    while (exp > 0) {
        if (exp % 2 == 1) res = res * base;
        base = base * base;
        exp /= 2;
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int b, n;
    if (!(cin >> b >> n)) return 0;

    vector<pair<int, int>> factors;
    int temp_b = b;
    for (int i = 2; i * i <= temp_b; ++i) {
        if (temp_b % i == 0) {
            int count = 0;
            while (temp_b % i == 0) {
                count++;
                temp_b /= i;
            }
            factors.push_back({i, count});
        }
    }
    if (temp_b > 1) factors.push_back({temp_b, 1});

    int k = (int)factors.size();
    vector<BigInt> M(k);
    for (int i = 0; i < k; ++i) {
        M[i] = power(BigInt(factors[i].first), n * factors[i].second);
    }

    vector<BigInt> solutions;
    for (int i = 0; i < (1 << k); ++i) {
        vector<BigInt> r(k);
        for (int j = 0; j < k; ++j) {
            if ((i >> j) & 1) r[j] = 1;
            else r[j] = 0;
        }

        BigInt x = r[0];
        BigInt mod = M[0];
        for (int j = 1; j < k; ++j) {
            BigInt target = (r[j] - x) % M[j];
            if (target.negative) target = target + M[j];
            BigInt inv = modInverse(mod % M[j], M[j]);
            BigInt k_val = (target * inv) % M[j];
            x = x + mod * k_val;
            mod = mod * M[j];
        }
        solutions.push_back(x);
    }

    BigInt b_pow_n_minus_1 = power(BigInt(b), n - 1);
    BigInt b_pow_n = power(BigInt(b), n);

    vector<BigInt> valid_solutions;
    for (const auto& sol : solutions) {
        if (sol >= b_pow_n_minus_1 && sol < b_pow_n) {
            valid_solutions.push_back(sol);
        }
    }

    cout << valid_solutions.size() << "\n";
    char digits_map[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (const auto& sol : valid_solutions) {
        string s = "";
        BigInt cur = sol;
        BigInt bigB(b);
        for (int j = 0; j < n; ++j) {
            pair<BigInt, BigInt> dm = cur.divmod(bigB);
            s += digits_map[dm.second.digits[0]];
            cur = dm.first;
        }
        reverse(s.begin(), s.end());
        cout << s << "\n";
    }

    return 0;
}

