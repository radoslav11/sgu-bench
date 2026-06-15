#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

int main() {
    int N;
    cin >> N;
    string s;
    cin >> s;

    string buttons = "LURD";
    vector<pair<char, char>> states;
    for (char a : buttons) {
        for (char b : buttons) {
            if (a != b && !(a == 'R' && b == 'L')) {
                states.push_back(make_pair(a, b));
            }
        }
    }
    int num_states = states.size();

    vector<vector<long long>> dp(N, vector<long long>(num_states, INF));
    vector<vector<int>> parent(N, vector<int>(num_states, -1));

    pair<char, char> init = make_pair('L', 'R');

    for (int j = 0; j < num_states; j++) {
        char l = states[j].first;
        char r = states[j].second;
        if (s[0] != 'N' && s[0] != l && s[0] != r) {
            continue;
        }

        long long cost;
        if (l == init.first && r == init.second) {
            cost = (s[0] == 'N') ? 0 : 1;
        } else if (l == init.first) {
            cost = 3;
        } else if (r == init.second) {
            cost = 3;
        } else {
            cost = 10;
        }
        dp[0][j] = cost;
        parent[0][j] = -1;
    }

    for (int i = 1; i < N; i++) {
        for (int j = 0; j < num_states; j++) {
            char l_curr = states[j].first;
            char r_curr = states[j].second;
            if (s[i] != 'N' && s[i] != l_curr && s[i] != r_curr) {
                continue;
            }

            for (int k = 0; k < num_states; k++) {
                if (dp[i-1][k] == INF) continue;
                char l_prev = states[k].first;
                char r_prev = states[k].second;

                long long cost;
                if (l_prev == l_curr && r_prev == r_curr) {
                    cost = (s[i] == 'N') ? 0 : 1;
                } else if (l_prev == l_curr) {
                    if (s[i-1] == 'N' || s[i-1] == l_prev) {
                        cost = 3;
                    } else {
                        cost = 9;
                    }
                } else if (r_prev == r_curr) {
                    if (s[i-1] == 'N' || s[i-1] == r_prev) {
                        cost = 3;
                    } else {
                        cost = 9;
                    }
                } else {
                    cost = 10;
                }

                long long new_cost = dp[i-1][k] + cost;
                if (new_cost < dp[i][j]) {
                    dp[i][j] = new_cost;
                    parent[i][j] = k;
                }
            }
        }
    }

    long long total = INF;
    int last_state = -1;
    for (int j = 0; j < num_states; j++) {
        if (dp[N-1][j] < total) {
            total = dp[N-1][j];
            last_state = j;
        }
    }

    vector<pair<char, char>> res(N);
    int cur = last_state;
    for (int i = N-1; i >= 0; i--) {
        res[i] = states[cur];
        if (i == 0) {
            break;
        }
        cur = parent[i][cur];
    }

    cout << total << '\n';
    for (int i = 0; i < N; i++) {
        cout << res[i].first << res[i].second << '\n';
    }

    return 0;
}
