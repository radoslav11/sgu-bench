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

int n, m;
string s, t;

void read() {
    cin >> s >> t;
    n = s.size();
    m = t.size();
}

void solve() {
    // Count pairs (left cut, right cut) so the message t is still a
    // subsequence of the remaining substring of the text s.
    //
    // nxt[i][c] = the first position >= i in s holding character c (or n if
    // none). For each starting position i we greedily match t through s
    // using nxt, landing at the position en of t's last matched character.
    // The smallest substring of s that starts at i and contains t as a
    // subsequence ends exactly at en, so every right end from en to n - 1
    // works: that is n - en valid right cuts. Summing n - en over all
    // starts i gives the total number of (start, end) windows, which is the
    // number of ways to trim the prefix and suffix.

    vector<array<int, 26>> nxt(n + 1);
    for(int c = 0; c < 26; c++) {
        nxt[n][c] = n;
    }

    for(int i = n - 1; i >= 0; i--) {
        for(int c = 0; c < 26; c++) {
            nxt[i][c] = nxt[i + 1][c];
        }
        nxt[i][s[i] - 'a'] = i;
    }

    int64_t answer = 0;
    for(int i = 0; i < n; i++) {
        int en = i - 1;
        for(char c: t) {
            en = nxt[en + 1][c - 'a'];
            if(en == n) {
                break;
            }
        }

        answer += n - en;
    }

    cout << answer << '\n';
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
