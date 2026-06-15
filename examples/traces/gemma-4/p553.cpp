#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/**
 * Problem: Sultan's Pearls
 * 
 * Key Idea:
 * - The pearls are in a range [L, R]. Initially L=1, R=n.
 * - Table pearls: [L, R-m], Hanging pearls: [R-m+1, R].
 * - T-action: Removes pearl L. New range [L+1, R].
 * - H-action: Removes pearl R. New range [L, R-1].
 *   Crucially, H-action also moves the topmost table pearl (R-m) to the hanging part.
 * 
 * If we decide to take j pearls from the hanging end (H) and i pearls from the table end (T),
 * the best strategy to keep the string motionless is to perform all H-actions first,
 * because this keeps the table mass (Wt) as large as possible for as long as possible.
 * 
 * For a fixed j, the number of H-actions (j) must be such that all H-actions
 * satisfy the friction condition. Then, for each such j, we find the largest i
 * such that the T-actions also satisfy the condition. Since all prices ci >= 1,
 * we should always pick the maximum possible i for a fixed j.
 * 
 * The total pearls removed will be i + j. The condition i + j <= n - m - 1
 * ensures that there is at least one table pearl left so that Wt > 0.
 */

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    if (!(cin >> n >> m >> k)) return 0;

    vector<long long> w(n + 1), c(n + 1);
    vector<long long> pw(n + 1, 0), pc(n + 1, 0);

    for (int i = 1; i <= n; ++i) {
        cin >> w[i] >> c[i];
        pw[i] = pw[i - 1] + w[i];
        pc[i] = pc[i - 1] + c[i];
    }

    long long Wt0 = pw[n - m];
    long long Wh0 = pw[n] - pw[n - m];

    int j_max = 0;
    // The H-actions can take at most j such that there is at least one table pearl left.
    // j <= n - m - 1
    for (int j = 1; j <= n - m - 1; ++j) {
        long long Wh_j = Wh0 - (pw[n] - pw[n - j]) + (pw[n - m] - pw[n - m - j]);
        long long Wt_j = Wt0 - (pw[n - m] - pw[n - m - j]);
        if (Wh_j <= (long long)k * Wt_j) {
            j_max = j;
        } else {
            break;
        }
    }

    long long max_s = 0;
    int best_i = 0, best_j = 0;

    for (int j = 0; j <= j_max; ++j) {
        // Calculate Wh_j and Wt_j for the current j
        long long Wh_j, Wt_j;
        if (j == 0) {
            Wh_j = Wh0;
            Wt_j = Wt0;
        } else {
            Wh_j = Wh0 - (pw[n] - pw[n - j]) + (pw[n - m] - pw[n - m - j]);
            Wt_j = Wt0 - (pw[n - m] - pw[n - m - j]);
        }

        // Find the largest i in [0, n - m - 1 - j] such that k * (Wt_j - pw[i]) >= Wh_j
        int low = 0, high = n - m - 1 - j;
        int current_i = -1;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (k * (Wt_j - pw[mid]) >= Wh_j) {
                current_i = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }

        if (current_i != -1) {
            long long current_s = pc[current_i] + (pc[n] - pc[n - j]);
            if (current_s > max_s) {
                max_s = current_s;
                best_i = current_i;
                best_j = j;
            }
        }
    }

    if (max_s == 0) {
        cout << "0 0" << endl;
    } else {
        cout << best_i + best_j << " " << max_s << "\n";
        string res = "";
        for (int idx = 0; idx < best_j; ++idx) res += 'H';
        for (int idx = 0; idx < best_i; ++idx) res += 'T';
        cout << res << endl;
    }

    return 0;
}

