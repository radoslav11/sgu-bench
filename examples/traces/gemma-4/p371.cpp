#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

/**
 * Problem Analysis:
 * A subway system consists of K circle lines and R radial lines.
 * K = M - N + 1 (where K is the number of circle lines)
 * Let k_i be the number of stations in the i-th circle line (3 <= k_i <= 10).
 * The circle lines form a chain, meaning circle line i shares exactly one station with circle line i+1.
 * The number of stations used by circle lines: N_C = sum(k_i) - (K - 1).
 * The number of segments used by circle lines: M_C = sum(k_i).
 * Each radial line connects a circle station to a unique terminal station.
 * Let R be the number of radial lines.
 * Each radial line uses one new station and one segment.
 * Total stations: N = N_C + R = sum(k_i) - K + 1 + R.
 * Total segments: M = M_C + R = sum(k_i) + R.
 * 
 * From M = sum(k_i) + R, we get R = M - sum(k_i).
 * Substituting R in the N equation:
 * N = sum(k_i) - K + 1 + M - sum(k_i) = M - K + 1 => K = M - N + 1.
 * 
 * Constraints:
 * 1. K >= 1 (Must have at least one circle line).
 * 2. 3 <= k_i <= 10.
 * 3. R >= 0 (Radial lines cannot be negative) => sum(k_i) <= M.
 * 4. Radial lines cannot connect to transfer stations.
 *    A transfer station connects two circle lines.
 *    Number of transfer stations in a chain of K lines is K-1.
 *    Number of non-transfer circle stations: N_C - (K-1) = (sum(k_i) - K + 1) - (K - 1) = sum(k_i) - 2K + 2.
 *    Each radial line needs one such station, so R <= sum(k_i) - 2K + 2.
 *    (M - sum(k_i)) <= sum(k_i) - 2K + 2 => 2 * sum(k_i) >= M + 2K - 2.
 * 
 * Let S = sum(k_i).
 * The conditions on S are:
 * - 3K <= S <= 10K
 * - S <= M
 * - 2S >= M + 2K - 2
 * 
 * If such an S exists, we can pick k_i in [3, 10] and build the chain.
 */

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    int K = M - N + 1;

    if (K < 1) {
        cout << "No solution" << endl;
        return 0;
    }

    // Find S such that:
    // S >= 3K
    // S <= 10K
    // S <= M
    // 2S >= M + 2K - 2
    int s_low = max(3 * K, (M + 2 * K - 2 + 1) / 2);
    int s_high = min(10 * K, M);

    if (s_low > s_high) {
        cout << "No solution" << endl;
        return 0;
    }

    // Pick S = s_high to minimize R (number of radial lines)
    int S = s_high;
    int R = M - S;

    vector<int> k(K, 3);
    int rem = S - 3 * K;
    for (int i = 0; i < K && rem > 0; ++i) {
        int add = min(rem, 7);
        k[i] += add;
        rem -= add;
    }

    vector<vector<int>> circles;
    int curr = 1;
    for (int i = 0; i < K; ++i) {
        vector<int> circle;
        if (i == 0) {
            for (int j = 1; j <= k[i]; ++j) {
                circle.push_back(j);
            }
            curr = k[i];
        } else {
            circle.push_back(curr);
            for (int j = 1; j < k[i]; ++j) {
                circle.push_back(++curr);
            }
        }
        circles.push_back(circle);
    }

    int NC = curr;
    set<int> transfer_stations;
    for (int i = 0; i < K - 1; ++i) {
        transfer_stations.insert(circles[i].back());
    }

    vector<int> non_transfer;
    for (int i = 1; i <= NC; ++i) {
        if (transfer_stations.find(i) == transfer_stations.end()) {
            non_transfer.push_back(i);
        }
    }

    // Verify we have enough non-transfer stations for R radial lines
    if (non_transfer.size() < (size_t)R) {
        // This shouldn't happen given the range check for S
        cout << "No solution" << endl;
        return 0;
    }

    cout << K << "\n";
    for (int i = 0; i < K; ++i) {
        cout << k[i];
        for (int st : circles[i]) {
            cout << " " << st;
        }
        cout << "\n";
    }

    cout << R << "\n";
    for (int i = 0; i < R; ++i) {
        cout << non_transfer[i] << " " << NC + 1 + i << "\n";
    }

    return 0;
}

