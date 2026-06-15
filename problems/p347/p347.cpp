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
vector<string> s;

void read() {
    cin >> n;
    s.resize(n);
    cin >> s;
}

void solve() {
    // To get the lexicographically smallest concatenation we sort the
    // strings by the comparator a + b < b + a: a should come before b iff
    // appending b after a is smaller than the reverse. This pairwise rule
    // is a total order for concatenation, so sorting by it and printing the
    // strings in that order yields the global optimum.

    sort(s.begin(), s.end(), [](const string& a, const string& b) {
        return a + b < b + a;
    });

    for(auto& x: s) {
        cout << x;
    }
    cout << '\n';
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
