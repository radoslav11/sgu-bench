#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int n, k;
    cin >> n >> k;
    vector<int> b(n);
    for (int i = 0; i < n; i++) {
        cin >> b[i];
    }
    sort(b.begin(), b.end());

    vector<unsigned long long> dp(k + 1, 0);
    dp[0] = 1;

    for (int i = 0; i < n; i++) {
        for (int j = k; j >= 1; j--) {
            long long available = b[i] - (j - 1);
            if (available > 0) {
                dp[j] += dp[j - 1] * available;
            }
        }
    }

    cout << dp[k] << endl;
    return 0;
}
