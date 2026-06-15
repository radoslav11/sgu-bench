#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

const int MAX_MASK = 1 << 18;
bool dp[MAX_MASK][17];
int parent_rem[MAX_MASK][17];
int parent_i[MAX_MASK][17];
int nxt_rem_table[17][10];

int main() {
    // Optimize standard I/O operations for competitive programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Precompute the next remainder for all possible current remainders and digits
    for (int r = 0; r < 17; ++r) {
        for (int d = 0; d < 10; ++d) {
            nxt_rem_table[r][d] = (r * 10 + d) % 17;
        }
    }

    string n;
    if (!(cin >> n)) return 0;

    int L = n.length();
    int max_mask = 1 << L;

    // Base case: empty prefix has remainder 0
    dp[0][0] = true;

    // DP over all subsets of digits
    for (int mask = 0; mask < max_mask; ++mask) {
        for (int i = 0; i < L; ++i) {
            // If the i-th digit is already used, skip
            if (mask & (1 << i)) continue;
            
            // The permutation cannot start with a zero
            if (mask == 0 && n[i] == '0') continue;

            int nxt_mask = mask | (1 << i);
            int d = n[i] - '0';
            
            // Transition to new states
            for (int rem = 0; rem < 17; ++rem) {
                if (dp[mask][rem]) {
                    int nxt_rem = nxt_rem_table[rem][d];
                    if (!dp[nxt_mask][nxt_rem]) {
                        dp[nxt_mask][nxt_rem] = true;
                        parent_rem[nxt_mask][nxt_rem] = rem;
                        parent_i[nxt_mask][nxt_rem] = i;
                    }
                }
            }
        }
    }

    // Check if we can form a number with all digits (max_mask - 1) that is divisible by 17 (rem == 0)
    if (dp[max_mask - 1][0]) {
        string ans = "";
        int curr_mask = max_mask - 1;
        int curr_rem = 0;
        
        // Backtrack to find the permutation
        while (curr_mask > 0) {
            int p_rem = parent_rem[curr_mask][curr_rem];
            int i = parent_i[curr_mask][curr_rem];
            ans += n[i];
            curr_rem = p_rem;
            curr_mask ^= (1 << i);
        }
        
        // The backtracked string is in reverse order
        reverse(ans.begin(), ans.end());
        cout << ans << "\n";
    } else {
        cout << -1 << "\n";
    }

    return 0;
}
