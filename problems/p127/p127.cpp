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

map<char, int> cnt;
int n, k;

void read() {
    cin >> k >> n;
    for(int i = 0; i < n; i++) {
        string s;
        cin >> s;
        cnt[s[0]]++;
    }
}

void solve() {
    // The first two pages are fixed (title + instructions). Numbers are
    // sorted, and every number sharing a first digit forms one contiguous
    // block that must begin on a fresh page, so the blocks never share a
    // page. cnt[d] is how many numbers start with digit d, and that block
    // needs ceil(cnt[d] / k) pages. The answer is 2 plus the sum of those
    // per-digit page counts.

    int ans = 2;
    for(auto [c, x]: cnt) {
        ans += (x + k - 1) / k;
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
