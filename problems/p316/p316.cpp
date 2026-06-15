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
vector<pair<int, int>> shots;

void read() {
    cin >> n >> m;
    shots.resize(m);
    cin >> shots;
}

void solve() {
    // Direct simulation of the match log:
    //
    // - hit_points[i] starts at 100, score_points[i] at 0.
    //
    // - For each shot (shooter, target), if the target still had positive
    //   hit points before the shot the shooter gains 3 score points, then
    //   the target loses 8 hit points regardless.
    //
    // - At the end every tank with positive hit points additionally gains
    //   half of its remaining hit points as score.

    vector<int> hit_points(n, 100), score_points(n, 0);
    for(auto [shooter, target]: shots) {
        shooter--;
        target--;
        if(hit_points[target] > 0) {
            score_points[shooter] += 3;
        }

        hit_points[target] -= 8;
    }

    for(int i = 0; i < n; i++) {
        if(hit_points[i] > 0) {
            score_points[i] += hit_points[i] / 2;
        }
    }

    for(int i = 0; i < n; i++) {
        cout << hit_points[i] << ' ' << score_points[i] << '\n';
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
