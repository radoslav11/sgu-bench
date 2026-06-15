#include <bits/stdc++.h>

#pragma GCC optimize("O3")
#pragma GCC target("avx2")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("tree-vectorize")

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
vector<vector<int>> R;

void multiply_add(
    vector<uint64_t>& result, const vector<uint64_t>& a,
    const vector<uint64_t>& b
) {
    const size_t size = a.size();
    const size_t chunk_size = 4;
    const size_t chunk_end = size - (size % chunk_size);

    for(size_t i = 0; i < chunk_end; i += chunk_size) {
        result[i] += a[i] * b[i];
        result[i + 1] += a[i + 1] * b[i + 1];
        result[i + 2] += a[i + 2] * b[i + 2];
        result[i + 3] += a[i + 3] * b[i + 3];
    }

    for(size_t i = chunk_end; i < size; i++) {
        result[i] += a[i] * b[i];
    }
}

void read() {
    cin >> n >> m;
    m--;
    R.resize(n, vector<int>(n));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            cin >> R[i][j];
        }
    }
}

void xor_transform(vector<uint64_t>& a, bool reverse = false) {
    int n = a.size();
    for(int i = 1; i < n; i <<= 1) {
        for(int j = 0; j < n; j += (i << 1)) {
            for(int k = 0; k < i; k++) {
                uint64_t x = a[j + k];
                uint64_t y = a[j + k + i];
                a[j + k] = x + y;
                a[j + k + i] = x - y;
            }
        }
    }

    if(reverse) {
        for(int i = 0; i < n; i++) {
            a[i] /= n;
        }
    }
}

void solve() {
    // We count minimal-height knockout tournaments in which player m wins. A
    // tournament is built by merging two disjoint sub-brackets: the winners of
    // two sub-tournaments over disjoint sets of players play, and the match
    // result R decides who advances. The bracket height must be minimal, which
    // means a sub-bracket over s players needs exactly ceil(log2(s)) rounds;
    // best_size[s-1] is that minimum number of merge rounds.
    //
    // The DP state is dp[step][winner][size][mask]: the number of ways to form
    // a sub-bracket of the given size and round count whose champion is
    // `winner` and whose participant set is `mask`. Merging two sub-brackets
    // requires their masks to be disjoint and their union to be tracked, which
    // is an XOR (subset) convolution over the participant bitmask.
    //
    // To make the convolution cheap we keep every dp[...] array in the
    // Walsh-Hadamard transformed domain over the n-bit mask space. There,
    // combining two sub-brackets is just pointwise multiplication
    // (multiply_add accumulates a[i] * b[i] for the winner determined by R).
    // Leaves dp[0][i][1] start as a single-player bracket for player i (the
    // indicator of mask {i}), transformed once. We iterate sub-bracket size
    // pairs in increasing max-size order so all inputs are ready, only allowing
    // round counts that keep the final tree at minimal height. Finally we
    // inverse-transform dp[steps-1][m][n] and read off the coefficient for the
    // full participant set (1 << n) - 1.

    vector<int> best_size(n + 1, 0);
    for(int i = 1; i <= n; i++) {
        best_size[i] = best_size[i >> 1] + 1;
    }

    int steps = best_size[n - 1] + 1;
    vector<vector<vector<vector<uint64_t>>>> dp(
        steps, vector<vector<vector<uint64_t>>>(
                   n, vector<vector<uint64_t>>(n + 1, vector<uint64_t>())
               )
    );

    for(int i = 0; i < n; i++) {
        dp[0][i][1].assign(1 << n, 0);
        dp[0][i][1][1 << i] = 1;
        xor_transform(dp[0][i][1]);
    }

    vector<pair<int, int>> sz_a_b;
    for(int sz_a = 1; sz_a <= n; sz_a++) {
        for(int sz_b = 1; sz_b <= n; sz_b++) {
            if(sz_a + sz_b > n) {
                continue;
            }
            sz_a_b.emplace_back(sz_a, sz_b);
        }
    }

    sort(sz_a_b.begin(), sz_a_b.end(), [](const auto& a, const auto& b) {
        return max(a.first, a.second) < max(b.first, b.second);
    });

    for(auto [sz_a, sz_b]: sz_a_b) {
        for(int step_a = 0; step_a + 1 < steps; step_a++) {
            if(step_a < best_size[sz_a - 1]) {
                continue;
            }
            for(int step_b = 0; step_b + 1 < steps; step_b++) {
                if(step_b < best_size[sz_b - 1]) {
                    continue;
                }
                for(int x = 0; x < n; x++) {
                    for(int y = x + 1; y < n; y++) {
                        int winner = R[x][y] ? x : y;
                        int new_step = max(step_a, step_b) + 1;
                        auto& dp_a = dp[step_a][x][sz_a];
                        auto& dp_b = dp[step_b][y][sz_b];
                        auto& dp_winner = dp[new_step][winner][sz_a + sz_b];

                        if(dp_a.empty() || dp_b.empty()) {
                            continue;
                        }

                        int left = n - sz_a - sz_b;
                        int left_steps = steps - new_step - 1;
                        if(left != 0 && left_steps == 0) {
                            continue;
                        }

                        if(dp_winner.empty()) {
                            dp_winner.assign(1 << n, 0);
                        }

                        multiply_add(
                            dp[new_step][winner][sz_a + sz_b],
                            dp[step_a][x][sz_a], dp[step_b][y][sz_b]
                        );
                    }
                }
            }
        }
    }

    if(dp[steps - 1][m][n].empty()) {
        cout << 0 << '\n';
        return;
    }
    xor_transform(dp[steps - 1][m][n], true);
    cout << dp[steps - 1][m][n][(1 << n) - 1] << '\n';
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
