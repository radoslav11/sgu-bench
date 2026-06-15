#include <iostream>
#include <vector>

using namespace std;

const int MAX_MASK = 1 << 21;
int sum_arr[MAX_MASK];
bool is_valid[MAX_MASK];
int dp[MAX_MASK];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    vector<int> S(N), D(N);
    long long sum_S = 0, sum_D = 0;
    for (int i = 0; i < N; ++i) {
        cin >> S[i];
        sum_S += S[i];
    }
    for (int i = 0; i < N; ++i) {
        cin >> D[i];
        sum_D += D[i];
    }

    if (sum_S != sum_D) {
        cout << -1 << "\n";
        return 0;
    }

    vector<int> diffs;
    int zero_count = 0;
    for (int i = 0; i < N; ++i) {
        int diff = S[i] - D[i];
        if (diff == 0) {
            zero_count++;
        } else {
            diffs.push_back(diff);
        }
    }

    int num_diffs = diffs.size();
    int num_masks = 1 << num_diffs;

    vector<int> valid_masks;
    for (int mask = 1; mask < num_masks; ++mask) {
        int i = __builtin_ctz(mask);
        sum_arr[mask] = sum_arr[mask ^ (1 << i)] + diffs[i];
        if (sum_arr[mask] == 0) {
            is_valid[mask] = true;
            valid_masks.push_back(mask);
        }
    }

    dp[0] = 0;
    for (int mask : valid_masks) {
        dp[mask] = 1;
        int i = __builtin_ctz(mask);
        int rest = mask ^ (1 << i);
        int best = 1;
        for (int sub = rest; ; sub = (sub - 1) & rest) {
            int current_sub = sub | (1 << i);
            if (is_valid[current_sub]) {
                int val = 1 + dp[mask ^ current_sub];
                if (val > best) best = val;
            }
            if (sub == 0) break;
        }
        dp[mask] = best;
    }

    int max_subsets = dp[num_masks - 1];
    int ans = N - (max_subsets + zero_count);
    cout << ans << "\n";

    return 0;
}
