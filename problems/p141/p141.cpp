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

int64_t x1v, x2v, p, k;

void read() { cin >> x1v >> x2v >> p >> k; }

int64_t extended_gcd(int64_t a, int64_t b, int64_t& x, int64_t& y) {
    if(b == 0) {
        x = 1;
        y = 0;
        return a;
    }

    int64_t x1, y1;
    int64_t g = extended_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

void solve() {
    // We need P1*x1 - N1*x1 + P2*x2 - N2*x2 = P with P1+N1+P2+N2 = K and all
    // four counts non-negative. Let a = P1 - N1 and b = P2 - N2; then
    // a*x1 + b*x2 = P, which is solvable via the extended Euclid only when
    // gcd(x1,x2) divides P. Among all solutions (shifting a by x2/g and b by
    // x1/g) we slide to the one minimising |a| + |b|, the smallest number of
    // jumps the net displacement forces.
    //
    // The leftover jumps last = K - (|a| + |b|) must be spent in
    // cancelling pairs (one positive plus one negative of the same length),
    // so last has to be non-negative and even. If it is odd we try shifting
    // by one (a +/- x2/g, b -/+ x1/g): this changes the parity of |a| + |b|
    // only when x1/g + x2/g is odd, otherwise no solution exists. Finally we
    // split each net count into positive and negative jumps and add half of
    // the leftover to a matching positive/negative pair.

    int64_t p1, p2;
    int64_t g = extended_gcd(x1v, x2v, p1, p2);

    if(p % g != 0) {
        cout << "NO\n";
        return;
    }

    int64_t dx = x2v / g;
    int64_t dy = x1v / g;
    p1 *= p / g;
    p2 *= p / g;

    while(llabs(p1 + dx) + llabs(p2 - dy) < llabs(p1) + llabs(p2)) {
        p1 += dx;
        p2 -= dy;
    }

    while(llabs(p1 - dx) + llabs(p2 + dy) < llabs(p1) + llabs(p2)) {
        p1 -= dx;
        p2 += dy;
    }

    if(llabs(p1) + llabs(p2) > k) {
        cout << "NO\n";
        return;
    }

    int64_t n1 = 0, n2 = 0;
    int64_t last = k - llabs(p1) - llabs(p2);
    if(last % 2 == 0) {
        if(p1 < 0) {
            n1 = -p1;
            p1 = 0;
        }

        if(p2 < 0) {
            n2 = -p2;
            p2 = 0;
        }

        p1 += last / 2;
        n1 += last / 2;
    } else {
        if((dx + dy) % 2 == 0) {
            cout << "NO\n";
            return;
        }

        if(llabs(p1 + dx) + llabs(p2 - dy) < llabs(p1 - dx) + llabs(p2 + dy)) {
            p1 += dx;
            p2 -= dy;
        } else {
            p1 -= dx;
            p2 += dy;
        }

        if(llabs(p1) + llabs(p2) > k) {
            cout << "NO\n";
            return;
        }

        last = k - llabs(p1) - llabs(p2);
        if(p1 < 0) {
            n1 = -p1;
            p1 = 0;
        }

        if(p2 < 0) {
            n2 = -p2;
            p2 = 0;
        }

        p1 += last / 2;
        n1 += last / 2;
    }

    cout << "YES\n";
    cout << p1 << ' ' << n1 << ' ' << p2 << ' ' << n2 << '\n';
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
