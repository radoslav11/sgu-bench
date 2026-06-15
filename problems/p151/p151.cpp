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

double c, b, m;

void read() { cin >> c >> b >> m; }

void solve() {
    // The median length satisfies m^2 = (2b^2 + 2c^2 - a^2) / 4, so the third
    // side is a = sqrt(2b^2 + 2c^2 - 4m^2); if that radicand is negative or
    // the three sides a, b, c violate the triangle inequality there is no
    // triangle. Otherwise we place A at the origin and B at (c, 0) along the
    // x-axis. With the law of cosines for the angle at A, cos = (b^2 + c^2 -
    // a^2)/(2bc), point C lies at (b*cos, b*sin), giving a valid placement.

    double inner_val = 2 * b * b + 2 * c * c - 4 * m * m;
    if(inner_val < 0) {
        cout << "Mission impossible\n";
        return;
    }

    double a = sqrt(inner_val);
    if(a > b + c || b > a + c || c > a + b) {
        cout << "Mission impossible\n";
        return;
    }

    double cos_c = (b * b + c * c - a * a) / (2 * b * c);

    double bx = c, by = 0;
    double cx = b * cos_c;
    double cy = b * sqrt(1 - cos_c * cos_c);

    cout << fixed << setprecision(6);
    cout << 0.0 << ' ' << 0.0 << '\n';
    cout << bx << ' ' << by << '\n';
    cout << cx << ' ' << cy << '\n';
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
