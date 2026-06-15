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

int n;

void read() { cin >> n; }

void solve() {
    /*
     * Connecting 2k contiguous points with k non-crossing chords minimizes the
     * number of regions, and a set of k chords splits the disk into exactly
     * k + 1 parts when no two chords cross. The count of non-crossing perfect
     * matchings of 2k points on a circle is the k-th Catalan number, built here
     * with the recurrence C[i] = sum_{j<i} C[j] * C[i-1-j]. So N = Catalan(k)
     * and P = k + 1.
     */

    vector<int64_t> C(n + 1, 0);
    C[0] = 1;
    for(int i = 1; i <= n; i++) {
        for(int j = 0; j < i; j++) {
            C[i] += C[j] * C[i - j - 1];
        }
    }

    cout << C[n] << " " << n + 1 << '\n';
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
