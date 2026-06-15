#include <bits/stdc++.h>

using namespace std;

template<typename T1, typename T2>
ostream& operator<<(ostream &out, const pair<T1, T2>& x) {
    return out << x.first << ' ' << x.second;
}

template<typename T1, typename T2>
istream& operator>>(istream &in, pair<T1, T2>& x) {
    return in >> x.first >> x.second;
}

template<typename T>
istream& operator>>(istream &in, vector<T>& a) {
    for(auto& x: a) {
        in >> x;
    }
    return in;
};

template<typename T>
ostream& operator<<(ostream &out, const vector<T>& a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
};

double X, Y, Z;

void read() { cin >> X >> Y >> Z; }

void solve() {
    // Both arrival times are uniform over a window of delta = (Y - X) * 60
    // minutes, so the sample space is a delta x delta square. They meet when
    // their arrival times differ by at most Z, the diagonal band
    // |t1 - t2| <= Z. Its area is delta^2 minus the two corner triangles of
    // total area (delta - Z)^2, and we report that area divided by delta^2,
    // written here as Z^2/delta^2 + 2*(delta - Z)*Z/delta^2.

    double delta = (Y - X) * 60;
    double ans = (Z * Z) / (delta * delta);
    ans += 2 * (delta - Z) * Z / (delta * delta);
    cout << fixed << setprecision(7) << ans << '\n';
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
