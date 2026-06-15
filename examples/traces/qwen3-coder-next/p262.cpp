#include <bits/stdc++.h>
using namespace std;

int main() {
    int N, M, K;
    cin >> N >> M >> K;
    vector<vector<string>> symbols(K, vector<string>(N));
    for (int k = 0; k < K; k++) {
        for (int i = 0; i < N; i++) {
            cin >> symbols[k][i];
        }
    }

    // If K==1, then minimal set is 0, but K>=2 per problem, so we assume K>=2.
    int total_pairs = K * (K - 1) / 2;
    vector<pair<int, int>> pairs;
    for (int i = 0; i < K; i++) {
        for (int j = i + 1; j < K; j++) {
            pairs.push_back({i, j});
        }
    }

    // Build the universe U: pixels that differ in at least one pair
    vector<pair<int, int>> pixels;
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < M; c++) {
            bool found = false;
            for (int i = 0; i < K && !found; i++) {
                for (int j = i + 1; j < K; j++) {
                    if (symbols[i][r][c] != symbols[j][r][c]) {
                        found = true;
                        break;
                    }
                }
            }
            if (found) {
                pixels.push_back({r, c});
            }
        }
    }

    int u = pixels.size();
    // If there are no differing pixels, then symbols are not unique? but problem says unique, so we assume u>0.
    // However, if u==0, then we output 0 and a grid of zeros.
    if (u == 0) {
        cout << 0 << endl;
        for (int i = 0; i < N; i++) {
            cout << string(M, '0') << endl;
        }
        return 0;
    }

    // Build cover mask for each pixel in pixels
    vector<int> cover(u, 0);
    for (int idx = 0; idx < u; idx++) {
        int r = pixels[idx].first;
        int c = pixels[idx].second;
        for (int pidx = 0; pidx < total_pairs; pidx++) {
            int i1 = pairs[pidx].first;
            int j1 = pairs[pidx].second;
            if (symbols[i1][r][c] != symbols[j1][r][c]) {
                cover[idx] |= (1 << pidx);
            }
        }
    }

    int total_states = 1 << total_pairs;
    vector<int> dp(total_states, INT_MAX);
    vector<int> parent(total_states, -1);
    vector<int> last_pixel(total_states, -1);

    dp[0] = 0;
    for (int i = 0; i < u; i++) {
        for (int state = total_states - 1; state >= 0; state--) {
            if (dp[state] == INT_MAX) continue;
            int new_state = state | cover[i];
            if (dp[new_state] > dp[state] + 1) {
                dp[new_state] = dp[state] + 1;
                parent[new_state] = state;
                last_pixel[new_state] = i;
            }
        }
    }

    int target = total_states - 1;
    // Since symbols are unique, dp[target] should be finite.
    if (dp[target] == INT_MAX) {
        // This should not happen, but fallback: use all pixels
        cout << u << endl;
        vector<string> grid(N, string(M, '0'));
        for (int i = 0; i < u; i++) {
            int r = pixels[i].first;
            int c = pixels[i].second;
            grid[r][c] = '1';
        }
        for (int i = 0; i < N; i++) {
            cout << grid[i] << endl;
        }
        return 0;
    }

    // Reconstruct the chosen pixels
    vector<bool> chosen(u, false);
    int state = target;
    while (state != 0) {
        int pidx = last_pixel[state];
        chosen[pidx] = true;
        state = parent[state];
    }

    cout << dp[target] << endl;
    vector<string> grid(N, string(M, '0'));
    for (int i = 0; i < u; i++) {
        if (chosen[i]) {
            int r = pixels[i].first;
            int c = pixels[i].second;
            grid[r][c] = '1';
        }
    }
    for (int i = 0; i < N; i++) {
        cout << grid[i] << endl;
    }

    return 0;
}
