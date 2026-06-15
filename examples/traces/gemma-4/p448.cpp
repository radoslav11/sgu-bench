#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Problem Analysis:
 * The goal is to find the number of tournament arrangements where a specific player M wins,
 * given the results of matches between all pairs of N players.
 * The tournament must have the minimum possible height, which is ceil(log2(N)).
 * A tournament arrangement is a rooted binary tree where the players are the leaves.
 * 
 * DP Approach:
 * Let dp[mask][p] be the number of tournament trees containing the players in the bitmask 'mask',
 * such that player 'p' (where p is part of the mask) is the winner of this tournament.
 * 
 * The height H(n) of a tournament with n players is ceil(log2(n)).
 * A tournament with mask 'mask' and height H(|mask|) can be formed by splitting 'mask' into two 
 * non-empty submasks s1 and s2 (where s1 | s2 = mask and s1 & s2 = 0), such that 
 * max(H(|s1|), H(|s2|)) = H(|mask|) - 1.
 * 
 * The recurrence relation for player p to win the tournament with mask 'mask':
 * dp[mask][p] = sum over all such s1, s2 where p is in s1:
 *      (dp[s1][p] * sum over all q in s2: (dp[s2][q] * [p beats q]))
 * 
 * To simplify, let win[mask][p] = sum over all q in mask: (dp[mask][q] * [p beats q]).
 * Then dp[mask][p] = sum over s1, s2: (dp[s1][p] * win[s2][p]).
 * 
 * Complexity:
 * The submask iteration leads to O(N * 3^N) total complexity, which is roughly 16 * 3^15 = 2.3e8 operations.
 * This fits within the 1-second time limit.
 */

// Use long long to prevent overflow, as the number of arrangements can be up to N!
typedef long long ll;

int N, M;
int adj[16][16];
int H[17];
ll dp[1 << 16][16];
ll win[1 << 16][16];

int main() {
    // Speed up standard input/output
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    // Read the number of players N and friend's ID M
    if (!(cin >> N >> M)) return 0;
    M--; // Convert to 0-indexed

    // Read the match results matrix
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> adj[i][j];
        }
    }

    // Precompute minimal heights for each number of players
    for (int i = 1; i <= 16; ++i) {
        H[i] = 0;
        while ((1 << H[i]) < i) H[i]++;
    }

    // Iterate through all possible mask sizes from 1 to N
    for (int k = 1; k <= N; ++k) {
        for (int mask = 0; mask < (1 << N); ++mask) {
            int pop = __builtin_popcount(mask);
            if (pop != k) continue;

            if (k == 1) {
                // Base case: a single player tournament has one way (the player themselves)
                int p = __builtin_ctz(mask);
                dp[mask][p] = 1;
            } else {
                // Recursive case: iterate over all submasks s1
                for (int s1 = (mask - 1) & mask; s1 > 0; s1 = (s1 - 1) & mask) {
                    int s2 = mask ^ s1;
                    if (s2 == 0) continue;
                    
                    // Minimal height condition
                    if (max(H[__builtin_popcount(s1)], H[__builtin_popcount(s2)]) == H[k] - 1) {
                        // For player p in s1, the number of ways is dp[s1][p] times the ways
                        // for any player in s2 to lose to p.
                        int temp_s1 = s1;
                        while (temp_s1 > 0) {
                            int p = __builtin_ctz(temp_s1);
                            if (dp[s1][p] > 0 && win[s2][p] > 0) {
                                dp[mask][p] += dp[s1][p] * win[s2][p];
                            }
                            temp_s1 &= (temp_s1 - 1);
                        }
                    }
                }
            }

            // After computing dp[mask][p] for all p in the mask, compute win[mask][p]
            // win[mask][p] is the sum of dp[mask][q] for all q in mask such that p beats q.
            for (int p = 0; p < N; ++p) {
                win[mask][p] = 0;
                for (int q = 0; q < N; ++q) {
                    if (((mask >> q) & 1) && adj[p][q]) {
                        win[mask][p] += dp[mask][q];
                    }
                }
            }
        }
    }

    // The answer is the number of ways for player M to win a tournament with all N players.
    cout << dp[(1 << N) - 1][M] << endl;

    return 0;
}

