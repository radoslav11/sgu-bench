#include <iostream>
#include <vector>
#include <iomanip>
#include <cstring>

using namespace std;

double dp[13][13][8192];
double ans[7][7];
double P[13];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    for (int i = 0; i < 12; ++i) {
        cin >> P[i];
    }

    double internet_sum = 0;
    for (int i = 0; i < N; ++i) {
        double p;
        cin >> p;
        internet_sum += p;
    }
    P[12] = internet_sum / N;

    memset(dp, 0, sizeof(dp));
    memset(ans, 0, sizeof(ans));

    dp[0][0][8191] = 1.0;

    for (int rounds = 0; rounds <= 10; ++rounds) {
        for (int c = 0; c <= rounds; ++c) {
            int v = rounds - c;
            if (c == 6 || v == 6) continue;
            
            for (int mask = 0; mask < 8192; ++mask) {
                if (__builtin_popcount(mask) != 13 - rounds) continue;
                double prob = dp[c][v][mask];
                if (prob == 0.0) continue;

                for (int j = 0; j < 13; ++j) {
                    if ((mask >> j) & 1) {
                        int i = j - 1;
                        while (i >= 0 && !((mask >> i) & 1)) i--;
                        if (i < 0) {
                            i = j + 12;
                            while (!((mask >> i) & 1)) i--;
                        }
                        
                        int dist = (j - i + 13) % 13;
                        if (dist == 0) dist = 13;
                        double p_choose = dist / 13.0;

                        double p_correct = P[j];
                        int next_mask = mask ^ (1 << j);

                        if (c + 1 == 6) {
                            ans[6][v] += prob * p_choose * p_correct;
                        } else {
                            dp[c + 1][v][next_mask] += prob * p_choose * p_correct;
                        }

                        if (v + 1 == 6) {
                            ans[c][6] += prob * p_choose * (1.0 - p_correct);
                        } else {
                            dp[c][v + 1][next_mask] += prob * p_choose * (1.0 - p_correct);
                        }
                    }
                }
            }
        }
    }

    cout << fixed << setprecision(3);
    cout << ans[6][0] << "\n";
    cout << ans[6][1] << "\n";
    cout << ans[6][2] << "\n";
    cout << ans[6][3] << "\n";
    cout << ans[6][4] << "\n";
    cout << ans[6][5] << "\n";
    cout << ans[5][6] << "\n";
    cout << ans[4][6] << "\n";
    cout << ans[3][6] << "\n";
    cout << ans[2][6] << "\n";
    cout << ans[1][6] << "\n";
    cout << ans[0][6] << "\n";

    return 0;
}
