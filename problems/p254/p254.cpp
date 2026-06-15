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

int n, q;
vector<int> nxt, prv;

void read() { cin >> n >> q; }

void solve() {
    // Direct simulation on a doubly linked list over the circle 1..N, where
    // nxt[i] / prv[i] are the clockwise / counter-clockwise neighbours.
    // Starting at 1 moving clockwise, each step walks Q positions in the
    // current direction to the number to erase, unlinks it, then advances one
    // step clockwise. The parity of the number landed on after that step sets
    // the direction for the next step (odd -> clockwise, even ->
    // counter-clockwise). We keep going until the list is empty and report the
    // last erased number.

    nxt.resize(n + 1);
    prv.resize(n + 1);
    for(int i = 1; i <= n; i++) {
        nxt[i] = (i == n) ? 1 : i + 1;
        prv[i] = (i == 1) ? n : i - 1;
    }

    int* nx = nxt.data();
    int* pv = prv.data();
    int current = 1;
    bool clockwise = true;
    int last_erased = -1;

    for(int remaining = n; remaining > 0; remaining--) {
        int pos = current;
        if(clockwise) {
            for(int i = 1; i < q; i++) {
                pos = nx[pos];
            }
        } else {
            for(int i = 1; i < q; i++) {
                pos = pv[pos];
            }
        }

        last_erased = pos;

        int next_pos = nx[pos];
        nx[pv[pos]] = next_pos;
        pv[next_pos] = pv[pos];

        current = next_pos;
        clockwise = (current & 1);
    }

    cout << last_erased << '\n';
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
