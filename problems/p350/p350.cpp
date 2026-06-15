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

int m, n;
vector<int> b;

void read() {
    cin >> m;
    b.resize(m);
    cin >> b;
    n = (1 + (int)sqrt(1 + 8 * m)) / 2;
}

void solve() {
    // B holds all M = n(n-1)/2 pairwise XORs of the unknown set A. Since any
    // pairwise XOR is unchanged by XOR-ing every element of A with the same
    // constant, we may fix the first reconstructed element to 0.
    //
    // - present = the multiset of values that appear in B (any reconstructed
    //   pairwise XOR must be one of these).
    //
    // - ans = the elements we have committed to (starting with 0), and span =
    //   the set of XORs of all subsets of ans (its linear span over GF(2)),
    //   used to detect when a new value is XOR-dependent on chosen ones.
    //
    // We scan the B values in order. We accept a candidate value v only when
    // it is not already in span (otherwise it would create a zero-XOR subset)
    // and v ^ a is a value present in B for every a already in ans, i.e. the
    // new pairwise XORs it introduces are all consistent with B. On accepting
    // v we extend span by XOR-ing v into every current span element and append
    // v to ans.

    set<int> present;
    for(int x: b) {
        present.insert(x);
    }

    vector<int> ans = {0}, span = {0};
    set<int> in_span = {0};

    for(int i = 0; i < m; i++) {
        if(in_span.count(b[i])) {
            continue;
        }

        bool ok = true;
        for(int a: ans) {
            if(!present.count(b[i] ^ a)) {
                ok = false;
            }
        }

        if(ok) {
            int sz = span.size();
            for(int x = 0; x < sz; x++) {
                int val = b[i] ^ span[x];
                span.push_back(val);
                in_span.insert(val);
            }

            ans.push_back(b[i]);
        }
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
