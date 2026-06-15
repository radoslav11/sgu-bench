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
vector<vector<short>> a;

void read() {
    cin >> n >> m;
    a.assign(n, vector<short>(m));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            cin >> a[i][j];
        }
    }
}

void solve() {
    // B[i][j] = min{ A[x][y] : y >= j and x >= i + j - y }, i.e. the minimum
    // over the lower-left quadrant relative to the anti-diagonal through
    // (i, j). Reindex by column y and anti-diagonal d = x + y: cell A[x][y]
    // lands at q[y][x + y]. The constraints y >= j and x + y >= i + j become
    // "row index >= j and diagonal index >= i + j", so each B value is a
    // suffix minimum of q over both indices. We compute that suffix minimum
    // with a backward sweep (each cell takes the min of itself, the cell
    // below and the cell to the right), then read B[i][j] back from
    // q[j][i + j].

    const short INF = numeric_limits<short>::max();
    vector<vector<short>> q(m, vector<short>(n + m, INF));

    for(int x = 0; x < n; x++) {
        for(int y = 0; y < m; y++) {
            q[y][x + y] = a[x][y];
        }
    }

    for(int i = m - 1; i >= 0; i--) {
        for(int j = n + m - 1; j >= 0; j--) {
            short cur = q[i][j];
            short down = (i + 1 < m) ? q[i + 1][j] : INF;
            short right = (j + 1 < n + m) ? q[i][j + 1] : INF;
            q[i][j] = min({cur, down, right});
        }
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            cout << q[j][i + j] << (j == m - 1 ? '\n' : ' ');
        }
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
