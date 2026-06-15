#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

struct building {
    int l, r, h;
};

int main() {
    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        vector<building> b(n);
        for (int i = 0; i < n; i++) {
            cin >> b[i].l >> b[i].r >> b[i].h;
        }

        // sort by right edge
        sort(b.begin(), b.end(), [](const building& a, const building& b) {
            return a.r < b.r;
        });

        vector<int> dp(n, 1000000000);
        for (int i = 0; i < n; i++) {
            // try to cover building i with one light source alone
            dp[i] = i + 1; // at least i+1 sources if we cover each alone

            // try to cover building i together with building j
            for (int j = 0; j < i; j++) {
                if (b[j].r <= b[i].l) {
                    dp[i] = min(dp[i], dp[j] + 1);
                } else {
                    // b[j].r > b[i].l, so they overlap in x? but the problem says no intersection or touch.
                    // so b[j].r <= b[i].l should be always true if sorted by r.
                    // unless the sort is by r and the next building's l is < this building's r? but they don't touch.
                    // So this branch might not happen.
                }
            }
        }
        cout << dp[n-1] << endl;
    }
    return 0;
}
