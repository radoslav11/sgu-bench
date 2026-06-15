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
vector<int> p;

void read() {
    cin >> n >> m;
    p.resize(m);
    for(int i = 0; i < m; i++) {
        cin >> p[i];
    }
    p.push_back(1);
}

void solve() {
    // This is a losing-last-match (misere) take-away game: dp[i] is 1 if the
    // player to move with i matches wins. Position i wins if some allowed move
    // x leads to a losing position dp[i - x] == 0; dp[0] = 1 encodes that
    // facing 0 matches means the opponent just took the last and lost. Since
    // moves take at most 9 matches, dp[i] depends only on the previous 10
    // values, so the sequence is eventually periodic. We pack the last 10
    // outcomes into a 10-bit state and record the first index each state
    // appears; on a repeat we know the cycle length and jump straight to the
    // equivalent index for n via modular arithmetic instead of iterating up to
    // 10^9.

    sort(p.begin(), p.end());
    p.erase(unique(p.begin(), p.end()), p.end());

    vector<int> pos_of_state(1 << 10, -1);

    vector<int> dp(min(1 << 13, n + 1), 0);
    dp[1] = 0;
    dp[0] = 1;

    int ans = -1;
    for(int i = 2; i <= n; i++) {
        for(int x: p) {
            if(x > i) {
                break;
            }
            if(dp[i - x] == 0) {
                dp[i] = 1;
                break;
            }
        }

        int state = 0;
        for(int prv = 0; prv < 10; prv++) {
            if(i - prv < 0 || dp[i - prv] == 1) {
                state |= (1 << prv);
            }
        }

        if(pos_of_state[state] == -1) {
            pos_of_state[state] = i;
        } else {
            int cycle_length = i - pos_of_state[state];
            int need = (n - i) % cycle_length;

            ans = dp[i - cycle_length + need];
            break;
        }
    }

    if(ans == -1) {
        ans = dp[n];
    }

    cout << (ans ? "FIRST PLAYER MUST WIN" : "SECOND PLAYER MUST WIN") << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
