#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Problem Analysis:
 * Two teams, Berland and Byteland, each with n players. Each player has a unique chess mastership level.
 * We need to find the number of ways to pair each Berland player with a Byteland player (sortition)
 * such that Berland wins exactly k points more than Byteland.
 * 
 * Let W_B be the number of games Berland wins and W_Y be the number of games Byteland wins.
 * W_B + W_Y = n (since no draws occur as all levels are distinct)
 * W_B - W_Y = k
 * Adding these gives 2 * W_B = n + k.
 * If (n + k) is odd, no such integer W_B exists, so the answer is 0.
 * Otherwise, W_B = (n + k) / 2.
 * 
 * Dynamic Programming Approach:
 * Sort all 2n players by their mastership level.
 * Let's use DP where we process players one by one from the sorted list.
 * At any stage m (number of players processed), we track:
 * - u: the number of Berland players already seen that are "waiting" to be paired with a Byteland player 
 *      whose level is higher (meaning these Berland players will eventually lose).
 * - w: the number of Byteland players already seen that are "waiting" to be paired with a Berland player 
 *      whose level is higher (meaning these Byteland players will eventually lose).
 * - wins: the number of Berland wins recorded so far.
 * 
 * The number of Byteland players waiting (w) can be derived from u, the total number of Berland players seen (I_m), 
 * and the total number of Byteland players seen (J_m). 
 * Every player is either waiting or has been paired.
 * If a Berland player is "waiting", they will lose (paired with a higher Byteland player).
 * If a Byteland player is "waiting", they will lose (paired with a higher Berland player).
 * A Berland win occurs when a Berland player is paired with a Byteland player seen earlier.
 * A Byteland win occurs when a Byteland player is paired with a Berland player seen earlier.
 * 
 * Let I_m be the number of Berland players in the first m players.
 * Let J_m be the number of Byteland players in the first m players.
 * I_m - J_m = (number of Berland players waiting) - (number of Byteland players waiting) = u - w.
 * So, w = u - (I_m - J_m).
 * 
 * Complexity:
 * Time: O(n^3) because the state space (m, u, wins) is bounded by 2n * n * n, but many are unreachable.
 * Memory: O(n^2) using space-optimized DP (only current and next m layers).
 */

const int MOD = 1000000009;

struct Player {
    int level;
    int team; // 0 for Berland, 1 for Byteland
};

bool comparePlayers(const Player& a, const Player& b) {
    return a.level < b.level;
}

int dp[2][505][505];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    if ((n + k) % 2 != 0) {
        cout << 0 << endl;
        return 0;
    }

    int target_wins = (n + k) / 2;
    vector<Player> players(2 * n);
    for (int i = 0; i < n; ++i) {
        cin >> players[i].level;
        players[i].team = 0;
    }
    for (int i = 0; i < n; ++i) {
        cin >> players[i + n].level;
        players[i + n].team = 1;
    }

    sort(players.begin(), players.end(), comparePlayers);

    // Precompute the number of Berland and Byteland players in the first m players.
    vector<int> I(2 * n + 1, 0), J(2 * n + 1, 0);
    for (int i = 0; i < 2 * n; ++i) {
        I[i + 1] = I[i] + (players[i].team == 0 ? 1 : 0);
        J[i + 1] = J[i] + (players[i].team == 1 ? 1 : 0);
    }

    // Initial state: 0 players processed, 0 waiting, 0 wins.
    dp[0][0][0] = 1;

    for (int m = 0; m < 2 * n; ++m) {
        int curr = m % 2;
        int next = (m + 1) % 2;

        // Clear the next DP layer
        for (int u = 0; u <= n; ++u) {
            for (int wins = 0; wins <= n; ++wins) {
                dp[next][u][wins] = 0;
            }
        }

        for (int u = 0; u <= n; ++u) {
            for (int wins = 0; wins <= n; ++wins) {
                if (dp[curr][u][wins] == 0) continue;

                int w = u - (I[m] - J[m]);
                if (w < 0) continue; // Should not happen if logic is correct

                if (players[m].team == 0) { // Berland player
                    // Case 1: Berland player wins (pairs with a waiting Byteland player)
                    if (w > 0 && wins + 1 <= n) {
                        dp[next][u][wins + 1] = (dp[next][u][wins + 1] + (long long)dp[curr][u][wins] * w) % MOD;
                    }
                    // Case 2: Berland player loses (becomes a "waiting" player)
                    if (u + 1 <= n) {
                        dp[next][u + 1][wins] = (dp[next][u + 1][wins] + dp[curr][u][wins]) % MOD;
                    }
                } else { // Byteland player
                    // Case 1: Byteland player wins (pairs with a waiting Berland player)
                    if (u > 0) {
                        dp[next][u - 1][wins] = (dp[next][u - 1][wins] + (long long)dp[curr][u][wins] * u) % MOD;
                    }
                    // Case 2: Byteland player loses (becomes a "waiting" player)
                    int w_next = w + 1;
                    if (w_next <= n) {
                        dp[next][u][wins] = (dp[next][u][wins] + dp[curr][u][wins]) % MOD;
                    }
                }
            }
        }
    }

    // After processing all 2n players, we need everyone to be paired (u=0, w=0) and wins == target_wins.
    // Since w = u - (I[2n] - J[2n]) and I[2n] = J[2n] = n, u=0 implies w=0.
    cout << dp[(2 * n) % 2][0][target_wins] << endl;

    return 0;
}

