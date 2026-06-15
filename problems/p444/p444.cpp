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

int64_t x, y;

void read() {
    cin >> x >> y;
}

void solve() {
    // Simulate long division of x by y digit by digit. The state at each step
    // is the current remainder; the next remainder is (rem * 10) % y. A finite
    // expansion is reached when the remainder hits 0; a recurring part begins
    // when a remainder repeats.
    //
    // visited[rem] stores the digit index at which remainder rem was first
    // seen. When rem == 0 we stop with no recurring part (length 0). When a
    // remainder repeats, the cycle length is the difference between the current
    // digit index and the index where that remainder first appeared, and the
    // non-recurring prefix length is digits - cycle_length.

    vector<int64_t> visited(y, -1);
    int64_t rem = x % y;
    int64_t digits = 0;
    while(rem != 0 && visited[rem] == -1) {
        visited[rem] = digits;
        rem = (rem * 10) % y;
        digits++;
    }

    int64_t non_recurring, cycle_length;
    if(rem == 0) {
        non_recurring = digits;
        cycle_length = 0;
    } else {
        cycle_length = digits - visited[rem];
        non_recurring = digits - cycle_length;
    }

    cout << non_recurring << ' ' << cycle_length << '\n';
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
