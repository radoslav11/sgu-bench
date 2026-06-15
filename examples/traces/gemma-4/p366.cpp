#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>
#include <map>

using namespace std;

struct Opponent {
    int a, b, id;
};

int N, K;
vector<Opponent> items;
bitset<1001> dp[21][1001];
const int MAX_SUM = 1000;
const int BLOCK_SIZE = 400;
const int CHECKPOINT_STEP = 5;

bitset<1001> dp_at_block[25][21][1001];
bitset<1001> block_states[405][21][1001];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N >> K)) return 0;

    map<pair<int, int>, vector<int>> groups;
    for (int i = 1; i <= N; ++i) {
        int a, b;
        cin >> a >> b;
        groups[{a, b}].push_back(i);
    }

    for (auto const& [vals, ids] : groups) {
        int a = vals.first;
        int b = vals.second;
        for (int i = 0; i < (int)ids.size() && i < K; ++i) {
            items.push_back({a, b, ids[i]});
        }
    }

    int M = items.size();
    dp[0][0][0] = 1;
    for (int i = 0; i < M; ++i) {
        if (i % (CHECKPOINT_STEP * BLOCK_SIZE) == 0) {
            int b_idx = i / (CHECKPOINT_STEP * BLOCK_SIZE);
            if (b_idx < 25) {
                for (int k = 0; k <= K; ++k)
                    for (int s = 0; s <= MAX_SUM; ++s)
                        dp_at_block[b_idx][k][s] = dp[k][s];
            }
        }
        int a = items[i].a, b = items[i].b;
        for (int k = K; k >= 1; --k) {
            for (int s = MAX_SUM; s >= a; --s) {
                dp[k][s] |= (dp[k - 1][s - a] << b);
            }
        }
    }

    int bestA = -1, bestB = -1, minDiff = 1e9, maxSum = -1;
    for (int s = 0; s <= MAX_SUM; ++s) {
        for (int v = 0; v <= MAX_SUM; ++v) {
            if (dp[K][s][v]) {
                int diff = abs(s - v);
                if (diff < minDiff || (diff == minDiff && (s + v) > maxSum)) {
                    minDiff = diff; maxSum = s + v; bestA = s; bestB = v;
                }
            }
        }
    }

    cout << bestA << " " << bestB << endl;
    vector<int> res;
    int curK = K, curA = bestA, curB = bestB;

    for (int i = M - 1; i >= 0; ) {
        int block_idx = i / BLOCK_SIZE;
        int block_start = block_idx * BLOCK_SIZE;
        int block_end = min(block_start + BLOCK_SIZE - 1, M - 1);
        int cp_m = (block_start / (CHECKPOINT_STEP * BLOCK_SIZE)) * (CHECKPOINT_STEP * BLOCK_SIZE);
        int cp_idx = cp_m / (CHECKPOINT_STEP * BLOCK_SIZE);

        static bitset<1001> temp_dp[21][1001];
        for (int k = 0; k <= K; ++k)
            for (int s = 0; s <= MAX_SUM; ++s)
                temp_dp[k][s] = dp_at_block[cp_idx][k][s];

        for (int m = cp_m; m < block_start; ++m) {
            int a = items[m].a, b = items[m].b;
            for (int k = K; k >= 1; --k)
                for (int s = MAX_SUM; s >= a; --s)
                    temp_dp[k][s] |= (temp_dp[k - 1][s - a] << b);
        }

        for (int k = 0; k <= K; ++k)
            for (int s = 0; s <= MAX_SUM; ++s)
                block_states[0][k][s] = temp_dp[k][s];

        for (int m = block_start; m <= block_end; ++m) {
            int a = items[m].a, b = items[m].b;
            for (int k = K; k >= 1; --k)
                for (int s = MAX_SUM; s >= a; --s)
                    temp_dp[k][s] |= (temp_dp[k - 1][s - a] << b);
            for (int k = 0; k <= K; ++k)
                for (int s = 0; s <= MAX_SUM; ++s)
                    block_states[m - block_start + 1][k][s] = temp_dp[k][s];
        }

        for (int m = block_end; m >= block_start; --m) {
            if (curK > 0 && curA >= items[m].a && curB >= items[m].b) {
                if (block_states[m - block_start][curK - 1][curA - items[m].a][curB - items[m].b]) {
                    res.push_back(items[m].id);
                    curA -= items[m].a; curB -= items[m].b; curK--;
                }
            }
        }
        i = block_start - 1;
    }

    sort(res.begin(), res.end());
    for (int i = 0; i < K; ++i) cout << res[i] << (i == K - 1 ? "" : " ");
    cout << endl;

    return 0;
}

