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

int a, alpha, bt, gamma, m, k;

void read() { cin >> a >> alpha >> bt >> gamma >> m >> k; }

int f(int x) { return (alpha * x * x + bt * x + gamma) % m; }

void solve() {
    // x_i = f(x_{i-1}) mod M, so the orbit starting at x_0 = A mod M enters a
    // cycle within at most M steps. We iterate f recording, in period[], the
    // first step index at which each residue is seen. If we exhaust the k
    // steps before revisiting a residue, the current value is the answer.
    // Otherwise we hit a repeat: the cycle length is the difference between the
    // current step count and when this residue first appeared, so we reduce the
    // remaining k modulo that length and finish by iterating f.

    if(k == 0) {
        cout << a << '\n';
        return;
    }

    vector<int> period(m, -1);
    int pos = 0, x = a % m;

    while(k > 0 && period[x] == -1) {
        period[x] = pos++;
        x = f(x);
        k--;
    }

    if(k == 0) {
        cout << x << '\n';
    } else {
        int cycle = pos - period[x];
        k %= cycle;
        while(k--) {
            x = f(x);
        }

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
