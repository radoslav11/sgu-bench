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
vector<pair<int, int>> a;

void read() {
    cin >> n;
    a.resize(n);
    cin >> a;
}

void solve() {
    /*
     * Outpost i is redundant iff some j has A[j] < A[i] and B[i] < B[j]. Sort
     * the intervals by left endpoint A ascending; then while scanning we track
     * mx = the largest right endpoint B among all already-seen outposts (which
     * all have strictly smaller A). The current outpost is redundant exactly
     * when its B is below mx. All A and all B are distinct, so no ties matter.
     */

    sort(a.begin(), a.end());
    int answer = 0;
    int mx = INT_MIN;
    for(int i = 0; i < n; i++) {
        if(a[i].second < mx) {
            answer++;
        }

        mx = max(mx, a[i].second);
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
