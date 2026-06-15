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

int n, k;

void read() { cin >> n >> k; }

void solve() {
    // Placing k non-attacking rooks means choosing k of the n rows, k of the n
    // columns, and a bijection between them: C(n, k) * C(n, k) * k! ways. We
    // compute this as (n!)^2 / (k! * ((n-k)!)^2), accumulating n! squared in
    // the numerator and dividing out the k! and (n-k)!^2 factors. If k > n no
    // placement is possible, so the answer is 0.

    if(n < k) {
        cout << 0 << '\n';
        return;
    }

    int64_t answer = 1;
    for(int i = 1; i <= n; i++) {
        answer *= i * i;
    }

    for(int i = 1; i <= k; i++) {
        answer /= i;
    }

    for(int i = 1; i <= n - k; i++) {
        answer /= i * i;
    }

    cout << answer << '\n';
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
