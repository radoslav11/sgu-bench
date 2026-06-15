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

double start_time, arrival;

void read() { cin >> start_time >> arrival; }

void solve() {
    // Map the lateness (arrival minus start, in minutes) to cups of tea:
    // on time owes 0, any positive delay owes 1, 5+ minutes owes 2, 15+ owes 3
    // and 30+ owes 4. Check the thresholds from the largest downward.

    double delay = (arrival - start_time) / 60.0;

    int ans = 4;
    if(delay < 30) {
        ans = 3;
    }
    if(delay < 15) {
        ans = 2;
    }
    if(delay < 5) {
        ans = 1;
    }
    if(delay <= 0) {
        ans = 0;
    }

    cout << ans << "\n";
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
