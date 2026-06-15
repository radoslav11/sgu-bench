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

vector<vector<int>> a;

void read() {
    a.assign(3, vector<int>(3));
    for(int i = 0; i < 3; i++) {
        cin >> a[i];
    }
}

void solve() {
    // Sort each of the three triples, take the middle element as that triple's
    // median, then sort those three medians and print the middle one.

    vector<int> medians;
    for(int i = 0; i < 3; i++) {
        sort(a[i].begin(), a[i].end());
        medians.push_back(a[i][1]);
    }

    sort(medians.begin(), medians.end());
    cout << medians[1] << '\n';
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
