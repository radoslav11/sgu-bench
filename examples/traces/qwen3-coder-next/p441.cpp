#include <iostream>
#include <vector>
using namespace std;

const int mod = 2007;

int main() {
    int N, K;
    cin >> N >> K;

    if (K > N) {
        cout << 0 << endl;
        return 0;
    }

    vector<int> dp(K + 1, 0);
    dp[0] = 1;

    for (int i = 1; i <= N; i++) {
        for (int j = K; j >= 1; j--) {
            dp[j] = (1LL * j * dp[j] + dp[j - 1]) % mod;
        }
        dp[0] = 0;
    }

    cout << dp[K] << endl;
    return 0;
}
