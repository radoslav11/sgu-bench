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
vector<string> grid;

void read() {
    cin >> n >> m;
    grid.resize(n);
    cin >> grid;
}

void solve() {
    // Simulate the bottle row by row. Find P, then for each row below track
    // the current column pos. A '\' deflects right (pos+1) and a '/' deflects
    // left (pos-1); if the deflection lands on an obstacle of the opposite
    // type the bottle stops (type change), encoded as pos = -1. Going off
    // either side (wall) also stops the bottle, encoded by setting pos = -2
    // and breaking. If the bottle falls past the last row pos still holds the
    // exit column; we print pos + 1, which becomes -1 when the bottle stopped.

    int x = 0, y = 0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(grid[i][j] == 'P') {
                x = i;
                y = j;
            }
        }
    }

    int pos = y;
    int i = x + 1;
    while(i < n) {
        if(grid[i][pos] == '\\') {
            pos += 1;
            if(pos < m && grid[i][pos] == '/') {
                pos = -1;
            }
        } else if(grid[i][pos] == '/') {
            pos -= 1;
            if(pos >= 0 && grid[i][pos] == '\\') {
                pos = -1;
            }
        }

        if(pos < 0 || pos >= m) {
            pos = -2;
            break;
        }

        i += 1;
    }

    cout << pos + 1 << '\n';
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
