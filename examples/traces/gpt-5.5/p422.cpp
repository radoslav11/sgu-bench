#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long double t;
    cin >> n >> t;

    vector<long double> a(n + 1);
    for (int i = 1; i <= n; ++i) cin >> a[i];

    vector<long double> dp(n + 1, 0.0L);
    dp[n] = 0.0L;

    const long double INF = 1e100L;
    const long double EPS = 1e-18L;

    for (int i = n - 1; i >= 0; --i) {
        long double pSelf = a[i + 1];
        long double denom = 1.0L - pSelf;

        if (denom <= EPS) {
            dp[i] = INF;
            continue;
        }

        long double pref = 1.0L; // probability that all chars before current r are correct
        long double S = 0.0L;    // sum of probabilities of non-self first errors
        long double B = 0.0L;    // weighted sum for non-self first errors

        dp[i] = INF;

        for (int r = i + 1; r <= n; ++r) {
            long double pFirstErrorHere = pref * a[r];

            if (r != i + 1) {
                S += pFirstErrorHere;
                B += pFirstErrorHere * (1.0L - r + dp[r - 1]);
            }

            pref *= (1.0L - a[r]); // probability of no errors on segment i+1..r

            long double m = r - i;

            long double numerator =
                m + t                       // typing segment and looking
                + pref * dp[r]              // no errors
                + pSelf * m                 // first char is wrong: erase whole segment, return to i
                + S * r + B;                // other first-error cases

            long double cand = numerator / denom;
            if (cand < dp[i]) dp[i] = cand;
        }
    }

    cout << fixed << setprecision(15) << (double)dp[0] << '\n';
    return 0;
}
