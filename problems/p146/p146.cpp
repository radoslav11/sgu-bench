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

const int64_t B = 10000;

int64_t L;
int N;
vector<pair<int64_t, int64_t>> a;

void read() {
    double _L;
    cin >> _L;
    L = (int64_t)(_L * B + 0.5);
    cin >> N;
    a.resize(N);
    cin >> a;
}

void solve() {
    // The total path length is the sum of T_i * V_i over the intervals; the
    // finishing position on the ring is that sum modulo L, and the answer is
    // the shorter arc, min(s, L - s). To stay in exact integers we scale all
    // lengths by B = 10000 (L has 4 decimals, T and V are integers), so every
    // distance is an integer number of ten-thousandths. We accumulate
    // T_i * V_i * B modulo L and finally divide back by B for output.

    int64_t s = 0;
    for(int i = 0; i < N; i++) {
        s += a[i].first * 1ll * a[i].second * B;
        s %= L;
    }

    s = min(s, L - s);
    cout << setprecision(4) << fixed << (s / (double)B) << endl;
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
