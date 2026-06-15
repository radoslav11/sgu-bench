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
vector<array<int, 3>> a;

void read() {
    cin >> n;
    a.resize(n);
    for(int i = 0; i < n; i++) {
        cin >> a[i][0] >> a[i][1];
        a[i][2] = i + 1;
    }
}

void solve() {
    // - We want the largest subset that can be totally ordered with both
    //   strength and beauty strictly increasing (any two members where one
    //   dominates strictly in both coexist; ties in either coordinate make them
    //   hate each other). This is a longest strictly-increasing chain in 2D.
    //
    // - Sort by strength ascending, breaking ties by beauty descending so that
    //   equal-strength members can never be picked together in a strictly
    //   increasing beauty run. Then a standard O(n log n) longest strictly
    //   increasing subsequence on the beauty values gives the answer length,
    //   with pos[i] recording the chain length ending at element i.
    //
    // - Reconstruct by scanning from the back, peeling off elements whose pos
    //   matches the current target length, then reversing to restore order and
    //   printing their original indices.

    sort(a.begin(), a.end(), [&](auto x, auto y) {
        if(x[0] != y[0]) {
            return x[0] < y[0];
        }
        return x[1] > y[1];
    });

    vector<int> lis;
    vector<int> pos(n, -1);
    for(int i = 0; i < n; i++) {
        auto it = lower_bound(lis.begin(), lis.end(), a[i][1]);
        pos[i] = it - lis.begin();
        if(it == lis.end()) {
            lis.push_back(a[i][1]);
        } else {
            *it = a[i][1];
        }
    }

    int len = lis.size();
    vector<int> ans;
    for(int i = n - 1; i >= 0; i--) {
        if(pos[i] == len - 1) {
            ans.push_back(a[i][2]);
            len--;
        }
    }

    reverse(ans.begin(), ans.end());
    cout << ans.size() << '\n';
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
