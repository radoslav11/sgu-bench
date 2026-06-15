#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/**
 * Problem: Dance it up!
 * Goal: Minimize energy to complete a song of N beats.
 * N <= 1000.
 * Possible positions: (Left Leg, Right Leg) on 4 buttons (L, U, R, D).
 * Forbidden position: (R, L).
 * Transitions depend on the previous beat's state (which leg pressed a button).
 * 
 * DP State: dp[i][l][r][p]
 * - i: current beat (0 to N)
 * - l: left leg position (0:L, 1:U, 2:R, 3:D)
 * - r: right leg position (0:L, 1:U, 2:R, 3:D)
 * - p: which legs pressed a button in beat i (0:None, 1:Left, 2:Right, 3:Both)
 * 
 * Transitions:
 * 1. Rule 1 (Cost 0): Stay, no press. Allowed if s[i] == 'N'.
 * 2. Rule 2 (Cost 1): Stay, press one existing button.
 * 3. Rule 3 (Cost 3): One leg moves to a vacant button (not current position of either leg) and presses.
 *    The leg that didn't press in the previous beat must move.
 * 4. Rule 4 (Cost 9): One leg moves to a vacant button and presses.
 *    The leg that pressed in the previous beat must move.
 * 5. Rule 5 (Cost 10): Jump both legs to any two different buttons and press both.
 */

const int INF = 1e9;

struct State {
    int l, r, p;
};

int dp[1001][4][4][4];
State parent[1001][4][4][4];

