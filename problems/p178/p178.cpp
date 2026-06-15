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

void solve() {
    // With b broken links we get b single loose links plus b + 1 intact
    // segments. The b loose links can make change for any value 1..b directly,
    // and the segments (chosen with the classic 1, 2, 4, ..., 2^b sizes) let us
    // cover everything up to (b + 1) * 2^b - 1 days, where the -1 accounts for
    // the loose links being part of the total. So we find the smallest b with
    // (b + 1) * 2^b - 1 >= n by incrementing b while the bound is still below.

    int64_t x = 2, ans = 0;
    while(x * (ans + 1) - 1 < n) {
        ans++;
        x *= 2;
    }

    cout << ans << '\n';
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
