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
vector<tuple<int, int, int>> a;

void read() {
    cin >> n;
    a.resize(n);
    for(auto& [x, y, z]: a) {
        cin >> x >> y >> z;
    }
}

void solve() {
    // We pick a single top level to start the chain reaction by paying to
    // depressurize it: its water W pours onto the next level, and from there
    // on the falling water w_fall accumulates downward. At each level, if the
    // accumulated water already exceeds the level's capacity L it breaks for
    // free and we keep falling; otherwise we must pay P to break it (and add
    // its own W to the cascade). The total cost of a given start is the sum of
    // the P of the levels we had to pay for; once w_fall exceeds the maximum
    // possible capacity (15000) the rest collapses for free, so we stop early.
    // We try every start, keep the cheapest, and replay it to list which
    // levels we ended up paying to depressurize.

    int ans = get<2>(a[n - 1]);
    int best_pos = n - 1;

    for(int start = 0; start < n; start++) {
        int w_fall = 0, candidate = 0;
        for(int i = start; i < n; i++) {
            auto [x, y, z] = a[i];
            w_fall += x;
            if(w_fall <= y) {
                candidate += z;
            } else if(w_fall > 15000) {
                break;
            }
        }

        if(candidate < ans) {
            ans = candidate;
            best_pos = start;
        }
    }

    int w_fall = 0;
    vector<int> best;
    for(int i = best_pos; i < n; i++) {
        auto [x, y, z] = a[i];
        w_fall += x;
        if(w_fall <= y) {
            best.push_back(i + 1);
        }
    }

    for(auto x: best) {
        cout << x << '\n';
    }
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
