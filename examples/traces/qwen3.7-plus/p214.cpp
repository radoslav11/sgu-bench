#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

const long long INF = 1e18;
long long d[256][256];
long long min_cost_A[256];
long long min_cost_B[256];
char best_A[256];
char best_B[256];

long long dp[2005][2005];
char choice[2005][2005];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string sigma, lambda_str, mu_str;
    if (!(cin >> sigma >> lambda_str >> mu_str)) return 0;

    int sz = sigma.length();
    for (int i = 0; i < 256; ++i) {
        min_cost_A[i] = INF;
        min_cost_B[i] = INF;
        best_A[i] = 0;
        best_B[i] = 0;
        for (int j = 0; j < 256; ++j) {
            d[i][j] = INF;
        }
    }

    for (int i = 0; i < sz; ++i) {
        for (int j = 0; j < sz; ++j) {
            long long w;
            cin >> w;
            d[(unsigned char)sigma[i]][(unsigned char)sigma[j]] = w;
        }
    }

    for (char c1 : sigma) {
        for (char c2 : sigma) {
            long long w = d[(unsigned char)c1][(unsigned char)c2];
            if (w < min_cost_A[(unsigned char)c1]) {
                min_cost_A[(unsigned char)c1] = w;
                best_A[(unsigned char)c1] = c2;
            } else if (w == min_cost_A[(unsigned char)c1]) {
                if (best_A[(unsigned char)c1] == 0 || c2 < best_A[(unsigned char)c1]) {
                    best_A[(unsigned char)c1] = c2;
                }
            }

            if (w < min_cost_B[(unsigned char)c2]) {
                min_cost_B[(unsigned char)c2] = w;
                best_B[(unsigned char)c2] = c1;
            } else if (w == min_cost_B[(unsigned char)c2]) {
                if (best_B[(unsigned char)c2] == 0 || c1 < best_B[(unsigned char)c2]) {
                    best_B[(unsigned char)c2] = c1;
                }
            }
        }
    }

    int n = lambda_str.length();
    int m = mu_str.length();

    for (int i = 0; i <= n; ++i) {
        for (int j = 0; j <= m; ++j) {
            dp[i][j] = INF;
        }
    }
    dp[0][0] = 0;

    for (int i = 0; i <= n; ++i) {
        for (int j = 0; j <= m; ++j) {
            if (i == 0 && j == 0) continue;

            long long cost;

            if (i > 0 && j > 0) {
                cost = dp[i-1][j-1] + d[(unsigned char)lambda_str[i-1]][(unsigned char)mu_str[j-1]];
                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                    choice[i][j] = 0;
                }
            }

            if (i > 0) {
                cost = dp[i-1][j] + min_cost_A[(unsigned char)lambda_str[i-1]];
                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                    choice[i][j] = 1;
                }
            }

            if (j > 0) {
                cost = dp[i][j-1] + min_cost_B[(unsigned char)mu_str[j-1]];
                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                    choice[i][j] = 2;
                }
            }
        }
    }

    string alpha = "";
    string beta = "";
    int i = n;
    int j = m;
    while (i > 0 || j > 0) {
        int c = choice[i][j];
        if (c == 0) {
            alpha += lambda_str[i-1];
            beta += mu_str[j-1];
            i--;
            j--;
        } else if (c == 1) {
            alpha += lambda_str[i-1];
            beta += best_A[(unsigned char)lambda_str[i-1]];
            i--;
        } else if (c == 2) {
            alpha += best_B[(unsigned char)mu_str[j-1]];
            beta += mu_str[j-1];
            j--;
        }
    }
    reverse(alpha.begin(), alpha.end());
    reverse(beta.begin(), beta.end());

    cout << dp[n][m] << "\n";
    cout << alpha << "\n";
    cout << beta << "\n";

    return 0;
}
