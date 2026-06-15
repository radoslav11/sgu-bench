#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>

using namespace std;

/**
 * Problem Analysis:
 * - N players, height in meters (real numbers).
 * - Height is an integer number of micrometers (1e-6 meters).
 * - Average height is exactly 2000 mm = 2.000 m.
 * - Condition for a "good" team:
 *   For any two selected players (i, j), let H be the sum of their heights and 
 *   the heights of all players between them, and K be the number of these players.
 *   If |H - 2000*K| > 10 cm (100 mm), the team is "bad".
 * - 10 cm = 100 mm = 100,000 micrometers.
 * - H - 2000*K = Sum_{k=i to j} (h_k - 2000 mm).
 * - Let h'_k = h_k - 2000 mm (in micrometers).
 * - The condition is |Sum_{k=i to j} h'_k| <= 100,000.
 * - This is equivalent to saying that the maximum difference between any two
 *   prefix sums of h'_k must be at most 100,000.
 * - Since the average is 2000 mm, Sum_{k=1 to N} h'_k = 0.
 * - Let S_m be the prefix sum of h'_k. S_0 = 0, S_N = 0.
 * - The condition is Max(S_m) - Min(S_m) <= 100,000.
 * - Given heights are between 1950 and 2050 mm, so h'_k is in [-50000, 50000].
 * - A greedy strategy:
 *   Keep the current prefix sum S as close to 0 as possible.
 *   If S <= 0, add the largest remaining h'_k.
 *   If S > 0, add the smallest (most negative) remaining h'_k.
 *   This strategy ensures S stays in the range [-50000, 50000].
 *   The range of S values will be at most 100,000, satisfying the condition.
 */

struct Player {
    long long h_prime;
    int id;
};

bool comparePlayers(const Player& a, const Player& b) {
    return a.h_prime < b.h_prime;
}

int main() {
    // Optimizing I/O
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;

    vector<Player> players(N);
    for (int i = 0; i < N; ++i) {
        double h_m;
        cin >> h_m;
        // Convert meters to micrometers and subtract 2,000,000 micrometers
        // Using long double and roundl to handle precision
        players[i].h_prime = (long long)roundl((long double)h_m * 1000000.0L - 2000000.0L);
        players[i].id = i + 1;
    }

    // Sorting players by their height relative to 2000 mm
    sort(players.begin(), players.end(), comparePlayers);

    vector<int> result;
    int left = 0, right = N - 1;
    long long current_S = 0;

    // Greedy selection using two pointers to keep the prefix sum S in [-50000, 50000]
    for (int i = 0; i < N; ++i) {
        if (current_S <= 0) {
            // Current prefix sum is non-positive, add largest available value
            current_S += players[right].h_prime;
            result.push_back(players[right].id);
            right--;
        } else {
            // Current prefix sum is positive, add smallest available value
            current_S += players[left].h_prime;
            result.push_back(players[left].id);
            left++;
        }
    }

    // Since a solution is guaranteed given the height bounds and average height, 
    // we just print the resulting order.
    cout << "yes" << "\n";
    for (int i = 0; i < N; ++i) {
        cout << result[i] << (i == N - 1 ? "" : " ");
    }
    cout << endl;

    return 0;
}

