#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

int main() {
    int N;
    cin >> N;
    vector<int> c(N);
    for (int i = 0; i < N; i++) {
        cin >> c[i];
    }
    int V;
    cin >> V;

    const int INF = V + 1;
    vector<int> dp(V + 1, INF);
    dp[0] = 0;

    for (int i = 1; i <= V; i++) {
        for (int j = 0; j < N; j++) {
            if (c[j] <= i && dp[i - c[j]] != INF) {
                dp[i] = min(dp[i], dp[i - c[j]] + 1);
            }
        }
    }

    if (dp[V] > V) {
        cout << -1 << endl;
    } else {
        cout << dp[V] << endl;
    }

    return 0;
}
