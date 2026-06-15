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

const int MAXN = (1 << 20);

int n;
bool used[MAXN];

void read() {
    cin >> n;
    for(int i = 0; i < n; i++) {
        int x;
        cin >> x;
        used[x] = 1;
    }
}

void solve() {
    // used[x] marks which friendship numbers are present. The answer is the
    // largest g that divides at least two of the given numbers, since that g is
    // an achievable gcd of some pair. For each candidate divisor i we count how
    // many present numbers are multiples of i by scanning i, 2i, 3i, ...; if at
    // least two are present then i is a feasible gcd, and we keep the maximum
    // such i. This is a harmonic-sum sieve over the value range.

    int answer = 1;
    for(int i = 1; i < MAXN; i++) {
        int cnt = 0;
        for(int j = i; j < MAXN; j += i) {
            cnt += used[j];
        }

        if(cnt >= 2) {
            answer = max(answer, i);
        }
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