int main() {
    // Optimization for fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;
    string S;
    cin >> S;

    // Map buttons to integers
    vector<int> s(N + 1);
    for (int i = 1; i <= N; ++i) {
        if (S[i - 1] == 'L') s[i] = 0;
        else if (S[i - 1] == 'U') s[i] = 1;
        else if (S[i - 1] == 'R') s[i] = 2;
        else if (S[i - 1] == 'D') s[i] = 3;
        else s[i] = -1;
    }

    // Initialize DP table
    for (int i = 0; i <= N; ++i)
        for (int l = 0; l < 4; ++l)
            for (int r = 0; r < 4; ++r)
                for (int p = 0; p < 4; ++p)
                    dp[i][l][r][p] = INF;

    // Initial position: Left on L, Right on R. (Beat 0: p=0)
    dp[0][0][2][0] = 0;

    for (int i = 1; i <= N; ++i) {
        for (int l = 0; l < 4; ++l) {
            for (int r = 0; r < 4; ++r) {
                for (int p = 0; p < 4; ++p) {
                    if (dp[i - 1][l][r][p] == INF) continue;

                    // Rule 1: Cost 0 - Not to change legs position. Not to press buttons.
                    if (s[i] == -1) {
                        if (dp[i - 1][l][r][p] < dp[i][l][r][0]) {
                            dp[i][l][r][0] = dp[i - 1][l][r][p];
                            parent[i][l][r][0] = {l, r, p};
                        }
                    }

                    // Rule 2: Cost 1 - Not to change legs position. Press the button on which one of the legs is.
                    if (s[i] == -1 || s[i] == l || s[i] == r) {
                        if (s[i] == -1) {
                            // Can press either the current left or current right leg
                            if (dp[i - 1][l][r][p] + 1 < dp[i][l][r][1]) {
                                dp[i][l][r][1] = dp[i - 1][l][r][p] + 1;
                                parent[i][l][r][1] = {l, r, p};
                            }
                            if (dp[i - 1][l][r][p] + 1 < dp[i][l][r][2]) {
                                dp[i][l][r][2] = dp[i - 1][l][r][p] + 1;
                                parent[i][l][r][2] = {l, r, p};
                            }
                        } else {
                            int p_new = (s[i] == l) ? 1 : 2;
                            if (dp[i - 1][l][r][p] + 1 < dp[i][l][r][p_new]) {
                                dp[i][l][r][p_new] = dp[i - 1][l][r][p] + 1;
                                parent[i][l][r][p_new] = {l, r, p};
                            }
                        }
                    }

                    // Rule 3: Cost 3 - Move the leg, which didn't press a button at the previous beat,
                    // to a vacant button and press it.
                    if (p == 1) { // Left pressed, Right didn't. Move Right leg.
                        for (int r_new = 0; r_new < 4; ++r_new) {
                            if (r_new != l && r_new != r && (s[i] == -1 || s[i] == r_new)) {
                                if (dp[i - 1][l][r][p] + 3 < dp[i][l][r_new][2]) {
                                    dp[i][l][r_new][2] = dp[i - 1][l][r][p] + 3;
                                    parent[i][l][r_new][2] = {l, r, p};
                                }
                            }
                        }
                    } else if (p == 2) { // Right pressed, Left didn't. Move Left leg.
                        for (int l_new = 0; l_new < 4; ++l_new) {
                            if (l_new != l && l_new != r && (s[i] == -1 || s[i] == l_new)) {
                                if (dp[i - 1][l][r][p] + 3 < dp[i][l_new][r][1]) {
                                    dp[i][l_new][r][1] = dp[i - 1][l][r][p] + 3;
                                    parent[i][l_new][r][1] = {l, r, p};
                                }
                            }
                        }
                    } else if (p == 0 || p == 3) { // Neither or Both pressed. Move either leg.
                        for (int r_new = 0; r_new < 4; ++r_new) {
                            if (r_new != l && r_new != r && (s[i] == -1 || s[i] == r_new)) {
                                if (dp[i - 1][l][r][p] + 3 < dp[i][l][r_new][2]) {
                                    dp[i][l][r_new][2] = dp[i - 1][l][r][p] + 3;
                                    parent[i][l][r_new][2] = {l, r, p};
                                }
                            }
                        }
                        for (int l_new = 0; l_new < 4; ++l_new) {
                            if (l_new != l && l_new != r && (s[i] == -1 || s[i] == l_new)) {
                                if (dp[i - 1][l][r][p] + 3 < dp[i][l_new][r][1]) {
                                    dp[i][l_new][r][1] = dp[i - 1][l][r][p] + 3;
                                    parent[i][l_new][r][1] = {l, r, p};
                                }
                            }
                        }
                    }

                    // Rule 4: Cost 9 - Move the leg, which pressed a button at the previous beat,
                    // to a vacant button and press it.
                    if (p == 1) { // Left pressed. Move Left leg.
                        for (int l_new = 0; l_new < 4; ++l_new) {
                            if (l_new != l && l_new != r && (s[i] == -1 || s[i] == l_new)) {
                                if (dp[i - 1][l][r][p] + 9 < dp[i][l_new][r][1]) {
                                    dp[i][l_new][r][1] = dp[i - 1][l][r][p] + 9;
                                    parent[i][l_new][r][1] = {l, r, p};
                                }
                            }
                        }
                    } else if (p == 2) { // Right pressed. Move Right leg.
                        for (int r_new = 0; r_new < 4; ++r_new) {
                            if (r_new != l && r_new != r && (s[i] == -1 || s[i] == r_new)) {
                                if (dp[i - 1][l][r][p] + 9 < dp[i][l][r_new][2]) {
                                    dp[i][l][r_new][2] = dp[i - 1][l][r][p] + 9;
                                    parent[i][l][r_new][2] = {l, r, p};
                                }
                            }
                        }
                    } else if (p == 3) { // Both pressed. Move either leg.
                        for (int l_new = 0; l_new < 4; ++l_new) {
                            if (l_new != l && l_new != r && (s[i] == -1 || s[i] == l_new)) {
                                if (dp[i - 1][l][r][p] + 9 < dp[i][l_new][r][1]) {
                                    dp[i][l_new][r][1] = dp[i - 1][l][r][p] + 9;
                                    parent[i][l_new][r][1] = {l, r, p};
                                }
                            }
                        }
                        for (int r_new = 0; r_new < 4; ++r_new) {
                            if (r_new != l && r_new != r && (s[i] == -1 || s[i] == r_new)) {
                                if (dp[i - 1][l][r][p] + 9 < dp[i][l][r_new][2]) {
                                    dp[i][l][r_new][2] = dp[i - 1][l][r][p] + 9;
                                    parent[i][l][r_new][2] = {l, r, p};
                                }
                            }
                        }
                    }

                    // Rule 5: Cost 10 - Jump with both legs to any two different buttons and press both of them.
                    for (int l_new = 0; l_new < 4; ++l_new) {
                        for (int r_new = 0; r_new < 4; ++r_new) {
                            if (l_new != r_new && (s[i] == -1 || s[i] == l_new || s[i] == r_new)) {
                                if (dp[i - 1][l][r][p] + 10 < dp[i][l_new][r_new][3]) {
                                    dp[i][l_new][r_new][3] = dp[i - 1][l][r][p] + 10;
                                    parent[i][l_new][r_new][3] = {l, r, p};
                                }
                            }
                        }
                    }
                }
            }
        }
        // Enforce illegal position: (R, L) is forbidden
        for (int p = 0; p < 4; ++p) dp[i][2][0][p] = INF;
    }

    // Find minimal energy
    int min_energy = INF;
    State best_state = {-1, -1, -1};
    for (int l = 0; l < 4; ++l) {
        for (int r = 0; r < 4; ++r) {
            for (int p = 0; p < 4; ++p) {
                if (dp[N][l][r][p] < min_energy) {
                    min_energy = dp[N][l][r][p];
                    best_state = {l, r, p};
                }
            }
        }
    }

    // Output energy and reconstruct path
    cout << min_energy << "\n";
    vector<string> path;
    State curr = best_state;
    for (int i = N; i >= 1; --i) {
        char buttons[] = {'L', 'U', 'R', 'D'};
        string s_pos = "";
        s_pos += buttons[curr.l];
        s_pos += buttons[curr.r];
        path.push_back(s_pos);
        curr = parent[i][curr.l][curr.r][curr.p];
    }
    reverse(path.begin(), path.end());
    for (const string& s_pos : path) {
        cout << s_pos << "\n";
    }

    return 0;
}

