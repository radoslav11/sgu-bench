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

void read() { cin >> n; }

void solve() {
    // The four side faces of one die always sum to 14 (the six faces sum to 21
    // and the hidden top/bottom pair sums to 7). So a tower of h dice exposes
    // 14 * h dots on its sides, plus the very top face and the very bottom
    // face. For h >= 2 those two extreme faces are independent, each in 1..6,
    // contributing any s in [2, 12]; thus n = 14 * h + s with s = n % 14 forces
    // s to lie in [2, 12]. For h = 1 the top and bottom are opposite faces of
    // the same die and sum to exactly 7, so the only achievable total is 21.

    int num_dice = n / 14;
    int rem = n % 14;

    if(num_dice == 1 && n != 21) {
        cout << -1 << '\n';
    } else if(num_dice == 0 || rem <= 1 || rem == 13) {
        cout << -1 << '\n';
    } else {
        cout << num_dice << '\n';
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
