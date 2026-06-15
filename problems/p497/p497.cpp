#include <bits/stdc++.h>

using namespace std;

template<typename T1, typename T2>
ostream& operator<<(ostream& out, const pair<T1, T2>& x) {
    return out << x.first << ' ' << x.second;
}

template<typename T1, typename T2>
istream& operator>>(istream& in, pair<T1, T2>& x) {
    return in >> x.first >> x.second;
}

template<typename T>
istream& operator>>(istream& in, vector<T>& a) {
    for(auto& x: a) {
        in >> x;
    }
    return in;
};

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
};

int64_t n;

void read() { cin >> n; }

vector<int64_t> compute_partitions(int max_n) {
    vector<int64_t> p(max_n + 1, 0);
    p[0] = 1;

    for(int n = 1; n <= max_n; ++n) {
        int64_t sum = 0;
        int k = 1;
        while(true) {
            int pent1 = (k * (3 * k - 1)) / 2;
            int pent2 = ((-k) * (3 * (-k) - 1)) / 2;
            if(pent1 > n && pent2 > n) {
                break;
            }

            int sign = (k % 2 == 1) ? 1 : -1;
            if(pent1 <= n) {
                sum += sign * p[n - pent1];
            }

            if(pent2 <= n && pent2 != pent1) {
                sum += sign * p[n - pent2];
            }

            ++k;
        }

        p[n] = sum;
    }

    return p;
}

void solve() {
    // By the fundamental theorem of finite abelian groups and the Chinese
    // remainder theorem, the number of abelian groups of order n is
    // multiplicative over the prime factorization: for a prime power p^e the
    // count of distinct decompositions equals the number of integer partitions
    // of e. So factorize n, and multiply together the partition counts of each
    // exponent. compute_partitions uses Euler's pentagonal number recurrence to
    // build the partition function up to 100 (the max exponent for n <= 10^12
    // is 39, since 2^40 > 10^12). The leftover factor after trial division up
    // to sqrt(n) is a single prime with exponent 1, contributing p(1) = 1.

    vector<int64_t> partitions = compute_partitions(100);
    int64_t result = 1;
    for(int64_t p = 2; p * p <= n; ++p) {
        if(n % p == 0) {
            int e = 0;
            while(n % p == 0) {
                e++;
                n /= p;
            }

            result *= partitions[e];
        }
    }

    if(n > 1) {
        result *= partitions[1];
    }

    cout << result << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
