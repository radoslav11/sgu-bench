#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
using namespace std;

vector<int> add(const vector<int>& a, const vector<int>& b) {
    vector<int> c;
    int carry = 0;
    int n = max(a.size(), b.size());
    for (int i = 0; i < n; i++) {
        if (i < a.size()) carry += a[i];
        if (i < b.size()) carry += b[i];
        c.push_back(carry % 10);
        carry /= 10;
    }
    if (carry) c.push_back(carry);
    return c;
}

vector<int> multiply(const vector<int>& a, int b) {
    vector<int> c;
    int carry = 0;
    for (int i = 0; i < a.size(); i++) {
        carry += a[i] * b;
        c.push_back(carry % 10);
        carry /= 10;
    }
    while (carry) {
        c.push_back(carry % 10);
        carry /= 10;
    }
    return c;
}

vector<int> divide_vector_by_int(const vector<int>& a, int b) {
    vector<int> c;
    int carry = 0;
    for (int i = a.size() - 1; i >= 0; i--) {
        carry = carry * 10 + a[i];
        int q = carry / b;
        carry = carry % b;
        if (q > 0 || !c.empty()) {
            c.push_back(q);
        }
    }
    if (c.empty()) {
        c.push_back(0);
    }
    reverse(c.begin(), c.end());
    return c;
}

bool is_divisible_by_int(const vector<int>& a, int b) {
    int remainder = 0;
    for (int i = a.size() - 1; i >= 0; i--) {
        remainder = (remainder * 10 + a[i]) % b;
    }
    return remainder == 0;
}

string vector_to_string(const vector<int>& a) {
    string s;
    for (int i = a.size() - 1; i >= 0; i--) {
        s += char('0' + a[i]);
    }
    return s;
}

int exponent_in_factorial(int n, int p) {
    int exp = 0;
    long long power = p;
    while (power <= n) {
        exp += n / power;
        power *= p;
    }
    return exp;
}

vector<int> compute_derangement(int m) {
    if (m == 0) {
        return {1};
    }
    if (m == 1) {
        return {0};
    }
    vector<int> D0 = {1};
    vector<int> D1 = {0};
    for (int i = 2; i <= m; i++) {
        vector<int> temp = add(D1, D0);
        temp = multiply(temp, i - 1);
        D0 = D1;
        D1 = temp;
    }
    return D1;
}

int main() {
    int K, N;
    cin >> K >> N;
    int m = N - K;

    // Special case: if m == 1, D(1)=0
    if (m == 1) {
        cout << "0" << endl;
        return 0;
    }

    vector<int> D_val = compute_derangement(m);

    // Check if D_val is zero
    if (D_val.size() == 1 && D_val[0] == 0) {
        cout << "0" << endl;
        return 0;
    }

    // List of primes up to 100
    vector<int> primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};

    // Compute exponent for denominator = fact[K] * fact[m]
    vector<int> exp_den_vec;
    for (int p : primes) {
        int exp_den = exponent_in_factorial(K, p) + exponent_in_factorial(m, p);
        exp_den_vec.push_back(exp_den);
    }

    // For each prime, compute exponent in D_val
    vector<int> exp_num_vec(primes.size(), 0);
    vector<int> current = D_val;
    for (int i = 0; i < primes.size(); i++) {
        int p = primes[i];
        while (is_divisible_by_int(current, p)) {
            exp_num_vec[i]++;
            current = divide_vector_by_int(current, p);
        }
    }

    // Compute remaining exponents after reducing common factors
    vector<int> exp_num_remaining(primes.size(), 0);
    vector<int> exp_den_remaining(primes.size(), 0);
    for (int i = 0; i < primes.size(); i++) {
        int common = min(exp_num_vec[i], exp_den_vec[i]);
        exp_num_remaining[i] = exp_num_vec[i] - common;
        exp_den_remaining[i] = exp_den_vec[i] - common;
    }

    // Compute reduced numerator: current * (product of primes[i]^exp_num_remaining[i])
    vector<int> reduced_num = current;
    for (int i = 0; i < primes.size(); i++) {
        for (int j = 0; j < exp_num_remaining[i]; j++) {
            reduced_num = multiply(reduced_num, primes[i]);
        }
    }

    // Compute reduced denominator: 1 * (product of primes[i]^exp_den_remaining[i])
    vector<int> reduced_den = {1};
    for (int i = 0; i < primes.size(); i++) {
        for (int j = 0; j < exp_den_remaining[i]; j++) {
            reduced_den = multiply(reduced_den, primes[i]);
        }
    }

    cout << vector_to_string(reduced_num) << "/" << vector_to_string(reduced_den) << endl;

    return 0;
}
