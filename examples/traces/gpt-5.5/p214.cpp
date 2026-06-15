#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string sigma, lambda, mu;
    if (!getline(cin, sigma)) return 0;
    getline(cin, lambda);
    getline(cin, mu);

    int K = (int)sigma.size();
    vector<int> id(256, -1);
    for (int i = 0; i < K; ++i) {
        id[(unsigned char)sigma[i]] = i;
    }

    vector<vector<long long>> d(K, vector<long long>(K));
    for (int i = 0; i < K; ++i) {
        for (int j = 0; j < K; ++j) {
            cin >> d[i][j];
        }
    }

    vector<long long> rowMin(K, LLONG_MAX), colMin(K, LLONG_MAX);
    vector<int> rowArg(K, 0), colArg(K, 0);

    for (int i = 0; i < K; ++i) {
        for (int j = 0; j < K; ++j) {
            if (d[i][j] < rowMin[i]) {
                rowMin[i] = d[i][j];
                rowArg[i] = j;
            }
            if (d[i][j] < colMin[j]) {
                colMin[j] = d[i][j];
                colArg[j] = i;
            }
        }
    }

    int n = (int)lambda.size();
    int m = (int)mu.size();
    int W = m + 1;

    const long long INF = (1LL << 62);

    vector<long long> dp((n + 1) * (m + 1), INF);
    vector<unsigned char> parent((n + 1) * (m + 1), 0);

    auto at = [&](int i, int j) -> int {
        return i * W + j;
    };

    dp[at(0, 0)] = 0;

    for (int i = 0; i <= n; ++i) {
        for (int j = 0; j <= m; ++j) {
            if (i == 0 && j == 0) continue;

            long long best = INF;
            unsigned char par = 0;

            if (i > 0) {
                int x = id[(unsigned char)lambda[i - 1]];
                long long val = dp[at(i - 1, j)] + rowMin[x];
                if (val < best) {
                    best = val;
                    par = 1;
                }
            }

            if (j > 0) {
                int y = id[(unsigned char)mu[j - 1]];
                long long val = dp[at(i, j - 1)] + colMin[y];
                if (val < best) {
                    best = val;
                    par = 2;
                }
            }

            if (i > 0 && j > 0) {
                int x = id[(unsigned char)lambda[i - 1]];
                int y = id[(unsigned char)mu[j - 1]];
                long long val = dp[at(i - 1, j - 1)] + d[x][y];
                if (val < best) {
                    best = val;
                    par = 3;
                }
            }

            dp[at(i, j)] = best;
            parent[at(i, j)] = par;
        }
    }

    string alpha, beta;
    int i = n, j = m;

    while (i > 0 || j > 0) {
        unsigned char par = parent[at(i, j)];

        if (par == 1) {
            int x = id[(unsigned char)lambda[i - 1]];
            alpha.push_back(lambda[i - 1]);
            beta.push_back(sigma[rowArg[x]]);
            --i;
        } else if (par == 2) {
            int y = id[(unsigned char)mu[j - 1]];
            alpha.push_back(sigma[colArg[y]]);
            beta.push_back(mu[j - 1]);
            --j;
        } else {
            alpha.push_back(lambda[i - 1]);
            beta.push_back(mu[j - 1]);
            --i;
            --j;
        }
    }

    reverse(alpha.begin(), alpha.end());
    reverse(beta.begin(), beta.end());

    cout << dp[at(n, m)] << '\n';
    cout << alpha << '\n';
    cout << beta << '\n';

    return 0;
}
