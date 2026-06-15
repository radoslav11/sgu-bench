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

int n, a, b;
string s;

void read() {
    cin >> n >> a >> b >> s;
}

void solve() {
    // We need exactly a zeros and b ones; everything else becomes a two.
    //
    // Subtract the zeros and ones already present from a and b, so a and b
    // become the signed deficits: a positive value means we still need that
    // many of the digit, a negative value means we have that many in excess.
    //
    // If a + b > n there is no way to fit the required zeros and ones, so the
    // answer is -1.
    //
    // A surplus zero (a < 0) can directly cover a needed one (b > 0) by a
    // single relabel, and vice versa; each such swap fixes two deficits with
    // one replacement, so we greedily apply those first. Whatever deficit
    // remains is settled against the twos: each leftover replacement turns a
    // surplus 0/1 into a 2 or a 2 into a needed 0/1. The total number of
    // replacements is the sum of the remaining absolute deficits plus the
    // swaps already made.

    if(a + b > n) {
        cout << -1 << '\n';
        return;
    }

    for(char c: s) {
        if(c == '0') {
            a--;
        } else if(c == '1') {
            b--;
        }
    }

    int answer = 0;
    if(a > 0 && b < 0) {
        int q = min(a, -b);
        answer += q;
        b += q;
        a -= q;
        for(char& c: s) {
            if(c == '1' && q) {
                c = '0';
                q--;
            }
        }
    }

    if(b > 0 && a < 0) {
        int q = min(-a, b);
        answer += q;
        b -= q;
        a += q;
        for(char& c: s) {
            if(c == '0' && q) {
                c = '1';
                q--;
            }
        }
    }

    answer += abs(a) + abs(b);

    for(char& c: s) {
        if(a < 0 && c == '0') {
            a++;
            c = '2';
        } else if(a > 0 && c == '2') {
            a--;
            c = '0';
        } else if(b < 0 && c == '1') {
            b++;
            c = '2';
        } else if(b > 0 && c == '2') {
            b--;
            c = '1';
        }
    }

    cout << answer << '\n';
    cout << s << '\n';
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
