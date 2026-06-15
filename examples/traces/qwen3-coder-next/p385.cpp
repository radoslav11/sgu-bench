#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;

int main() {
    int n;
    cin >> n;

    vector<vector<double>> F(n+1, vector<double>(n+1, 0.0));

    for (int M = 0; M <= n; M++) {
        F[0][M] = 1.0;
    }

    for (int k = 1; k <= n; k++) {
        for (int M = 0; M <= n; M++) {
            if (M < 2) {
                F[k][M] = 0.0;
            } else {
                double sum = 0.0;
                int j_max = min(k, M);
                for (int j = 2; j <= j_max; j++) {
                    sum += F[k - j][M];
                }
                F[k][M] = sum / k;
            }
        }
    }

    double d_n = F[n][n];
    double total_sum = 0.0;
    for (int L = 2; L <= n; L++) {
        int k = n - L;
        total_sum += F[k][L];
    }

    double ans = total_sum / d_n;
    cout << fixed << setprecision(15) << ans << endl;

    return 0;
}
