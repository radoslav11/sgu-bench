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

int day, month;

void read() { cin >> day >> month; }

void solve() {
    // 2001 is not a leap year and Jan 1, 2001 was a Monday. Validate the date
    // against the month lengths, then count the days elapsed since Jan 1 and
    // map that offset modulo 7 onto Monday..Sunday (1..7).

    vector<int> months = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if(month <= 0 || day <= 0 || month > 12 || day > months[month - 1]) {
        cout << "Impossible" << '\n';
        return;
    }

    int offset = day - 1;
    for(int i = 1; i < month; i++) {
        offset += months[i - 1];
    }

    cout << (offset % 7) + 1 << '\n';
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
