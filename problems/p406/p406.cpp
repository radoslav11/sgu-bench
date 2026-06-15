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

int n, m;
vector<vector<int>> seqs;
vector<set<int>> present;

void read() {
    cin >> n >> m;
    seqs.assign(n, {});
    present.assign(n, {});
    for(int i = 0; i < n; i++) {
        int k;
        cin >> k;
        seqs[i].resize(k);
        cin >> seqs[i];
        for(int x: seqs[i]) {
            present[i].insert(x);
        }
    }
}

void solve() {
    // Each database sequence stores its element multiset (as a vector to
    // preserve order) and a set of present values for O(log) membership tests.
    // A query lists wanted values (positive) and unwanted values (negative);
    // a sequence matches if it contains every positive value and none of the
    // values whose negation is given. We scan sequences in input order and
    // print the matching ones in their original format.

    while(m--) {
        int l;
        cin >> l;
        vector<int> query(l);
        cin >> query;
        vector<int> matched;
        for(int i = 0; i < n; i++) {
            bool ok = true;
            for(int q: query) {
                if(q > 0 && !present[i].count(q)) {
                    ok = false;
                } else if(q < 0 && present[i].count(-q)) {
                    ok = false;
                }
            }

            if(ok) {
                matched.push_back(i);
            }
        }

        cout << matched.size() << '\n';
        for(int idx: matched) {
            cout << seqs[idx].size() << ' ' << seqs[idx] << '\n';
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    read();
    solve();

    return 0;
}
